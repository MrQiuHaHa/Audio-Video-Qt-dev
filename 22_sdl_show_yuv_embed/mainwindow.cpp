#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playthread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _widget = new QWidget(this);
    _widget->setGeometry(200, 50, 512, 512);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    PlayThread *thread = new PlayThread((void *) _widget->winId(), this);
    thread->start();
}

