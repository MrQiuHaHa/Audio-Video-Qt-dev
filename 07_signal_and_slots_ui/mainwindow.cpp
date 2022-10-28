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


void MainWindow::on_loginBtn_clicked()
{
    qDebug() << "loginBtn: " << ui->loginBtn;
}


void MainWindow::on_registerBtn_clicked()
{
    qDebug() << "registerBtn: " << ui->registerBtn;
}

