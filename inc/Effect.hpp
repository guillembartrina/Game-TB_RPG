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
    Effect(const std::vector<Coord>& area, const std::vector<Modification>& modifications, bool haveSprite, bool haveSound);
    ~Effect();

    //std::vector<std::pair<UnitAttribute, Cond<int>>> _aConditions;
    //std::vector<std::pair<UnitAttribute, Cond<bool>>> _sConditions;

    std::vector<Coord> _area;

    std::vector<Modification> _modifications;

    bool _haveSprite;
    AnimatedSprite _sprite;
    bool _haveSound;
    sf::Sound _sound;
};

#endif