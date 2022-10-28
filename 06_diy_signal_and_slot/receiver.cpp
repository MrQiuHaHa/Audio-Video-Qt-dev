#include "receiver.h"
#include <QDebug>

Receiver::Receiver(QObject *parent)
    : QObject{parent}
{

}

int Receiver::handleExit(int n1, int n2)
{
    qDebug() << "Receiver::handleExit" << n1 << n2;
    return n1 + n2;
}

void Receiver::handleExit2(int n1, int n2)
{
    qDebug() << "Receiver::handleExit2" << n1 << n2;
}
