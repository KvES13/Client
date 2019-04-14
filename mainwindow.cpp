#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new Client();

    ui->lineAddress_2->setText((client->GetServerAdrress()));
    ui->linePort_2->setText((client->GetServerPort()));

    countTcpDatagrams=ui->lineCountTcp->text().toInt();
    countUdpDatagrams=ui->lineCountUdp->text().toInt();
    sizeTcp=ui->lineSizeTcp->text().toInt();
    sizeUdp=ui->lineSizeUdp->text().toInt();
    timeTcp=ui->lineTimeTcp->text().toInt();

    connect(client,SIGNAL(array(QByteArray)),this,SLOT(showArray(QByteArray)));

    //Таймер для вывода информации об отправленных/полученных сообщениях
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));

    //Заполнение списков /////////////////////////////////////////
    QString msg;
    for(int i = 0; i < 5; i++)
    {
        msg+="a";
    }

    for (int i = 0; i < countTcpDatagrams; i++)
    {
        client->FillList(i, msg);
    }
    // /////////////////////////////////////
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
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

void MainWindow::on_ClearButton_clicked()
{
    timer->stop();
    ui->plainTextEdit->clear();
    ui->textBrowser->clear();
    count_send = 0;
    count_rec = 0;
}

// Число TCP датаграмм
void MainWindow::on_lineCount_textChanged(const QString &arg1)
{
    countTcpDatagrams = arg1.toInt();

}
//Размер TCP сообщения
void MainWindow::on_lineSizeTcp_textChanged(const QString &arg1)
{
    sizeTcp = arg1.toInt();
}
//Время задержки TCP датаграммы
void MainWindow::on_lineTimeTcp_textChanged(const QString &arg1)
{
    timeTcp = arg1.toInt();
}


//Число UDP датаграмм
void MainWindow::on_lineCountUdp_textChanged(const QString &arg1)
{
    countUdpDatagrams =arg1.toInt();
}
//Размер UDP сообщения
void MainWindow::on_lineSizeUdp_textChanged(const QString &arg1)
{
    sizeUdp = arg1.toInt();
}
