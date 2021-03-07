#include "mistakes.h"


void Mistakes::Clear()
{
    checkSumCount =0;
    orderCount = 0;
    duplicateCount = 0;
    total = 0;
}

void Mistakes::Calculate(MistakeTypes type)
{
    switch (type) {
    case MistakeTypes::CheckSum:
        checkSumCount++;
        break;
    case MistakeTypes::Duplicate:
        duplicateCount++;
        break;
    case MistakeTypes::Order:
            orderCount++;
        break;
    default:
        return;
    }

    total++;
}

QString Mistakes::GetInfo()
{
    return  "Ошибки: "
            "Всего = " +QString::number(total) + " "
            "Контрольная сумма = " + QString::number(checkSumCount)
            +" Порядок = "+QString::number(orderCount) +
            " Дублирование = "+QString::number(duplicateCount)+"\n\n";
}

