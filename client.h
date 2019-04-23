
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QList>
#include <QTimer>
#include <QDebug>
#include <QCryptographicHash>

/* Сообщение */
struct Message
{
    quint32 number;       //Номер сообщения
    bool protocol;  //Номер протокола
    QString text;     //Текст сообщения
    QString checkSum; //Контрольная сумма
    //Конструктор
    Message(int _number,bool _protocol, QString _text, QString _checkSum)
    {
        number = _number;
        protocol = _protocol;
        text = _text;
        checkSum =_checkSum;
    }
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
//    Client(int count, int size, int time)
//    {
////        countDatagram = count;
////        SizeMessage = size;
////        TimeTcp = time;
//    }
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
    // Сброс
    void Reset();
    //Заполнение списка TCP сообщений
    void FillList(quint32 id,bool protocol, QString data, int timeTcp);
    //Число полученных в ответ датаграмм
    int GetReceivedDatagramNumber();
    //Число отправленных датаграмм
    int GetSentDatagramNumber();  // Sent или по-другому?
    //IP адрес получателя
    QString GetServerAdrress();
    //Номер протокола получателя
    QString GetServerPort();


private:
    //
    QUdpSocket *udpsocket = nullptr;
    //IP адрес
    QHostAddress address;
    //Номер порта
    quint16 port;
    //Список сообщений
    QList<Message*> *List = nullptr;
  //  QList<Message*> *udpList = nullptr;
    //Таймер для повторной отправки сообщения
    //по протоколу TCP
    QTimer *timer = nullptr;
    //Число полученных датаграмм
    int receivedDatagramNumber =0;
    //Число отправленных датаграмм
    int sentDatagramNumber =0; //Sent или по-другому?
    int countDatagram;
    int SizeMessage;
    int TimeTcp;


private slots:
    //Отправка повторного сообщения по истечению таймера
    void OnTimer();


};

#endif // CLIENT_H

