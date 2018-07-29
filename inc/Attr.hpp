#ifndef ATTR_HPP
#define ATTR_HPP

#include <string>
#include <utility>

class AttrBase
{
public:

    ~AttrBase() = default;
    
};

template<typename T>
class Attr : public AttrBase
{
public:

    Attr()
    {
        _value = T();
    }

    Attr(T value)
    {
        _value = value;
    }

    ~Attr() {}

    T _value;
};

#endif