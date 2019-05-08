#include "client.h"

//#define address QHostAddress::LocalHost
//#define address QHostAddress("192.168.1.39")
//#define serveraddress QHostAddress::LocalHost
#define serveraddress QHostAddress("192.168.204.129")
#define serverport 5000


Client::Client(QObject *parent) : QObject(parent)
//Client::Client(int count,int size, int time)
{


    udpsocket = new QUdpSocket(this);


    List = new QList<Message*>();
    //udpList = new QList<Message*>();

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(ReadDatagrams()));

    timer = new QTimer(this);
    timerRec = new QTimer(this);
   // timer->setInterval(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    connect(timerRec,SIGNAL(timeout()),this,SLOT(MsgTimeOut()));
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
void Client::FillList(int count,bool protocol, int size, int timeTcp)
{
    //Размер сообщения
    QString msg;
    for(int i = 0; i < size; i++)
    {
        msg+="a";
    }

    //Заполнение
    for (int j=0;j<count; j++)
    {
        //Рассчет контрольной суммы
        QByteArray sum = msg.toUtf8()+ QString::number(j).toUtf8();
        QString MD5sum = QCryptographicHash::hash(sum, QCryptographicHash::Md5).toHex();
        //Добавление структуры в список
        List->append(new Message(j,protocol,msg,MD5sum));
    }

     //Установка значения для таймера
     timer->setInterval(timeTcp);


     if(timeTcp<1000)
     {
        timerRec->setInterval(2000);
     }
     else
     {
        timerRec->setInterval(timeTcp*2);
     }



}


//Отправление первой TCP датаграммы
//и датаграмм по таймеру
void Client::SendTcpDatagrams()
{
    OnTimer();
    timer->start();
    timerRec->start();
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
void Client:: MsgTimeOut()
{
    QByteArray arr;
    arr +="Не удается получить ответ от " ;
    emit array(arr);
}
//Отправка UDP датаграмм
void Client::SendUdpDatagrams()
{
    timerRec->start();
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
    //Остановка таймеров
    timer->stop();
    timerRec->stop();
    //Очистка списка
    List->clear();
    //Обнуление значений
    sentDatagramNumber =0;
    receivedDatagramNumber=0;
}

//Чтение входящих датаграмм
void Client::ReadDatagrams()
{
    while(udpsocket->hasPendingDatagrams())
    {
        //Адрес получателя
        QHostAddress sender;
        //Порт получателя
        quint16 senderPort;
        //Входящая датаграмма
        QByteArray datagram;
        //Номер протокола
        bool current_protocol;
        //Номер датаграммы
        quint32 current_number;
        //Сообщение
        QString msg;
       // QString sum;
        datagram.resize(udpsocket->pendingDatagramSize());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        udpsocket->readDatagram(datagram.data(),
                                datagram.size(),
                                &sender,
                                &senderPort);

          in >> current_number >> current_protocol >> msg;
          //Остановка таймера таймаута
          timerRec->stop();
          //Если пришло сообщение с протоколом 1
          if(current_protocol)
          {
              //Остановка таймера
              timer->stop();
              //Удаление первого элемента из списка
              List->removeAt(0);
              //Если список пустой
              if(List->isEmpty())
                  timer->stop();
              //Отправка следующей датаграммы
              else {
                   SendTcpDatagrams();
                   }
          }
             //Количество полученных датаграмм +1
             receivedDatagramNumber++;


           //Массив для вывода на экран
           QByteArray arr;
          arr +="IP: "+ sender.toString()+"  Port:" + QString::number(senderPort)
                  +"  Номер: " + QString::number(current_number)
                  + "  Протокол: "+ QString::number(current_protocol)
                  + "  Сообщение: " + msg;
          //Сигнал, передающийся в mainwindow
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


