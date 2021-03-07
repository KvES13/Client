#ifndef MISTAKES_H
#define MISTAKES_H

#include <QString>

enum class MistakeTypes : int
{
    Success = 0,
    CheckSum,
    Duplicate,
    Order
};


class Mistakes
{
public:
    Mistakes():checkSumCount(0),orderCount(0),duplicateCount(0),total(0){};

    void Clear();
    void Calculate(MistakeTypes type);
    QString GetInfo();
    int GetTotalCount()const {return  total;}

private:
    int checkSumCount;
    int orderCount;
    int duplicateCount;
    int total;

};

#endif // MISTAKES_H
