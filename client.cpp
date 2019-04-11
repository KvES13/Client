#include "client.h"
#include <QDebug>

//#define address QHostAddress::LocalHost
//#define address QHostAddress("192.168.1.39")
#define serveraddress QHostAddress::LocalHost
//#define serveraddress QHostAddress("192.168.204.129")
#define serverport 5000


Client::Client(QObject *parent) : QObject(parent)
{
    udpsocket = new QUdpSocket(this);

  //  Message msg =Message();
    tcpList = new QList<Message*>();
    udpList = new QList<Message*>();

    receivedTcpDatagramNumber = 0;
    receivedUdpDatagramNumber = 0;

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(ReadDatagrams()));

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
}

Client::~Client()
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

    delete udpsocket;
}


void Client::FillList(int id, QString data)
{
     tcpList->append(new Message(id,1,data));
     udpList->append(new Message(id,0,data+"udp"));
}

void Client::SendTcpDatagrams()
{
    OnTimer();
    timer->start();
}

void Client::OnTimer()
{
    QDataStream out(&message, QIODevice::WriteOnly);
    out << tcpList->at(0)->number
        << tcpList->at(0)->protocol
        << tcpList->at(0)->text;
    udpsocket->writeDatagram(message, serveraddress, serverport);

}
void Client::SendUdpDatagrams()
{
  //  QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);

    foreach (Message *msg, *udpList)
    {
        out << msg->number
            << msg->protocol
            << msg->text;
        udpsocket->writeDatagram(message, serveraddress, serverport);
    }
}



void Client::ShowList()
{
    foreach (Message *mes, *tcpList)
    {
        qDebug() << mes->text;
        qDebug() << mes->number;
    }

    foreach (Message *mes, *udpList)
    {
        qDebug() << mes->text;
        qDebug() << mes->number;
    }
}

/*
 * MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
ui->setupUi(this);

_list = new QList<ABC *>();

FillList();
}

void MainWindow::FillList()
{
_list->append(ABC(1, 2));
qDebug() « _list->last().b;
}

MainWindow::~MainWindow()
{
if (_list)
{
foreach (ABC *abc, *_list)
{
delete abc;
abc = nullptr;
}
delete _list;
_list = nullptr;
}

delete ui;
}*/

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
        datagram.resize(udpsocket->pendingDatagramSize());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        udpsocket->readDatagram(datagram.data(),
                                datagram.size(),
                                &sender,
                                &senderPort);

          // quint32 current_number;
          //  int checksum;
        //    QString msg;

          in >> current_number >> current_protocol >> msg;
          if(current_protocol)
          {
              receivedTcpDatagramNumber++;
              timer->stop();
              tcpList->removeAt(0);
              SendTcpDatagrams();

          }
          else
          {
             receivedUdpDatagramNumber++;
          }

          emit id(QString::number(current_number));
          emit _message(msg);
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


