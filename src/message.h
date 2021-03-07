#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include "mistakes.h"

class Message
{
public:

    Message (){};

    Message(int _number,MistakeTypes _condition,QString _text, QString _checkSum,
            bool _protokol) :
        number(_number), condition(_condition), text(_text),checkSum(_checkSum),
        tcp_imitation(_protokol){};


    int number;       //Номер сообщения
    MistakeTypes condition;  //Состояние сообщения
    QString text;     //Текст сообщения
    QString checkSum; //Контрольная сумма
    bool tcp_imitation;  //Номер протокола

};

#endif // MESSAGE_H
