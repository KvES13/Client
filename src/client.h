
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
#include <QTime>
#include <QDateTime>
#include <memory>
#include "message.h"
#include "statistics_handler.h"




class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    ~Client();

signals:
    //Сигнал, выводящий информацию о входящем сообщении
    void array(const QString& arr);
    //Сигнал, выводящий общую статистику
    void stat(const QString& info);

public slots:
    //Отправление датаграмм по TCP
    void SendTcpDatagrams();
    //ОТправление датаграмм по UDP
    void SendUdpDatagrams();
    //Чтение входящих датаграмм
    void ReadDatagrams();
    // Сброс
    void Reset();
    //Заполнение списка сообщений
    void FillList(int count,bool protocol, int size, int timeTcp);
    //IP адрес получателя
    QString GetServerAdrress() const;
    //Номер протокола получателя
    QString GetServerPort() const;
    void SetServerPort(quint16 port);
    void SetServerAddress(QHostAddress  servAddress);
private:

    QUdpSocket *udpsocket = nullptr;
    //IP адрес
    QHostAddress address;
    //Номер порта
    quint16 port;
    //Список сообщений
    QVector<Message> messages;

    //Тайм-аут соединения
    QTimer *timerRec = nullptr;
    //Таймер для повторной отправки сообщения
    QTimer *timer = nullptr;
    QTime time;

    int SizeMessage;

   std::unique_ptr<StatisticsHandler> statHandler;

private slots:
    //Отправка повторного сообщения по истечению таймера
    void OnTimer();
    void MsgTimeOut();

};

#endif // CLIENT_H

