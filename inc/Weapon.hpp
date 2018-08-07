#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <set>
#include <vector>

#include "Coord.hpp"

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
    std::vector<int> _enemy;
    //Effects

    bool _tarjetsAlly;
    std::vector<int> _ally;
    //Effects

    //std::vector<Effect> _enemyEffects;
    //std::vector<Effect> _allyEffects;
    
    //Effects, abilities, bonuses agaist, requeriments

};

#endif