#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <string>
#include <vector>

#include "SFML/Audio/Sound.hpp"

#include "Coord.hpp"
#include "Cond.hpp"
#include "Modification.hpp"
#include "AnimatedSprite.hpp"

#include "Enums.hpp"

class Effect
{
public:

    Effect();
    ~Effect();

    //std::vector<std::pair<UnitAttribute, Cond<int>>> _aConditions;
    //std::vector<std::pair<UnitAttribute, Cond<bool>>> _sConditions;

    std::vector<Coord> _area;

    std::vector<Modification> _modifications;

    AnimatedSprite _effect;
    sf::Sound _sound;
};

#endif