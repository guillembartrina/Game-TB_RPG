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
#include "Modification.hpp"
#include "Passive.hpp"

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

    void applyModification(const Modification& modification);
    void applyModifications(const std::vector<Modification>& modifications);

    void update(TarjetTeam team);

    /* DATA */
    unsigned int _team;
    UnitData _base;

    std::vector<int> _baseAttributes;
    std::vector<bool> _baseStates;

    /* VARS */
    bool _alive;
    bool _active;

    Coord _position;

    MovementType _movementType;
    std::set<int> _movementRange;
    std::vector<Coord> _specialMovementRange;

    std::vector<int> _attributes; //U
    std::map<OtherUnitAttribute, AttrBase*> _otherAttributes; //U

    std::vector<bool> _states;

    std::list<Passive> _passives;

private:
};

#endif