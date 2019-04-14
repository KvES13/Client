#include "client.h"

//#define address QHostAddress::LocalHost
//#define address QHostAddress("192.168.1.39")
#define serveraddress QHostAddress::LocalHost
//#define serveraddress QHostAddress("192.168.204.129")
#define serverport 5000



Client::Client(QObject *parent) : QObject(parent)
{
    udpsocket = new QUdpSocket(this);


    tcpList = new QList<Message*>();
    udpList = new QList<Message*>();

    //Вот это мб в конструкторе в заголовочном прописывать?
    receivedTcpDatagramNumber = 0;
    receivedUdpDatagramNumber = 0;
    sentUdpDatagramNumber =0;
    sentTcpDatagramNumber =0;

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(ReadDatagrams()));

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
}

Client::~Client()
{
    DeleteList();
    udpsocket->close();
}
//Удаление списков
void Client::DeleteList()
{
    if (tcpList)
    {
        foreach (Message *abc, *tcpList)
        {
            delete abc;
            abc = nullptr;
        }
        delete tcpList;
        tcpList = nullptr;
    }
    if (udpList)
    {
        foreach (Message *abc, *udpList)
        {
            delete abc;
            abc = nullptr;
        }
        delete udpList;
        udpList = nullptr;
    }
}
//Очистка TCP списка
void Client::ClearTcpList()
{
    tcpList->clear();
}
//Очистка UDP списка
void Client::ClearUdpList()
{
    udpList->clear();
}
//Заполнение TCP списка
void Client::FillTcpList(int id, QString data)
{

     tcpList->append(new Message(id,1,data));

}
//Заполнение UDP списка
void Client::FillUdpList(int id, QString data)
{
     udpList->append(new Message(id,0,data+"udp"));
}
//Отправление первой TCP датаграммы
//и датаграмм по таймеру
void Client::SendTcpDatagrams()
{
    OnTimer();
    timer->start();
}

//Отправка TCP датаграмм
void Client::OnTimer()
{
    if(!tcpList->isEmpty())
    {
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out << tcpList->at(0)->number
        << tcpList->at(0)->protocol
        << tcpList->at(0)->text;
    udpsocket->writeDatagram(message, serveraddress, serverport);
    sentTcpDatagramNumber++;
    }

}

//Отправка UDP датаграмм
void Client::SendUdpDatagrams()
{
    foreach (Message *msg, *udpList)
    {
        QByteArray message;
        QDataStream out(&message, QIODevice::WriteOnly);
        out << msg->number
            << msg->protocol
            << msg->text;
        udpsocket->writeDatagram(message, serveraddress, serverport);
        sentUdpDatagramNumber++;

    }

}



void Client::ShowList()
{
//    foreach (Message *mes, *tcpList)
//    {
//        qDebug() << mes->protocol<<" "<<mes->number << mes->text;
//    }

////    foreach (Message *mes, *udpList)
////    {
////        qDebug() << mes->protocol<<" "<<mes->number << mes->text;
////    }
}

//Чтение входящих датаграмм
void Client::ReadDatagrams()
{
    while(udpsocket->hasPendingDatagrams())
    {

        QHostAddress sender;
        quint16 senderPort;
        QByteArray datagram;
        quint8 current_protocol;

        int current_number;
        QString msg;
        datagram.resize(udpsocket->pendingDatagramSize());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        udpsocket->readDatagram(datagram.data(),
                                datagram.size(),
                                &sender,
                                &senderPort);

          in >> current_number >> current_protocol >> msg;
          if(current_protocol)
          {
              receivedTcpDatagramNumber++;
              timer->stop();
              tcpList->removeAt(0);
              if(tcpList->isEmpty())
                  timer->stop();
              else {
                   SendTcpDatagrams();
                   }


          }
          else
          {
             receivedUdpDatagramNumber++;
          }


          QByteArray arr;
          QDataStream out(&arr, QIODevice::WriteOnly);
          out<<sender<<senderPort<<current_number<<current_protocol<<msg;
          emit array(arr);
    }
}
//Адрес сервера
QString Client::GetServerAdrress()
{
    return QHostAddress(serveraddress).toString();
}
//Порт сервера
QString Client::GetServerPort()
{
    return QString::number(serverport);
}
//Число полученных UDP датаграмм
int Client::GetReceivedUdpDatagramNumber()
{
    return receivedUdpDatagramNumber;
}
//Число полученных TCP датаграмм
int Client::GetReceivedTcpDatagramNumber()
{
    return receivedTcpDatagramNumber;
}
//Число отправленных UDP датаграмм
int Client::GetSentUdpDatagramNumber()
{
    return sentUdpDatagramNumber;
}
//Число отправленных TCP датаграмм
int Client::GetSentTcpDatagramNumber()
{
    return sentTcpDatagramNumber;
}

