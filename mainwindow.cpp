#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    QString msg;
    for(int i=0;i<30;i++)
    {
        msg+="a";
    }
    for (int i=0;i<datagrams_count;i++)
    {
        client->FillList(i,msg);

    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_SendButton_clicked()
{
//    QString msg;
//    for(int i=0;i<30;i++)
//    {
//        msg+="a";
//    }
//    for (int i=0;i<datagrams_count;i++)
//    {
//        client->SendDatagrams(i,msg);
//    }
    client->ShowList();
    client->SendUdpDatagrams();
    client->SendTcpDatagrams();

}


void MainWindow::showMessage(QString msg)
{
   ui->plainTextEdit->appendPlainText(msg);
}


void MainWindow::showID(QString id)
{
 ui->plainTextEdit->appendPlainText(id);
}



void MainWindow::on_ClearButton_clicked()
{
    ui->plainTextEdit->clear();
    ui->textBrowser->clear();
    count_send=0;
    count_rec=0;
}
void MainWindow::on_lineCount_textChanged(const QString &arg1)
{
    datagrams_count = arg1.toInt();

}
