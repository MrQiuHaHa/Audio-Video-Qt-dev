#include "mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include "receiver.h"
#include "sender.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QPushButton *btn = new QPushButton;
    btn->setText("点击发送信号");
    btn->setFixedSize(100, 40);
    btn->setParent(this);
    connect(btn, &QPushButton::clicked, this, &MainWindow::handleClick);
    connect(btn, &QPushButton::clicked, []() {
        qDebug() << "点击了Window上的按钮--触发了lambda方式的槽函数";
    });
    Sender *sender = new Sender;
    connect(sender, &Sender::exit, [](int n1, int n2) {
        qDebug() << "Sender槽函数Lambda" << n1 << n2;
    });

    Receiver *receiver = new Receiver;
    connect(sender, &Sender::exit, receiver, &Receiver::handleExit);
    connect(sender, &Sender::exit, receiver, &Receiver::handleExit2);
    qDebug() << "返回值 == " << emit sender->exit(10, 20);
    delete sender;
    delete receiver;
}

void MainWindow::handleClick()
{
    qDebug() << "点击了Window上的按钮--触发了槽函数handleClick()";
}

MainWindow::~MainWindow()
{
}

