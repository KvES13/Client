#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QByteArray>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new Client(this);

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

    connect(client,SIGNAL(array(const QString&)),this,SLOT(showArray(const QString&)));
    connect(client,SIGNAL(stat(const QString&)),this,SLOT(showInfo(const QString&)));

}

MainWindow::~MainWindow()
{
//    delete client;
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
    client->Reset();
    FillList();

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


void MainWindow::showArray(const QString &arr)
{

  //Вывод массива с ифнормацией о сообщении на экран
  ui->plainTextEdit->appendPlainText(arr);


}
void MainWindow::showInfo(const QString &info)
{
     ui->textBrowser->setText("Начальные значения:\n"
                              "IP адрес " +client->GetServerAdrress()+
                              " Порт " + client->GetServerPort() +"\n" +
                              "Повтор сообщения: " +str +
                              " Количество "+ QString::number(countDatagrams) +
                              " Размер " + QString::number(sizeMessage)+"\n" +
                                info);

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


}


// Число датаграмм
void MainWindow::on_lineCountTcp_textChanged(const QString &arg1)
{
    countDatagrams = arg1.toInt();
}
//Размер сообщения
void MainWindow::on_lineSizeTcp_textChanged(const QString &arg1)
{
    sizeMessage = arg1.toInt();
}
//Время задержки TCP датаграммы
void MainWindow::on_lineTimeTcp_textChanged(const QString &arg1)
{
    timeTcp = arg1.toInt();

}

//Проверка чекбокса
bool MainWindow::CheckRetry()
{
    //Если выбран пункт "Повторная отправка"
    if(ui->checkBox_Retry->isChecked())
    {
        ui->lineTimeTcp->setEnabled(true);
        str = "ВКЛ";
        return true;
    }
    else
    {
        ui->lineTimeTcp->setEnabled(false);
        str = "ВЫКЛ";
        return false;
    }
}

//Изменение состояния чекбокса
void MainWindow::on_checkBox_Retry_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    CheckRetry();
}

//Корректность номера порта
void MainWindow::on_linePort_2_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if (ui->linePort_2->text().toInt() >65553)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Введите номер порта \n в диапазоне от 0 до 65535");

    }
}
//Корректность номера порта Ip
void MainWindow::on_lineAddress_2_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if (ui->lineAddress_2->text().toInt() >255)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","IP-адрес должен быть в формате\n\
                               XXX.XXX.XXX.XXX (XXX - от 0 до 255)");

    }
}

void MainWindow::on_lineSizeTcp_editingFinished()
{
    if (ui->lineSizeTcp->text().toInt() >65507)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Размер пользовательских данных \n не должен превышать 65507 байт");

    }
}

void MainWindow::on_ClearButton_2_clicked()
{
    QMessageBox messageBox;
    messageBox.information(0,"Info","Данные сохранены в файл \n output2205.csv");

}
