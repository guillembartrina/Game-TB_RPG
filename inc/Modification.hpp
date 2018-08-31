#ifndef MODIFICATION_HPP
#define MODIFICATION_HPP

#include <vector>

#include "Attr.hpp"
#include "Passive.hpp"

#include "Enums.hpp"

class Passive; //DELETE?

class Modification
{
public:

    Modification();
    Modification(UnitAttribute tarjet, bool relative, int value, bool invert, bool permanent, std::vector<std::pair<UnitAttribute, float>> sum, std::vector<std::pair<UnitAttribute, float>> res);
    Modification(UnitState tarjet, bool value, bool permanent);
    Modification(std::vector<Passive> add);
    Modification(bool delAll, std::vector<std::string> del);
    ~Modification();

    /* ATTRIBUTES */
    bool _modAttributes;

    UnitAttribute _aTarjet;
    bool _aRelative;
    int _aValue;

    bool _aInvert;

    std::vector<std::pair<UnitAttribute, float>> _aSum;
    std::vector<std::pair<UnitAttribute, float>> _aRes;

    /* STATES */
    bool _modStates;

    UnitState _sTarjet;
    bool _sValue;

    /* PASSIVES ADD*/
    bool _modPassivesAdd;

    std::vector<Passive> _pAdd;

    /* PASSIVES DEL*/
    bool _modPassivesDel;

    std::vector<std::string> _pDel;
    bool _pDelAll;


    bool _permanent;
};

#endif