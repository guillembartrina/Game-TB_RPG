#include "Effect.hpp"

Effect::Effect()
{
    _haveSprite = false;
    _haveSound = false;
}

Effect::Effect(const std::vector<Coord>& area, const std::vector<Modification>& modifications, bool haveSprite, bool haveSound)
{
    _area = area;
    _modifications = modifications;
    _haveSprite = haveSprite;
    _haveSound = haveSound;
}

Effect::~Effect() {}