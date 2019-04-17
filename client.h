
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
    int number;       //Номер сообщения
    quint8 protocol;  //Номер протокола
    QString text;     //Текст сообщения

    //Конструктор
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
    //Отправление датаграмм по TCP
    void SendTcpDatagrams();
    //ОТправление датаграмм по UDP
    void SendUdpDatagrams();
    //Чтение входящих датаграмм
    void ReadDatagrams();
    void ShowList(); // потом удалить
    //Заполнение списка TCP сообщений
    void FillTcpList(int id, QString data);
    //Заполнение списка UDP сообщений
    void FillUdpList(int id, QString data);
    //Число полученных в ответ датаграмм
    int GetReceivedUdpDatagramNumber();
    int GetReceivedTcpDatagramNumber();
    //Число отправленных датаграмм
    int GetSentUdpDatagramNumber();  // Sent или по-другому?
    int GetSentTcpDatagramNumber();  // Sent или по-другому?
    //IP адрес получателя
    QString GetServerAdrress();
    //Номер протокола получателя
    QString GetServerPort();

    //Очистка списка
    void ClearTcpList();
    void ClearUdpList();

private:
    //
    QUdpSocket *udpsocket = nullptr;
    //IP адрес
    QHostAddress address;
    //Номер порта
    quint16 port;
    //Список сообщений
    QList<Message*> *tcpList = nullptr;
    QList<Message*> *udpList = nullptr;
    //Таймер для повторной отправки сообщения
    //по протоколу TCP
    QTimer *timer = nullptr;
    //Число полученных датаграмм
    int receivedUdpDatagramNumber;
    int receivedTcpDatagramNumber;
    //Число отправленных датаграмм
    int sentUdpDatagramNumber; //Sent или по-другому?
    int sentTcpDatagramNumber; // Sent или по-другому?
private slots:
    //Отправка повторного сообщения по истечению таймера
    void OnTimer();
    //Удаление списка
    void DeleteList();  //пока в пУблик

};

#endif // CLIENT_H

