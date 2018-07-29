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
        _baseAttributes = std::vector<int>(UnitAttribute::UA_ELEMS);
    }

    std::string _name;

    std::set<WeaponType> _weaponCompatibility;
    std::list<std::string> _byNameWeaponCompatibility;
    Weapon _weapon;

    MovementType _movementType;
    std::set<int> _movementRange;
    bool _specialMovement;
    std::vector<Coord> _specialMovementCoords;

    std::vector<int> _baseAttributes;
    std::list<AttrBase*> _otherBaseAttributes;

    AnimatedSprite _sprite;
};

class Unit
{
public:

    Unit();
    ~Unit();

    std::string _name;

    std::set<WeaponType> _weaponCompatibility;
    std::vector<std::string> _byNameWeaponCompatibility;
    Weapon _weapon;

    MovementType _movementType;
    std::set<int> _movementRange;
    bool _specialMovement;
    std::vector<Coord> _specialMovementCoords;

    std::vector<int> _baseAttributes;
    std::list<AttrBase*> _otherBaseAttributes;

    AnimatedSprite _sprite;

    /* IN GAME */

    unsigned int _team;

    bool _alive;

    std::vector<int> _attributes;
    std::list<AttrBase*> _otherAttributes;
    bool _pendingUpdate;

    bool _dazed;
    bool _fixed;

    //Effects, abilities, states

};

#endif