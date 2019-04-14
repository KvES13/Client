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

void Client::FillList(int id, QString data)
{
     udpList->append(new Message(id,0,data+"udp"));
     tcpList->append(new Message(id,1,data));
}

void Client::SendTcpDatagrams()
{
    OnTimer();
    timer->start();
}

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

//Удалить потом///////////////////////////
void Client::Send(int i,QString s)
{
     QByteArray message;
     QDataStream out(&message, QIODevice::WriteOnly);
     quint8 p =0;
     out<<i<<p<<s;
     udpsocket->writeDatagram(message,message.size(), serveraddress, serverport);
       qDebug()<<QString::number(message.size())<<"Send"<<i;
}
// ///////////////////////////////////////
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

QString Client::GetServerAdrress()
{
    return QHostAddress(serveraddress).toString();
}

QString Client::GetServerPort()
{
    return QString::number(serverport);
}

int Client::GetReceivedUdpDatagramNumber()
{
    return receivedUdpDatagramNumber;
}
int Client::GetReceivedTcpDatagramNumber()
{
    return receivedTcpDatagramNumber;
}
int Client::GetSentUdpDatagramNumber()
{
    return sentUdpDatagramNumber;
}
int Client::GetSentTcpDatagramNumber()
{
    return sentTcpDatagramNumber;
}

