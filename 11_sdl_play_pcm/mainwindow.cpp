#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <SDL2/SDL.h>

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

void showVersion() {
    SDL_version version;
    SDL_VERSION(&version);
    qDebug() << version.major << version.minor << version.patch;
}

void MainWindow::on_playButton_clicked()
{
    showVersion();

    if (_playThread) { // 停止播放
        _playThread->requestInterruption();
        _playThread = nullptr;
        ui->playButton->setText("开始播放");
    } else {
        _playThread = new PlayThread(this);
        _playThread->start();
        ui->playButton->setText("停止播放");
        // 监听线程结束(也就是自然播放的时候自动切换变成开始播放状态)
        connect(_playThread, &PlayThread::finished, [this]() {
            _playThread = nullptr;
            ui->playButton->setText("开始播放");
        });
    }
}

