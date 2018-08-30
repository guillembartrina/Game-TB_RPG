#include "Unit.hpp"

UnitData::UnitData()
{
    _attributes = std::vector<int>(UnitAttribute::UA_ELEMS);
}

Unit::Unit()
{
    _alive = false;
    _active = false;
    _baseStates = std::vector<bool>(UnitState::US_ELEMS, false);
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
    _baseAttributes = _base._attributes;
    _attributes = _base._attributes;
    _otherAttributes = _base._otherAttributes;
}

void Unit::applyModification(const Modification& modification)
{
    if(modification._modAttributes)
    {
        int res = modification._aValue;

        for(unsigned int i = 0; i < modification._aSum.size(); ++i) res += _attributes[modification._aSum[i].first] * modification._aSum[i].second;
        for(unsigned int i = 0; i < modification._aRes.size(); ++i) res -= _attributes[modification._aRes[i].first] * modification._aRes[i].second;

        if(!modification._aInvert)
        {
            if(modification._aValue == 0) res = 0;
            else if(modification._aValue > 0 && res < 0) res = 0;
            else if(modification._aValue < 0 && res > 0) res = 0;
        }

        if(modification._aRelative)
        {
            _attributes[modification._aTarjet] += res;
            if(modification._permanent) _baseAttributes[modification._aTarjet] += res;
        }
        else
        {
            _attributes[modification._aTarjet] = res;
            if(modification._permanent) _baseAttributes[modification._aTarjet] = res;
        }
    }

    if(modification._modStates)
    {
        _states[modification._sTarjet] = modification._sValue;
        if(modification._permanent) _baseStates[modification._sTarjet] = modification._sValue;
    }
}

void Unit::applyModifications(const std::vector<Modification>& modifications)
{
    for(unsigned int i = 0; i < modifications.size(); ++i)
    {
        applyModification(modifications[i]);
    }
}

void Unit::update(TarjetTeam team)
{
    _attributes = _baseAttributes;
    _states = _baseStates;

    std::list<Passive>::iterator it = _passives.begin();
    while(it != _passives.end())
    {
        if(it->_triggerTurns.find(team) != it->_triggerTurns.end())
        {
            std::cerr << "APPLY: " << it->_name << ", turns: " << it->_turns << std::endl;

            for(unsigned int i = 0; i < it->_modifications.size(); ++i) applyModification(it->_modifications[i]);

            if(it->_turns != -1)
            {
                --it->_turns;
                if(it->_turns == 0) it = _passives.erase(it);
            }
        }

        ++it;
    }
}