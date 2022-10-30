#include "audiothread.h"
#include <QDebug>
#include "ffmpegs.h""

AudioThread::AudioThread(QObject *parent) : QThread(parent) {
    // 当监听到线程结束时（finished），就调用deleteLater回收内存
    connect(this, &AudioThread::finished,
            this, &AudioThread::deleteLater);
}

AudioThread::~AudioThread() {
    // 断开所有的连接
    disconnect();
    // 内存回收之前，正常结束线程
    requestInterruption();
    // 安全退出
    quit();
    wait();
    qDebug() << this << "析构（内存被回收）";
}

// 当线程启动的时候（start），就会自动调用run函数
// run函数中的代码是在子线程中执行的
// 耗时操作应该放在run函数中
void AudioThread::run() {
    qDebug() << this << "开始执行----------";

    FFmpegs::pcm2wav();

    qDebug() << this << "正常结束----------";

//    char wavHeader[44];
//    wavHeader[0] = 'R';
//    wavHeader[1] = 'I';
//    wavHeader[2] = 'F';
//    wavHeader[3] = 'F';

//    // 10 00 00 00
//    wavHeader[4] = 0x10;
//    wavHeader[5] = 0x00;
//    wavHeader[6] = 0x00;
//    wavHeader[7] = 0x00;
}
