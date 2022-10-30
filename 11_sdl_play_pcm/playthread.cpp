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
#define SAMPLEs 1024
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

/*
SDL播放音频有2种模式：
Push（推）：【程序】主动推送数据给【音频设备】
Pull（拉）：【音频设备】主动向【程序】拉取数据（我们当前使用的模式）
*/
void PlayThread::run() {

}
