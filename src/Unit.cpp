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

void Unit::init(const UnitData& unitData, int team, const Coord& position)
{
    _name = unitData._name;
    _weapon = unitData._weapon;
    _movementType = unitData._movementType;
    _movementRange = unitData._movementRange;
    _specialMovement = unitData._specialMovement;
    _specialMovementCoords = unitData._specialMovementCoords;
    _baseAttributes = unitData._baseAttributes;
    _otherBaseAttributes = unitData._otherBaseAttributes;
    _sprite = unitData._sprite;

    _team = team;
    _alive = true;
    _position = position;
    _pendingUpdate = false;
    _dazed = false;
    _fixed = false;
}