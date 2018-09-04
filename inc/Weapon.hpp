#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <set>
#include <vector>

#include "Coord.hpp"
#include "Effect.hpp"

#include "Enums.hpp"

class Weapon
{
public:

    Weapon();
    ~Weapon();

    std::string _name;

    WeaponType _type;

    std::set<int> _range;
    std::vector<Coord> _specialRange;

    bool _tarjetsEnemy;
    std::vector<Effect> _enemy;

    bool _tarjetsAlly;
    std::vector<Effect> _ally;
    
    //abilities, bonuses agaist, requeriments

};

#endif