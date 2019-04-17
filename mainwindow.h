#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataStream>
#include <client.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
      Ui::MainWindow *ui;
      //
      Client *client;
      //Таймер
      QTimer *timer = nullptr;
      //Число отправленных сообщений
      int count_send;
      //Число полученных сообщений
      int count_rec;
      //Заданное количество сообщений
      int countTcpDatagrams;
      int countUdpDatagrams;
      //Размер одного сообщения
      int sizeTcp;
      int sizeUdp;
      //Время для повторной отправки
      //сообщения по протоколу tcp
      int timeTcp;
public slots:
  //  quint32 check_order (quint32 &prev, quint32 &cur );
  //  quint32 check_sum (int &sum, QString &message );

    void showArray(QByteArray arr);

private slots:
    //Обработка нажатия на кнопку "Отправить"
    void on_SendButton_clicked();
    //Обработка нажатия на кнопку "Удалить"
    void on_ClearButton_clicked();
    //Таймер, по которому выводится статистика
    void OnTimer();
    //Изменение значения размера сообщения
    void on_lineSizeTcp_textChanged(const QString &arg1);
    //Изменение времени таймера
    void on_lineTimeTcp_textChanged(const QString &arg1);
    //Изменение количества отправляемых сообщений
    void on_lineCountTcp_textChanged(const QString &arg1);
    //Заполнение списков
    void FillTcp();
    void FillUdp();

};

#endif // MAINWINDOW_H
