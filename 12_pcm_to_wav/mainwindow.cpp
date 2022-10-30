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


void MainWindow::on_pushButton_clicked()
{
    if (!_audioThread) { // 点击了“pcm转wav”
        // 开启线程
        _audioThread = new AudioThread(this);
        _audioThread->start();

        connect(_audioThread, &AudioThread::finished,
        [this]() { // 线程结束
            _audioThread = nullptr;
            ui->pushButton->setEnabled(true);
        });

        // 设置按钮文字
        ui->pushButton->setEnabled(false);
    }
}

