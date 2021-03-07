#ifndef DATAGRAMSTATISTICS_H
#define DATAGRAMSTATISTICS_H

#include <QString>

class DatagramStatistics
{
public:

    DatagramStatistics();
    void Clear();
    void Calculate(int mistakes_count);
    QString GetInfo();

    long long sentSize;
    long long receivedSize;
    int sentNumber;
    int receivedNumber;

    double percentLostMsg;
    double percentSuccess;

private:


};



#endif // DATAGRAMSTATISTICS_H
