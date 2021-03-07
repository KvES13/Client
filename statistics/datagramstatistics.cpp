#include "datagramstatistics.h"

DatagramStatistics::DatagramStatistics()
{

}

void DatagramStatistics::Clear()
{
    sentSize = 0;
    receivedSize = 0;
    sentNumber = 0;
    receivedNumber = 0;

    percentLostMsg = 0;
    percentSuccess = 0;
}

void DatagramStatistics::Calculate(int mistakes_count)
{
    percentLostMsg=  static_cast<double>((1-(receivedNumber/sentNumber))*100);
    percentSuccess = static_cast<double>(((receivedNumber-mistakes_count)/sentNumber)*100);
}

QString DatagramStatistics::GetInfo()
{    
    return "\nОтправлено = " +QString::number(sentNumber)
            +" ("+QString::number(sentSize) + "байт)  "
            "Получено=  "+ QString::number(receivedNumber) +
            " (" + QString::number(receivedSize) + " байт)\n";
}
