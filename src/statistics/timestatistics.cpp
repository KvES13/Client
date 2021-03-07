#include "timestatistics.h"


void TimeStatistics::Clear()
{
    avg =0;
    total =0;
    min =0;
    max =0;
    is_tcp_imitation = false;
}

void TimeStatistics::Calucalate(int time,int receivedDatagramNumber)
{
    is_tcp_imitation = true;

    total +=time;

    if(min > time) min = time;
    if (max < time) max = time;

    avg = static_cast<double>(total/receivedDatagramNumber);
}

QString TimeStatistics::GetInfo()
{
    if(is_tcp_imitation)
        return "Приблизительное время приема/передачи:\n"
                "Минимальное ="+ QString::number(min)+ "мс "
                "Среднее =" +QString::number(avg) +"мс "
                "Максимальное =" +QString::number(max) + "мс "
                "Общее =" + QString::number(total) +"мс";
    return "";
}
