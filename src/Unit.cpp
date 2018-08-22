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

void Unit::applyModification(const Modification& modification)
{
    if(modification._modAttributes)
    {
        int res = modification._aValue;

        for(unsigned int i = 0; i < modification._aPro.size(); ++i) res += _attributes[modification._aPro[i].first] * modification._aPro[i].second;
        for(unsigned int i = 0; i < modification._aCont.size(); ++i) res -= _attributes[modification._aCont[i].first] * modification._aCont[i].second;

        if(modification._aRelative) _attributes[modification._aTarjet] += res;
        else _attributes[modification._aTarjet] = res;
    }

    if(modification._modStates)
    {
        _states[modification._sTarjet] = modification._sValue;
    }
}

void Unit::update()
{
    
}