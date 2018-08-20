#include "Unit.hpp"

UnitData::UnitData()
{
    _attributes = std::vector<int>(UnitAttribute::UA_ELEMS);
}

Unit::Unit()
{
    _alive = false;
    _active = false;
    _states = std::vector<bool>(UnitState::US_ELEMS, false);
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
    _position = position;

    _movementType = _base._movementType;
    _movementRange = _base._movementRange;
    _specialMovementRange = _base._specialMovementRange;
    _attributes = _base._attributes;
    _otherAttributes = _base._otherAttributes;
}

void Unit::update()
{
    
}