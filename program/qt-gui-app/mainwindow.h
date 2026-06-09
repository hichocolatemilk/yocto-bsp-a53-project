#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    Ui::MainWindow *ui;

    QTimer *timer;
    QUdpSocket *udpSocket;
    void udpServer_init();
    void udpServer_read();
    void updateMessage(const QString &msg);
    void updateDummyMessage(const QString &msg);
private slots:
    void Slot_updateDummy();
};
#endif // MAINWINDOW_H
