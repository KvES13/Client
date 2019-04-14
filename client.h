
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QList>
#include <QTimer>
#include <QDebug>
/* Сообщение */
struct Message
{
    int number;
    quint8 protocol;
    QString text;

    Message(int _number,quint8 _protocol, QString _text)
    {
        number = _number;
        protocol = _protocol;
        text = _text;
    }
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

signals:

    void array(QByteArray arr);

public slots:
    void SendTcpDatagrams();
    void SendUdpDatagrams();
    void ReadDatagrams();
    void ShowList(); // потом удалить
    void FillTcpList(int id, QString data);
    void FillUdpList(int id, QString data);
    int GetReceivedUdpDatagramNumber();
    int GetReceivedTcpDatagramNumber();
    int GetSentUdpDatagramNumber();  // Sent или по-другому?
    int GetSentTcpDatagramNumber();  // Sent или по-другому?
    QString GetServerAdrress();
    QString GetServerPort();

    void ClearTcpList();
    void ClearUdpList();

private:
    QUdpSocket *udpsocket = nullptr;
    QHostAddress address;
    quint16 port;
    QList<Message*> *tcpList = nullptr;
    QList<Message*> *udpList = nullptr;
    QTimer *timer = nullptr;
    int receivedUdpDatagramNumber;
    int receivedTcpDatagramNumber;
    int sentUdpDatagramNumber; //Sent или по-другому?
    int sentTcpDatagramNumber; // Sent или по-другому?
private slots:
    void OnTimer();
    void DeleteList();  //пока в пУблик

};

#endif // CLIENT_H

