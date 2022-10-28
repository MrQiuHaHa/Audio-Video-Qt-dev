#include "audiothread.h"

#include <QDebug>
#include <QFile>
#include <QDateTime>

extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
}

#ifdef Q_OS_WIN
    // 格式名称
    #define FMT_NAME "dshow"
    // 设备名称
    #define DEVICE_NAME "audio=线路输入 (3- 魅声T800)"
    // PCM文件名
    #define FILEPATH "F:/"
#else
    #define FMT_NAME "avfoundation"
    #define DEVICE_NAME ":0"
    #define FILEPATH "/Users/davis/Desktop/"
#endif

AudioThread::AudioThread(QObject *parent)
    : QThread{parent}
{
    // 当监听到线程结束时（finished），就调用deleteLater回收内存
    connect(this, &AudioThread::finished, this, &AudioThread::deleteLater);
}

AudioThread::~AudioThread() {
    //断开所有的连接
    disconnect();
    //内存回收之前，请求正常的结束线程的执行
    requestInterruption();
    //安全退出
    quit();
    wait();
    qDebug() << this << "子线程内存被回收";
}

void showSpec(const AVFormatContext *ctx) {
    // 获取输入流
    AVStream *stream = ctx->streams[0];
    //获取音频设备的参数（编码器？）
    AVCodecParameters *params = stream->codecpar;

    //声道数量
    av_log(nullptr, AV_LOG_INFO, "channels-- %d \n", params->ch_layout.nb_channels);

//    采样率是指每秒采集的样本数量，称为采样率（采样频率，采样速率，Sampling Rate）。
//    比如，采样率44.1kHz表示1秒钟采集44100个样本。
//    采样率44.1kHZ、位深度16bit的1分钟立体声PCM数据有多大？
//    采样率 * 位深度 * 声道数 * 时间
//    44100 * 16 * 2 * 60 / 8 ≈ 10.34MB
    av_log(nullptr, AV_LOG_INFO, "sample_rate-- %d \n", params->sample_rate);
//    比特率（Bit Rate），指单位时间内传输或处理的比特数量，
//    单位是：比特每秒（bit/s或bps），还有：千比特每秒（Kbit/s或Kbps）、
//    兆比特每秒（Mbit/s或Mbps）、吉比特每秒（Gbit/s或Gbps）、太比特每秒（Tbit/s或Tbps）
//    采样率 * 位深度 * 声道数
//    44100 * 16 * 2 = 1411.2Kbps
    av_log(nullptr, AV_LOG_INFO, "bit_rate-- %lld \n", params->bit_rate);

    //采样格式
    av_log(nullptr, AV_LOG_INFO, "format-- %d \n", params->format);

    // 每一个样本的一个声道占用多少个字节
    av_log(nullptr, AV_LOG_INFO, "bytes-- %d \n", av_get_bytes_per_sample((AVSampleFormat) params->format));
}

// 当线程启动的时候（start），就会自动调用run函数
// run函数中的代码是在子线程中执行的
// 耗时操作应该放在run函数中
void AudioThread::run() {

    //获取输入格式对象
    const AVInputFormat *fmt = av_find_input_format(FMT_NAME);
    if (!fmt) {
        av_log(nullptr, AV_LOG_ERROR, "get input format failed\n");
        return;
    }

    // 获取录音设备的格式上下文
    AVFormatContext *ctx = nullptr;
    int ret;
    //打开设备
    ret = avformat_open_input(&ctx, DEVICE_NAME, fmt, nullptr);
    if (ret < 0) {
        char errbuf[1024];
        av_strerror(ret, errbuf, sizeof(errbuf));
        av_log(nullptr, AV_LOG_ERROR, "open audio device failed\n");
        return;
    }

    // 打印一下录音设备的参数信息
    showSpec(ctx);

    // 文件名
    QString fileName = FILEPATH;

    fileName += QDateTime::currentDateTime().toString("MM_dd_HH_mm_ss");
    fileName += ".pcm";
    QFile file(fileName);

    //打开文件
    // WriteOnly：只写模式。如果文件不存在，就创建文件；如果文件存在，就会清空文件内容
    if (!file.open(QFile::WriteOnly)) {
        av_log(nullptr, AV_LOG_ERROR, "open File Failed\n");
        //关闭设备上下文
        avformat_close_input(&ctx);
        return;
    }

    // 数据包
    AVPacket *pkt = av_packet_alloc();
    while (!isInterruptionRequested()) {
        //不断采集数据
        ret = av_read_frame(ctx, pkt);

        if (ret == 0) { //读取成功
            //将数据写入文件
            file.write((const char *)pkt->data, pkt->size);
        } else if (ret == AVERROR(EAGAIN)) {//临时资源不可用
            continue;
        } else {//其他错误
            char errbuf[1024];
            av_strerror(ret, errbuf, sizeof(errbuf));
            av_log(nullptr, AV_LOG_ERROR, "av_read_frame error\n");
            break;
        }
        //必须要加，释放pkt内部的资源，比如当前pkt内的data成员内存等等
        av_packet_unref(pkt);
    }

    //录音结束，关闭文件
    file.close();

    // 释放资源
    av_packet_free(&pkt);

    //关闭设备
    avformat_close_input(&ctx);

    av_log(nullptr, AV_LOG_INFO, "sub thread over\n");
}

void AudioThread::setStop(bool stop) {
    _stop = stop;
}
