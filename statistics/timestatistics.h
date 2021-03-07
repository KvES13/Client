#ifndef TIMESTATISTICS_H
#define TIMESTATISTICS_H

#include <QString>

class TimeStatistics
{
public:
    TimeStatistics(): avg(0),total(0),min(1),max(0),is_tcp_imitation(false){};

    void Clear();
    void Calucalate(int time, int receivedDatagramNumber);
    QString GetInfo();



private:
    //Время
    double avg;
    int total;
    int min;
    int max;
    bool is_tcp_imitation;
};

#endif // TIMESTATISTICS_H
