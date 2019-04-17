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
    countDatagrams=ui->lineCountTcp->text().toInt();
    //Размер сообщения
    sizeMessage=ui->lineSizeTcp->text().toInt();
    //Время таймера
    timeTcp=ui->lineTimeTcp->text().toInt();

    radioButTcp = false;

    connect(client,SIGNAL(array(QByteArray)),this,SLOT(showArray(QByteArray)));

    //Таймер для вывода информации об отправленных/полученных сообщениях
    timer = new QTimer(this);
    timer->setInterval(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));

    //Заполнение списка /////////////////////////////////////////
    FillList();
    //FillUdp();
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

//Заполнение Списка
void MainWindow::FillList()
{
    //Сделать нормально
    QString msg;
    for(int i = 0; i < sizeMessage; i++)
    {
        msg+="a";
    }

    for (int i = 0; i < countDatagrams; i++)
    {
        if(radioButTcp)
        {
            client->FillTcpList(i, msg);
        }
        else
        {
            client->FillUdpList(i,msg);
        }
    }
}

void MainWindow::OnTimer()
{
    //
    count_send = client->GetSentDatagramNumber();
    count_rec = client->GetReceivedDatagramNumber();

    ui->textBrowser->setText("TCP Отправлено/Получено: "+QString::number(count_send) +"/" +
                                QString::number(count_rec)+"\n");
//    ui->lineSizeTcp->setEnabled(false);
//    ui->lineTimeTcp->setEnabled(false);
//    ui->lineCountTcp->setEnabled(false);
}

void MainWindow::on_SendButton_clicked()
{
//    for (int i = 0;i<10;i++)
//        client->Send(i,"123456789");
    timer->start();
    if(radioButTcp)
    {
    client->SendTcpDatagrams();
    }
    else
    {
    client->SendUdpDatagrams();
    }

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
//    ui->lineSizeTcp->setEnabled(true);
//    ui->lineTimeTcp->setEnabled(true);
//    ui->lineCountTcp->setEnabled(true);
    //Обнуление количества отправленных сообщений
    count_send = 0;
    //Обнуление количества полученных сообщений
    count_rec = 0;

    //Очистка списка
    client->ClearList();
    //Заполнение списка
    FillList();

}


// Число датаграмм
void MainWindow::on_lineCountTcp_textChanged(const QString &arg1)
{
    countDatagrams = arg1.toInt();
    client->ClearList();
    FillList();
}


//Размер сообщения
void MainWindow::on_lineSizeTcp_textChanged(const QString &arg1)
{
    sizeMessage = arg1.toInt();
    client->ClearList();
    FillList();
}
//Время задержки TCP датаграммы
void MainWindow::on_lineTimeTcp_textChanged(const QString &arg1)
{
    timeTcp = arg1.toInt();
    client->ClearList();
    FillList();
}




void MainWindow::on_radioButtonTcp_clicked()
{
    ui->lineTimeTcp->setEnabled(true);
    radioButTcp = true;
}

void MainWindow::on_radioButtonUdp_clicked()
{
    ui->lineTimeTcp->setEnabled(false);
    radioButTcp = false;
}
