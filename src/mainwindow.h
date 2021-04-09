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
      //Заданное количество сообщений
      int countDatagrams;

      //Размер одного сообщения
      int sizeMessage;
      //Время для повторной отправки сообщения
      int timeTcp;
      //Состояние радиобатона "Повторная отправка"
      bool radioButTcp;
      QString retryMsgState;
public slots:

    void showArray(const QString & arr);
    void showInfo (const QString &info);

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
    void on_linePort_2_textChanged(const QString &arg1);
    void on_lineAddress_2_textChanged(const QString &arg1);
    void on_lineSizeTcp_editingFinished();
    void on_ClearButton_2_clicked();
};

#endif // MAINWINDOW_H
