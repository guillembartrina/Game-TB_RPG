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

    UnitData();
};

class Unit
{
public:

    Unit();
    ~Unit();

    void init(const UnitData& unitData, int team, const Coord& position);

    void update();

    /* DATA */
    unsigned int _team;
    UnitData _base;

    /* VARS */
    bool _alive;
    bool _active;

    Coord _position;

    std::vector<bool> _states;

    MovementType _movementType;
    std::set<int> _movementRange;
    std::vector<Coord> _specialMovementRange;

    std::vector<int> _attributes; //U
    std::map<OtherUnitAttribute, AttrBase*> _otherAttributes; //U

    //Effects, abilities, states
};

#endif