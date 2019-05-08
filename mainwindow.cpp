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



    connect(client,SIGNAL(array(QByteArray)),this,SLOT(showArray(QByteArray)));


    //Заполнение списка
    FillList();

}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

//Заполнение списка с заданными параметрами
void MainWindow::FillList()
{
    client->FillList(countDatagrams,CheckRetry(), sizeMessage,timeTcp);
}

//Обработка нажатия на кнопку "Отправить"
void MainWindow::on_SendButton_clicked()
{

    //Если выбран пункт "Повторная отправка"
    if(CheckRetry())
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

  //Вывод массива с ифнормацией о сообщении на экран
  ui->plainTextEdit->appendPlainText(arr);

  //Число отправленных / полученных датаграмм
  count_send = client->GetSentDatagramNumber();
  count_rec = client->GetReceivedDatagramNumber();

  ui->textBrowser->setText(" Отправлено/Получено: "+QString::number(count_send) +"/" +
                              QString::number(count_rec)+"\n");
}


//Обрабокта нажатия на кнопку "Удалить"
void MainWindow::on_ClearButton_clicked()
{

    //Очистка
    ui->plainTextEdit->clear();
    ui->textBrowser->clear();


    //Обнуление списков,
    //количества отправленных/полученных датаграмм
    client->Reset();
    //Заполнение списка
    FillList();

}


// Число датаграмм
void MainWindow::on_lineCountTcp_textChanged(const QString &arg1)
{
    countDatagrams = arg1.toInt();
    client->Reset();
    FillList();
}


//Размер сообщения
void MainWindow::on_lineSizeTcp_textChanged(const QString &arg1)
{
    sizeMessage = arg1.toInt();
    client->Reset();
    FillList();
}
//Время задержки TCP датаграммы
void MainWindow::on_lineTimeTcp_textChanged(const QString &arg1)
{
    timeTcp = arg1.toInt();
    client->Reset();
    FillList();
}





//Проверка чекбокса
bool MainWindow::CheckRetry()
{
    //Если выбран пункт "Повторная отправка"
    if(ui->checkBox_Retry->isChecked())
    {
        ui->lineTimeTcp->setEnabled(true);
        return true;
    }
    else
    {
        ui->lineTimeTcp->setEnabled(false);
        return false;
    }
}

//Изменение состояния чекбокса
void MainWindow::on_checkBox_Retry_stateChanged(int arg1)
{
    CheckRetry();
}
