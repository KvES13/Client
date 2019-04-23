#include "client.h"

//#define address QHostAddress::LocalHost
//#define address QHostAddress("192.168.1.39")
#define serveraddress QHostAddress::LocalHost
//#define serveraddress QHostAddress("192.168.204.129")
#define serverport 5000


Client::Client(QObject *parent) : QObject(parent)
//Client::Client(int count,int size, int time)
{


    udpsocket = new QUdpSocket(this);


    List = new QList<Message*>();
    //udpList = new QList<Message*>();

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(ReadDatagrams()));

    timer = new QTimer(this);
   // timer->setInterval(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
}

Client::~Client()
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
    udpsocket->close();
}



//Заполнение списка
void Client::FillList(quint32 id,bool protocol, QString data, int timeTcp)
{

     QByteArray sum = data.toUtf8()+ QString::number(id).toUtf8();
     QString MD5sum = QCryptographicHash::hash(sum, QCryptographicHash::Md5).toHex();

     List->append(new Message(id,protocol,data,MD5sum));
     timer->setInterval(timeTcp);

     qDebug()<<MD5sum<<MD5sum.size();



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
        << List->at(0)->text
        << List->at(0)->checkSum;
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
            << msg->text
            << msg->checkSum;
        udpsocket->writeDatagram(message, serveraddress, serverport);
        sentDatagramNumber++;
    }

}



void Client::Reset()
{
    timer->stop();
    List->clear();
    sentDatagramNumber =0;
    receivedDatagramNumber=0;
}

//Чтение входящих датаграмм
void Client::ReadDatagrams()
{
    while(udpsocket->hasPendingDatagrams())
    {

        QHostAddress sender;
        quint16 senderPort;
        QByteArray datagram;
        bool current_protocol;

        quint32 current_number;
        QString msg;
       // QString sum;
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


