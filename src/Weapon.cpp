#include "Weapon.hpp"

Weapon::Weapon()
{
    _enemy = std::vector<int>(DamageType::DT_ELEMS);
    _ally = std::vector<int>(DamageType::DT_ELEMS);
}

Weapon::~Weapon() {}