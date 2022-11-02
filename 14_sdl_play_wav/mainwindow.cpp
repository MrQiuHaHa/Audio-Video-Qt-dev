#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SDL2/SDL.h>
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
    if (_playThread) { // 停止播放
        _playThread->requestInterruption();
        _playThread = nullptr;
        ui->pushButton->setText("播放wav");
    } else { // 开始播放
        _playThread = new PlayThread(this);
        _playThread->start();
        // 监听线程的结束
        connect(_playThread, &PlayThread::finished,
        [this]() {
            _playThread = nullptr;
            ui->pushButton->setText("播放wav");
        });
        ui->pushButton->setText("停止播放");
    }
}

