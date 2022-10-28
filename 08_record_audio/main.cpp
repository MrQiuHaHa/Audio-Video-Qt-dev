#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QThread>

extern "C" {
#include <libavdevice/avdevice.h>
}

void log() {

    //设置FFPmeg的日志等级
    av_log_set_level(AV_LOG_DEBUG);

    char error[100] = "200";
    av_log(nullptr, AV_LOG_ERROR, "Error Code is %s \n", error);
    av_log(nullptr, AV_LOG_WARNING, "Warning Code is %s \n", error);
    av_log(nullptr, AV_LOG_INFO, "Info Code is %s \n", error);
}

int main(int argc, char *argv[])
{

    //注册
    avdevice_register_all();
    log();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
