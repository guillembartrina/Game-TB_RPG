#ifndef MODIFICATION_HPP
#define MODIFICATION_HPP

#include <vector>

#include "Attr.hpp"

#include "Enums.hpp"

class Modification
{
public:

    Modification();
    Modification(UnitAttribute tarjet, bool relative, int value, bool invert, bool permanent, std::vector<std::pair<UnitAttribute, float>> sum, std::vector<std::pair<UnitAttribute, float>> res);
    Modification(UnitState tarjet, bool value, bool permanent);
    ~Modification();

    bool _modAttributes;

    UnitAttribute _aTarjet;
    bool _aRelative;
    int _aValue;

    bool _aInvert;

    std::vector<std::pair<UnitAttribute, float>> _aSum;
    std::vector<std::pair<UnitAttribute, float>> _aRes;

    bool _modStates;

    UnitState _sTarjet;
    bool _sValue;

    bool _permanent;

/*
    bool _modPassive;

    std::vector<State> _adds;
    bool _removeAll;
    std::vector<ID> _removes;
*/
};

#endif