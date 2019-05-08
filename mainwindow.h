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
      int countDatagrams;

      //Размер одного сообщения
      int sizeMessage;
      //Время для повторной отправки сообщения по протоколу tcp
      int timeTcp;
      //
      bool radioButTcp;
public slots:
  //  quint32 check_order (quint32 &prev, quint32 &cur );
  //  quint32 check_sum (int &sum, QString &message );

    void showArray(QByteArray arr);

private slots:
    //Обработка нажатия на кнопку "Отправить"
    void on_SendButton_clicked();
    //Обработка нажатия на кнопку "Удалить"
    void on_ClearButton_clicked();
    //Изменение значения размера сообщения
    void on_lineSizeTcp_textChanged(const QString &arg1);
    //Изменение времени таймера
    void on_lineTimeTcp_textChanged(const QString &arg1);
    //Изменение количества отправляемых сообщений
    void on_lineCountTcp_textChanged(const QString &arg1);
    //Заполнение списков
    void FillList();
    //Проверка чекбокса
    bool CheckRetry();


    void on_checkBox_Retry_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
