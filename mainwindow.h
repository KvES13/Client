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
      Client *client;
      QTimer *timer = nullptr;
      int count_send;
      int count_rec;
      int datagrams_count;
public slots:
  //  quint32 check_order (quint32 &prev, quint32 &cur );
  //  quint32 check_sum (int &sum, QString &message );

    void showMessage(QString msg);
    void showID(QString id);
    void showArray(QByteArray arr);

private slots:
    void on_SendButton_clicked();
    void on_ClearButton_clicked();
    void on_lineCount_textChanged(const QString &arg1);
    void OnTimer();
};

#endif // MAINWINDOW_H
