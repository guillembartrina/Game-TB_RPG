#include "Modification.hpp"

Modification::Modification()
{
    _modAttributes = false;
    _modStates = false;
    _modPassivesAdd = false;
    _modPassivesDel = false;
}

Modification::Modification(UnitAttribute tarjet, bool relative, int value, bool invert, bool permanent, std::vector<std::pair<UnitAttribute, float>> sum, std::vector<std::pair<UnitAttribute, float>> res)
{
    _modAttributes = true;
    _modStates = false;
    _modPassivesAdd = false;
    _modPassivesDel = false;

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
    _modPassivesAdd = false;
    _modPassivesDel = false;

    _sTarjet =  tarjet;
    _sValue = tarjet;
    _permanent = permanent;
}

Modification::Modification(Passive* add)
{
    _modAttributes = false;
    _modStates = false;
    _modPassivesAdd = true;
    _modPassivesDel = false;

    _pAdd = add;
}

Modification::Modification(bool delAll, std::string del)
{
    _modAttributes = false;
    _modStates = false;
    _modPassivesAdd = false;
    _modPassivesDel = true;

    _pDelAll = delAll;
    _pDel = del;
}

Modification::~Modification() {}