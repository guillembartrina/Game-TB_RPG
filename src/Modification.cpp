#include "Modification.hpp"

Modification::Modification()
{
    _modAttributes = false;
    _modStates = false;
}

Modification::Modification(UnitAttribute tarjet, bool relative, int value, bool invert, bool permanent, std::vector<std::pair<UnitAttribute, float>> sum, std::vector<std::pair<UnitAttribute, float>> res)
{
    _modAttributes = true;
    _modStates = false;

    _aTarjet = tarjet;
    _aRelative = relative;
    _aValue = value;
    _aInvert = invert;
    _permanent = permanent;

    _aSum = sum;
    _aRes = res;
}

Modification::Modification(UnitState tarjet, bool value, bool permanent)
{
    _modAttributes = false;
    _modStates = true;

    _sTarjet =  tarjet;
    _sValue = tarjet;
    _permanent = permanent;
}

Modification::~Modification() {}