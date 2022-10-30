#include "playthread.h"
#include <QDebug>
#include <SDL2/SDL.h>
#include <QFile>

#define FILENAME "~/Desktop/test.pcm"
// 采样率 (这是mac，如果是windows默认44100)
#define SAMPLE_RATE 48000
// 采样格式 (这是mac，如果是windows默认AUDIO_S16LSB)
#define SAMPLE_FORMAT AUDIO_F32LSB
// 单个声道的采样大小 (位深)
#define SAMPLE_SIZE SDL_AUDIO_BITSIZE(SAMPLE_FORMAT)
// 声道数 (windows默认是2)
#define CHANNELS 1
// 音频缓冲区的样本数量
#define SAMPLES 1024
// 每个样本占用多少个字节
#define BYTES_PER_SAMPLE (SAMPLE_SIZE * CHANNELS / 8)
// 文件缓冲区的大小
#define BUFFER_SIZE (SAMPLES * BYTES_PER_SAMPLE)

typedef struct AudioBuffer {
    int len = 0;
    int pullLen = 0;
    Uint8 *data = nullptr;
} AudioBuffer;

PlayThread::PlayThread(QObject *parent)
    : QThread{parent}
{
    connect(this, &PlayThread::finished, this, &PlayThread::deleteLater);
}

PlayThread::~PlayThread() {
    disconnect();
    requestInterruption();
    quit();
    wait();
    qDebug() << this << "析构";
}

// 等待音频设备回调(会回调多次,每当消费完了音频缓冲区的样本，就会再次回调获取新的音频数据)
void pull_audio_data(void *userdata, Uint8 *stream, int len) {
    qDebug() << "pull_audio_data -- " << len;

    // 清空stream (静音处理)
    SDL_memset(stream, 0, len);

    // 取出AudioBuffer
    AudioBuffer *buffer = (AudioBuffer *) userdata;

    // 文件数据还没准备好
    if (buffer->len <= 0) return;

    // 取len、bufferLen的最小值（为了保证数据安全，防止指针越界）
    buffer->pullLen = (len > buffer->len) ? buffer->len : len;

    // 填充数据
    SDL_MixAudio(stream, buffer->data, buffer->pullLen, SDL_MIX_MAXVOLUME);
    buffer->data += buffer->pullLen;
    buffer->len -= buffer->pullLen;
}

/*
SDL播放音频有2种模式：
Push（推）：【程序】主动推送数据给【音频设备】
Pull（拉）：【音频设备】主动向【程序】拉取数据（我们当前使用的模式）
*/
void PlayThread::run() {
    // 初始化Audio子系统
    if (SDL_Init(SDL_INIT_AUDIO)) {
        qDebug() << "SDL_Init error" << SDL_GetError();
        return;
    }

    // 音频参数
    SDL_AudioSpec spec;
    // 采样率
    spec.freq = SAMPLE_RATE;
    // 采样格式 (s16le or f32le)
    spec.format = SAMPLE_FORMAT;
    // 声道数
    spec.channels = CHANNELS;
    // 音频缓冲区的样本数量 （这个值必须是2的幂次方）
    spec.samples = SAMPLES;
    // 回调
    spec.callback = pull_audio_data;
    // 传递给回调的参数（由我们自定义）
    AudioBuffer buffer;
    spec.userdata = &buffer;

    // 打开设备
    if (SDL_OpenAudio(&spec, nullptr)) {
        qDebug() << "SDL_OpenAudio error" << SDL_GetError();
        // 清除所有的子系统
        SDL_Quit();
        return;
    }

    // 打开文件
    QFile file(FILENAME);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "file open error" << FILENAME;
        // 关闭设备
        SDL_CloseAudio();
        //清除子系统
        SDL_Quit();
        return;
    }

    // 开始播放 (0是取消暂停，也就是播放)
    SDL_PauseAudio(0);

    // 存放从文件中读取的数据
    Uint8 data[BUFFER_SIZE];
    while (!isInterruptionRequested()) {
        // 只要从文件中读取的音频数据，还没有填充到音频缓冲区，就跳过
        if (buffer.len > 0) continue;

        buffer.len = file.read((char *) data, BUFFER_SIZE);

        // 文件数据已经读取完毕
        if (buffer.len <= 0) {
            // 剩余的样本数量
            int samples = buffer.pullLen / BYTES_PER_SAMPLE;
            int ms = samples / SAMPLE_RATE * 1000;
            SDL_Delay(ms);
            break;
        }

        // 读取到了pcm文件的数据
        buffer.data = data;
    }

    // 关闭文件
    file.close();

    // 关闭设备
    SDL_CloseAudio();

    // 清除所有的子系统
    SDL_Quit();
}
