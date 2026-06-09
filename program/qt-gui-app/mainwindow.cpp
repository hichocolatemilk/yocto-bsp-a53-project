#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    // , timer(new QTimer(this))
{
    ui->setupUi(this);
    udpServer_init();
    // timer->start(1000);
    // connect(timer, &QTimer::timeout, this, &MainWindow::Slot_updateDummy);
};

void MainWindow::Slot_updateDummy()
{
    updateDummyMessage("CPU=55;MEM=70;UPTIME=10:20:30");
};

void MainWindow::updateDummyMessage(const QString &msg)
{
    QStringList list = msg.split(";");

    for(const QString &lists:list)
    {
        QStringList data = lists.split("=");
        QString key = data[0].trimmed();//trimmed 앞뒤 공백제거
        QString value = data[1].trimmed();

         // 처음에 오면 아마 저쪽에서 Sprintf 스트링으로 올테니 int로변환
        if(key == "CPU")
        {
            ui->cpuBar->setValue(value.toInt());
        }
        else if (key == "MEM") {
            ui->memoryBar->setValue(value.toInt());
        } else if (key == "UPTIME") {
            ui->uptime->setText(value);
        }
    }

};

void MainWindow::updateMessage(const QString &msg)
{
    QStringList list = msg.split(";");

    for(const QString &lists:list)
    {

        QStringList data = lists.split("=");
        // 이거 없으면 반대쪽에서 프로그램 끊을 시 죽음
        if (data.size() < 2) {
            continue;
        }
        QString key = data[0].trimmed();//trimmed 앞뒤 공백제거
        QString value = data[1].trimmed();

        // 처음에 오면 아마 저쪽에서 Sprintf 스트링으로 올테니 int로변환
        if(key == "CPU")
        {
            ui->cpuBar->setValue(value.toInt());
        }
        else if (key == "MEM") {
            ui->memoryBar->setValue(value.toInt());
        } else if (key == "UPTIME") {
            ui->uptime->setText(value);
        }
    }

};

void MainWindow::udpServer_init()
{
    udpSocket = new QUdpSocket(this);
    // udpSocket->bind(QHostAddress(QString("192.168.35.126")), 8888); //
    udpSocket->bind(QHostAddress::AnyIPv4, 12345);

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &MainWindow::udpServer_read);
}

void MainWindow::udpServer_read()
{
    while(udpSocket->hasPendingDatagrams()){
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString msg = QString::fromUtf8(datagram.data());
        updateMessage(msg);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}