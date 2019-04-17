#include "client.h"

//#define address QHostAddress::LocalHost
//#define address QHostAddress("192.168.1.39")
#define serveraddress QHostAddress::LocalHost
//#define serveraddress QHostAddress("192.168.204.129")
#define serverport 5000



Client::Client(QObject *parent) : QObject(parent)
{
    udpsocket = new QUdpSocket(this);


    List = new QList<Message*>();
    //udpList = new QList<Message*>();

    //Вот это мб в конструкторе в заголовочном прописывать?
    receivedDatagramNumber = 0;
    sentDatagramNumber =0;


    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(ReadDatagrams()));

    timer = new QTimer(this);
    timer->setInterval(1);
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
    if (List)
    {
        foreach (Message *abc, *List)
        {
            delete abc;
            abc = nullptr;
        }
        delete List;
        List = nullptr;
    }

}

//Очистка списка
void Client::ClearList()
{
    List->clear();
}

//Заполнение TCP списка
void Client::FillTcpList(int id, QString data)
{

     List->append(new Message(id,1,data));

     //

     // Например
  QString s = data + QString::number(id);
     QByteArray ba = data.toUtf8();
     QString d;
     d=  QCryptographicHash::hash(ba, QCryptographicHash::Sha256).toHex();
     //QString t = b
qDebug()<<d;

}
//Заполнение UDP списка
void Client::FillUdpList(int id, QString data)
{
     List->append(new Message(id,0,data+"udp"));

     QString s = data + QString::number(id);
        QByteArray ba = data.toUtf8();
        QString d;
        qDebug()<< QCryptographicHash::hash(ba, QCryptographicHash::Sha256).toHex();
                qDebug()<<"md5"<< QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex();
                        qDebug()<<"sha1"<< QCryptographicHash::hash(ba, QCryptographicHash::Sha1).toHex();
        //QString t = b
  // qDebug()<<d;
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
    if(!List->isEmpty())
    {
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out << List->at(0)->number
        << List->at(0)->protocol
        << List->at(0)->text;
    udpsocket->writeDatagram(message, serveraddress, serverport);
    sentDatagramNumber++;
    }

}

//Отправка UDP датаграмм
void Client::SendUdpDatagrams()
{
    foreach (Message *msg, *List)
    {
        QByteArray message;
        QDataStream out(&message, QIODevice::WriteOnly);
        out << msg->number
            << msg->protocol
            << msg->text;
        udpsocket->writeDatagram(message, serveraddress, serverport);
        sentDatagramNumber++;

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
              timer->stop();
              List->removeAt(0);
              if(List->isEmpty())
                  timer->stop();
              else {
                   SendTcpDatagrams();
                   }
          }

             receivedDatagramNumber++;



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
//Число полученных датаграмм
int Client::GetReceivedDatagramNumber()
{
    return receivedDatagramNumber;
}

//Число отправленных датаграмм
int Client::GetSentDatagramNumber()
{
    return sentDatagramNumber;
}


