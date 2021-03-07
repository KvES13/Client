#ifndef STATISTICS_HANDLER_H
#define STATISTICS_HANDLER_H

#include "mistakes.h"
#include "timestatistics.h"
#include "datagramstatistics.h"

#include <QByteArray>

class StatisticsHandler
{
public:
    StatisticsHandler();


    void Clear();
    QString getInfo();

    Mistakes mistakes {};
    TimeStatistics time {};
    DatagramStatistics datagrams;

private:


};


#endif // STATISTICS_HANDLER_H
