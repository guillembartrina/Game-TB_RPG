#ifndef UNIT_HPP
#define UNIT_HPP

#include <set>
#include <vector>
#include <list>
#include <string>

#include "Coord.hpp"
#include "Attr.hpp"
#include "Weapon.hpp"
#include "AnimatedSprite.hpp"

#include "Enums.hpp"

struct UnitData
{
    UnitData()
    {
        _attributes = std::vector<int>(UnitAttribute::UA_ELEMS);
    }

    std::string _name;

    std::set<WeaponType> _weaponCompatibility;
    std::list<std::string> _byNameWeaponCompatibility;
    Weapon _weapon;

    MovementType _movementType;
    std::set<int> _movementRange;
    std::vector<Coord> _specialMovementRange;

    std::vector<int> _attributes;
    std::map<OtherUnitAttribute, AttrBase*> _otherAttributes;

    AnimatedSprite _sprite;
};

class Unit
{
public:

    Unit();
    ~Unit();

    void init(const UnitData& unitData, int team, const Coord& position);
    void update();

    unsigned int _team;

    bool _alive;
    bool _active;

    Coord _position;

    bool _dazed;
    bool _fixed;

    MovementType _movementType;
    std::set<int> _movementRange;
    std::vector<Coord> _specialMovementRange;

    std::vector<int> _attributes; //U
    std::map<OtherUnitAttribute, AttrBase*> _otherAttributes; //U
    bool _pendingUpdate;

    UnitData _base;

    //Effects, abilities, states

};

#endif