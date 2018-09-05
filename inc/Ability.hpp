#ifndef ABILITY_HPP
#define ABILITY_HPP

#include <string>
#include <vector>

#include "Coord.hpp"
#include "Effect.hpp"

#include "Enums.hpp"

class Ability
{
public:

    Ability();
    ~Ability();

    std::string _name;

    std::vector<std::pair<Coord, Effect>> _effects;
};

#endif