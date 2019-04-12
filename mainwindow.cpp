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

    datagrams_count=ui->lineCount->text().toInt();

    connect(client,SIGNAL(id(QString)),this,SLOT(showID(QString)));
    connect(client,SIGNAL(_message(QString)),this,SLOT(showMessage(QString)));
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

    for (int i = 0; i < 100; i++)
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


void MainWindow::showMessage(QString msg)
{
    //ui->plainTextEdit->appendPlainText(msg);
}


void MainWindow::showID(QString id)
{
    //ui->plainTextEdit->appendPlainText(id);
}

void MainWindow::showArray(QByteArray arr)
{
  QDataStream in(&arr, QIODevice::ReadOnly);
  int n;
  quint8 p;
  QString m;

  in>>n>>p>>m;
  ui->plainTextEdit->appendPlainText("Номер: " + QString::number(n) + "\tПротокол: "+ QString::number(p) + "\tСообщение: " + m);
}

void MainWindow::on_ClearButton_clicked()
{
    timer->stop();
    ui->plainTextEdit->clear();
    ui->textBrowser->clear();
    count_send = 0;
    count_rec = 0;
}
void MainWindow::on_lineCount_textChanged(const QString &arg1)
{
    datagrams_count = arg1.toInt();

//    timer->stop();

//    ////////////////////////////
//    client->DeleteList();
//    QString msg;
//    for(int i = 0; i < 30; i++)
//    {
//        msg+="a";
//    }

//    for (int i = 0; i < datagrams_count; i++)
//    {
//        client->FillList(i, msg);
//    }
//    /////////////////////////////
}
