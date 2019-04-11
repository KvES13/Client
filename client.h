
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QList>
#include <QTimer>

/* Сообщение */
struct Message
{
    int number;
    bool protocol;
    QString text;

    Message(int _number,bool _protocol, QString _text)
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
    void id(QString curr);
    void _message(QString mess);

public slots:
    void SendTcpDatagrams();
    void SendUdpDatagrams();
    void ReadDatagrams();
    QString GetServerAdrress();
    QString GetServerPort();
    void ShowList();
    void FillList(int id, QString data);
    int GetReceivedUdpDatagramNumber();
    int GetReceivedTcpDatagramNumber();

private:
  //  Message *msg;
    QUdpSocket *udpsocket = nullptr;
    QHostAddress address;
    quint16 port;
    bool flag = false;
    QList<Message*> *tcpList = nullptr;
    QList<Message*> *udpList = nullptr;
    QTimer *timer = nullptr;
    QByteArray message;
    int receivedUdpDatagramNumber;
    int receivedTcpDatagramNumber;

private slots:
    void OnTimer();

};

#endif // CLIENT_H

