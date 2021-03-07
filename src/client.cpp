#include "client.h"

//#define address QHostAddress::LocalHost
#define address QHostAddress("192.168.1.39")
//#define serveraddress QHostAddress::LocalHost
//#define serveraddress QHostAddress("192.168.1.62")
#define serveraddress QHostAddress("127.0.0.1")
#define serverport 5000


Client::Client(QObject *parent) : QObject(parent)
{
    udpsocket = new QUdpSocket(this);

    statHandler = std::make_unique<StatisticsHandler>();

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(ReadDatagrams()));

    timer = new QTimer(this);
    timerRec = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    connect(timerRec,SIGNAL(timeout()),this,SLOT(MsgTimeOut()));
}

Client::~Client()
{
    udpsocket->close();
}


//Заполнение списка
void Client::FillList(int count,bool protocol, int size, int timeTcp)
{
//32713 = 65507
    //Заполнение
    SizeMessage = size;
    for (int j=0;j<count; j++)
    {
        QString msg;
        for(int i = 0; i < (size-81)/2; i++)
        {
            msg+=char(rand()%26+0x61); //Заполнение рандомными буквами
        }
        //Рассчет контрольной суммы + QString::number(j).toUtf8()
        QByteArray sum = msg.toUtf8();
        QString MD5sum = QCryptographicHash::hash(sum, QCryptographicHash::Md5).toHex();
        //Добавление структуры в список
        messages.append(Message {j,MistakeTypes::Success,msg,MD5sum,protocol});
    }

     //Установка значения для таймера
     timer->setInterval(timeTcp);

     //Тайм-аут соединения
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
    if(!messages.isEmpty())
    {
        QByteArray message;

        QDataStream out(&message, QIODevice::WriteOnly);
        out << messages[0].number
            << messages[0].tcp_imitation
            << messages[0].text
            << messages[0].checkSum
            << messages[0].condition;
        udpsocket->writeDatagram(message, serveraddress, serverport);
        time.start();
        statHandler->datagrams.sentNumber++;
        statHandler->datagrams.sentSize+=message.size();
    }
}
void Client:: MsgTimeOut()
{
    QString errorMsg ="Не удается получить ответ от "+serveraddress.toString() +
            QString::number(serverport); //serveraddress.toString()
    emit array(errorMsg);
}
//Отправка UDP датаграмм
void Client::SendUdpDatagrams()
{
    timerRec->start();
    for (auto &&msg : messages)
    {
        QByteArray message;
        QDataStream out(&message, QIODevice::WriteOnly);

        out << msg.number
            << msg.tcp_imitation
            << msg.text
            << msg.checkSum
            << msg.condition;
        udpsocket->writeDatagram(message, serveraddress, serverport);

        statHandler->datagrams.sentNumber++;
        statHandler->datagrams.sentSize+=message.size();
    }
}

void Client::Reset()
{
    //Остановка таймеров
    timer->stop();
    timerRec->stop();
    //Обнуление значений
    messages.clear();
    statHandler->Clear();
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

        Message msg {};

        datagram.resize(udpsocket->pendingDatagramSize());

        QDataStream in(&datagram, QIODevice::ReadOnly);

        udpsocket->readDatagram(datagram.data(),
                                datagram.size(),
                                &sender,
                                &senderPort);

        in >> msg.number >> msg.tcp_imitation >>
              msg.text   >> msg.checkSum >> msg.condition;

        //Остановка таймера таймаута
        timerRec->stop();
        QString timeZe ="";
        int t =0;

        //Количество полученных датаграмм +1
        statHandler->datagrams.receivedNumber++;
        //Если пришло сообщение с протоколом 1
        if(msg.tcp_imitation)
        {
            //Остановка таймера
            timer->stop();
            t= time.elapsed();

            if(msg.condition == MistakeTypes::Success ||
                    msg.condition == MistakeTypes::Duplicate)
            {
                //Удаление первого элемента из списка
                messages.removeAt(0);
            }
            //Если список пустой
            if(messages.isEmpty())
                timer->stop();
            //Отправка следующей датаграммы
            else
                SendTcpDatagrams();

           statHandler->time.Calucalate(t,statHandler->datagrams.receivedNumber);

           timeZe = "Время = "+ QString::number(t) +"мс ";
      }

        //Подсчет ошибок
        statHandler->mistakes.Calculate(msg.condition);
        //Размер сообщений
        statHandler->datagrams.receivedSize+=datagram.size();
        //Подсчет неудачных отправлений
        statHandler->datagrams.Calculate(statHandler->mistakes.GetTotalCount());

       //Массив для вывода на экран
      //Сигнал, передающийся в mainwindow
      emit array("Ответ от 192.168.1.62 "+ QString::number(senderPort)
               +"  Номер =" + QString::number(msg.number)
               +"  Размер =" + QString::number(SizeMessage) + " байт " +
               timeZe+ " Состояние = " +
               QString::number(static_cast<int>(msg.condition)));


      emit stat(statHandler->getInfo());
    }
}


//Адрес сервера
QString Client::GetServerAdrress() const
{
    return QHostAddress(serveraddress).toString();
}
//Порт сервера
QString Client::GetServerPort() const
{
    return QString::number(serverport);
}

void Client::SetServerPort(quint16 servPort)
{
 //   serverport = servPort;
}
void Client::SetServerAddress(QHostAddress  servAddress)
{
   // serveraddress = servAddress;
}





