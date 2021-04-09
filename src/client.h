
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QCryptographicHash>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>

#include <QDateTime>

#include <vector>

#include "message.h"
#include "statistics_handler.h"

constexpr int kMaxIPValue = 255;
constexpr uint16_t kMaxPort = 65535;
constexpr int kMinInterval = 1000;




class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    ~Client();

signals:
    //Сигнал, выводящий информацию о входящем сообщении
    void SendCurrentMsgStat(const QString& arr);
    //Сигнал, выводящий общую статистику
    void SendTotalStat(const QString& info);

public slots:
    //Отправление датаграмм по TCP
    void SendTcpDatagrams();
    //ОТправление датаграмм по UDP
    void SendUdpDatagrams();
    //Чтение входящих датаграмм
    void ReadDatagrams();
    // Обнуление значений
    void Reset();
    //Заполнение списка сообщений
    void FillList(int count,bool protocol, int size, int timeTcp);
    //IP адрес получателя
    QString GetServerAdrress() const;
    //Номер протокола получателя
    QString GetServerPort() const;
    void SetServerPort(uint16_t port);
    void SetServerAddress(QHostAddress  servAddress);
private:

    //UDP сокет
    QUdpSocket *udpsocket = nullptr;
    //IP адрес
    QHostAddress serverAddress;
    //Номер порта
    uint16_t serverPort;
    //Список сообщений
    QVector<Message> messages;

    //Тайм-аут соединения
    QTimer timerRec;
    //Таймер для повторной отправки сообщения
    QTimer timer;
    QTime time;

    int sizeMessage;

   std::unique_ptr<StatisticsHandler> statHandler;
private slots:
    //Отправка повторного сообщения по истечению таймера
    void OnTimer();
    void MsgTimeOut();

};

#endif // CLIENT_H

