#include "Unit.hpp"

Unit::Unit()
{
    _team = 0;
    _alive = false;
    
    _pendingUpdate = false;

    _dazed = false;
    _fixed = false;
}

Unit::~Unit()
{
    std::list<AttrBase*>::iterator it = _otherBaseAttributes.begin();
    while(it != _otherBaseAttributes.end())
    {
        delete *it;
        ++it;
    }

    it = _otherAttributes.begin();
    while(it != _otherAttributes.end())
    {
        delete *it;
        ++it;
    }
}