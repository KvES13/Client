#include "statistics_handler.h"

StatisticsHandler::StatisticsHandler()
{
}

void StatisticsHandler::Clear()
{
    mistakes.Clear();
    datagrams.Clear();
    time.Clear();
}

QString StatisticsHandler::getInfo()
{
    return datagrams.GetInfo()+ time.GetInfo() +
           "\nПроцент потерь = "+
            QString::number(datagrams.percentLostMsg)+"%\n"+
            mistakes.GetInfo()+
            "Процент успешно доставленных сообщений=  "+
            QString::number(datagrams.percentSuccess) +"%";
}
