#ifndef MODIFICATION_HPP
#define MODIFICATION_HPP

#include <vector>

#include "Attr.hpp"

#include "Enums.hpp"

class Modification
{
public:

    Modification();
    Modification(UnitAttribute tarjet, bool relative, int value, bool permanent, std::vector<std::pair<UnitAttribute, float>> pro, std::vector<std::pair<UnitAttribute, float>> cont);
    Modification(UnitState tarjet, bool value, bool permanent);
    ~Modification();

    bool _modAttributes;

    UnitAttribute _aTarjet;
    bool _aRelative;
    int _aValue;

    std::vector<std::pair<UnitAttribute, float>> _aPro;
    std::vector<std::pair<UnitAttribute, float>> _aCont;

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