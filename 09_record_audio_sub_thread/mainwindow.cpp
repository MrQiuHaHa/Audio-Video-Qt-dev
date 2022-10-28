#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

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


void MainWindow::on_audioButton_clicked()
{
    if (!_audioThread) { //点击了开始录音
        // 开启线程
        _audioThread = new AudioThread(this);
        _audioThread->start();

        //设置按钮文字
        ui->audioButton->setText("结束录音");

        connect(_audioThread, &AudioThread::finished, [this]() {
           _audioThread = nullptr;
           ui->audioButton->setText("开始录音");
        });

    } else {
        // 结束线程
        _audioThread->requestInterruption();
        _audioThread = nullptr;

        //设置按钮文字
        ui->audioButton->setText("开始录音");
    }
}
