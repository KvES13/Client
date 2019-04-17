#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    client = new Client();

    //Вывод на экран ip адрес получателя
    ui->lineAddress_2->setText((client->GetServerAdrress()));
    //Вывод на экран номер порта получателя
    ui->linePort_2->setText((client->GetServerPort()));

    //Задание начальных значений
    //Количество сообщений
    countTcpDatagrams=ui->lineCountTcp->text().toInt();

    //Размер сообщения
    sizeTcp=ui->lineSizeTcp->text().toInt();

    //Время таймера
    timeTcp=ui->lineTimeTcp->text().toInt();

    connect(client,SIGNAL(array(QByteArray)),this,SLOT(showArray(QByteArray)));

    //Таймер для вывода информации об отправленных/полученных сообщениях
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));

    //Заполнение списков /////////////////////////////////////////
    FillTcp();
    FillUdp();
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

//Заполнение TCP
void MainWindow::FillTcp()
{
    QString msg;
    for(int i = 0; i < sizeTcp; i++)
    {
        msg+="a";
    }

    for (int i = 0; i < countTcpDatagrams; i++)
    {
        client->FillTcpList(i, msg);
    }
}
//Заполнение UDP
void MainWindow::FillUdp()
{
    QString msg;
    for(int i = 0; i < sizeUdp; i++)
    {
        msg+="a";
    }

    for (int i = 0; i < countUdpDatagrams; i++)
    {
        client->FillUdpList(i, msg);
    }
}
void MainWindow::OnTimer()
{
   // ХЗ КАК НОРМ СДЕЛАТЬ
    QString sentTcp = QString::number(client->GetSentTcpDatagramNumber());
    QString sentUdp = QString::number(client->GetSentUdpDatagramNumber());
    QString recTcp = QString::number(client->GetReceivedTcpDatagramNumber());
    QString recUdp = QString::number(client->GetReceivedUdpDatagramNumber());

    ui->textBrowser->setText("TCP Отправлено/Получено: "+sentTcp +"/" + recTcp+"\n"
                             + "UDP Отправлено/Получено: " + sentUdp + "/" + recUdp);
}

void MainWindow::on_SendButton_clicked()
{
//    for (int i = 0;i<10;i++)
//        client->Send(i,"123456789");
    timer->start();
    client->SendUdpDatagrams();
    client->SendTcpDatagrams();
}




void MainWindow::showArray(QByteArray arr)
{
  QDataStream in(&arr, QIODevice::ReadOnly);
  quint32 number;
  quint8 protocol;
  QString msg;
  QHostAddress sender;
  quint16 senderPort;

  in>>sender>>senderPort>>number>>protocol>>msg;
  ui->plainTextEdit->appendPlainText("IP: "+ sender.toString()+"  Port:" + QString::number(senderPort)
                                     +"  Номер: " + QString::number(number)
                                     + "  Протокол: "+ QString::number(protocol)
                                     + "  Сообщение: " + msg);
}


//Обрабокта нажатия на кнопку "Удалить"
void MainWindow::on_ClearButton_clicked()
{
    //Остановка таймера
    timer->stop();
    //Очистка ?
    ui->plainTextEdit->clear();
    ui->textBrowser->clear();
    //Обнуление количества отправленных сообщений
    count_send = 0;
    //Обнуление количества полученных сообщений
    count_rec = 0;

    //Очистка списка
    client->ClearTcpList();
    //Заполнение списка
    FillTcp();

    client->ClearUdpList();
    FillUdp();

}


// Число TCP датаграмм
void MainWindow::on_lineCountTcp_textChanged(const QString &arg1)
{
    countTcpDatagrams = arg1.toInt();
    client->ClearTcpList();
    FillTcp();
}


//Размер TCP сообщения
void MainWindow::on_lineSizeTcp_textChanged(const QString &arg1)
{
    sizeTcp = arg1.toInt();
    client->ClearTcpList();
    FillTcp();
}
//Время задержки TCP датаграммы
void MainWindow::on_lineTimeTcp_textChanged(const QString &arg1)
{
    timeTcp = arg1.toInt();
    client->ClearTcpList();
    FillTcp();
}



