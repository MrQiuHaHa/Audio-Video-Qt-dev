#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QThread>

extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

#ifdef Q_OS_WIN
    // 格式名称
    #define FMT_NAME "dshow"
    // 设备名称
    #define DEVICE_NAME "audio=线路输入 (3- 魅声T800)"
    //录制的PCM文件名称
    #define FILE_NAME "F:/out.pcm"
#else
    // 格式名称
    #define FMT_NAME "avfoundation"
    // 设备名称
    #define DEVICE_NAME ":0"
    //录制的PCM文件名称
    #define FILE_NAME "/Users/davis/Desktop"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_recordButton_clicked()
{
    qDebug() << "on_recordButton_clicked" << QThread::currentThread();
    return;

    const AVInputFormat *fmt = av_find_input_format(FMT_NAME);
    if (!fmt) {
        av_log(nullptr, AV_LOG_ERROR, "获取输入格式对象失败");
        return;
     }
    // 获取格式上下文
    AVFormatContext *ctx = nullptr;
    // 打开设备
    int ret = avformat_open_input(&ctx, DEVICE_NAME, fmt, nullptr);
    if (ret < 0) {
        char errbuf[1024];
        av_strerror(ret, errbuf, sizeof(errbuf));
        av_log(nullptr, AV_LOG_ERROR, "打开设备失败%s", errbuf);
        return;
    }

    // 文件名
    QFile file(FILE_NAME);

    // 打开文件
    if (!file.open(QFile::WriteOnly)) {
        av_log(nullptr, AV_LOG_ERROR, "打开文件失败");
        //关闭设备
        avformat_close_input(&ctx);
        return;
    }

    //设置采集次数
    int count = 30;

    // 数据包
    AVPacket pkt;
    //循环采集
    while (count-- > 0 && av_read_frame(ctx, &pkt) == 0) {
        //单次采集完毕，把包里的数据写入文件（其实只是写到了文件缓冲区）
        file.write((const char *) pkt.data, pkt.size);
    }

    // 释放资源，关闭文件
    file.close();

    // 关闭设备上下文
    avformat_close_input(&ctx);
}

