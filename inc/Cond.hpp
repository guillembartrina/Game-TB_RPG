#ifndef COND_HPP
#define COND_HPP

#include <utility>

enum CondType
{
    TRUE,
    FALSE,
    EQUAL,
    NOT_EQUAL,
    LESS,
    GREAT,
    LESS_EQUAL,
    GREAT_EQUAL
};

template<typename T>
class Cond
{
public:

    CondType type;
    T value;

    Cond() {}

    Cond(CondType type, T value)
    {
        this->type = type;
        this->value = value;
    }
    
    ~Cond() {}

   bool evalCond(T data, Cond cond) const
    {
        bool res = false;

        switch(cond.type)
        {
            case CondType::TRUE:
                res = true;
                break;
            case CondType::FALSE:
                res = false;
                break;
            case CondType::EQUAL:
                res = (value == cond.value);
                break;
            case CondType::NOT_EQUAL:
                res = (value != cond.value);
                break;
            case CondType::LESS:
                res = (value < cond.value);
                break;
            case CondType::GREAT:
                res = (value > cond.value);
                break;
            case CondType::LESS_EQUAL:
                res = (value <= cond.value);
                break;
            case CondType::GREAT_EQUAL:
                res = (value >= cond.value);
                break;
        }

        return res;
    }

};

#endif