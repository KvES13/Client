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
      int countTcpDatagrams;
      int countUdpDatagrams;
      int sizeTcp;
      int sizeUdp;
      int timeTcp;
public slots:
  //  quint32 check_order (quint32 &prev, quint32 &cur );
  //  quint32 check_sum (int &sum, QString &message );

    void showArray(QByteArray arr);

private slots:
    void on_SendButton_clicked();
    void on_ClearButton_clicked();
    void OnTimer();
    void on_lineSizeTcp_textChanged(const QString &arg1);
    void on_lineTimeTcp_textChanged(const QString &arg1);
    void on_lineCountUdp_textChanged(const QString &arg1);
    void on_lineSizeUdp_textChanged(const QString &arg1);
    void FillTcp();
    void FillUdp();
    void on_lineCountTcp_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
