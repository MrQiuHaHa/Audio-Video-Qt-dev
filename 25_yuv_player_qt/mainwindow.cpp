#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "yuvplayer.h"
#include <QDebug>

static int yuvIdx = 0;
static Yuv yuvs[] = {
    {
        "/Users/davis/Desktop/out.yuv",
        640, 480,
        AV_PIX_FMT_YUYV422,
        30
    }
};

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


void MainWindow::on_playButton_clicked()
{
    if (_player->isPlaying()) { // 歌曲正在播放
        _player->pause();
    } else { // 歌曲没有正在播放
        _player->play();
    }
}


void MainWindow::on_stopButton_clicked()
{
    _player->stop();
}


void MainWindow::on_pushButton_3_clicked()
{
    int yuvCount = sizeof (yuvs) / sizeof (Yuv);
    yuvIdx = ++yuvIdx % yuvCount;

    _player->stop();
    _player->setYuv(yuvs[yuvIdx]);
    _player->play();
}

void MainWindow::onPlayerStateChanged() {
    if (_player->getState() == YuvPlayer::Playing) {
        ui->playButton->setText("暂停");
    } else { // 不是处于正在播放状态
        ui->playButton->setText("播放");
    }
}
