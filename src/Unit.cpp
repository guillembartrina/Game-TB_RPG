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
    std::map<OtherUnitAttribute, AttrBase*>::iterator it = _base._otherAttributes.begin();
    while(it != _base._otherAttributes.end())
    {
        delete it->second;
        ++it;
    }

    it = _otherAttributes.begin();
    while(it != _otherAttributes.end())
    {
        delete it->second;
        ++it;
    }
}

void Unit::init(const UnitData& unitData, int team, const Coord& position)
{
    _base = unitData;

    _team = team;
    _alive = true;
    _active = false;
    _position = position;
    _pendingUpdate = false;
    _dazed = false;
    _fixed = false;

    update();
}

void Unit::update()
{
    _movementType = _base._movementType;
    _movementRange = _base._movementRange;
    _specialMovementRange = _base._specialMovementRange;
    _attributes = _base._attributes;
    _otherAttributes = _base._otherAttributes;
}