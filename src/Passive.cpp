#include "Passive.hpp"

Passive::Passive()
{
    _name = "UNKNOWN";
    _turns = 0;
}

Passive::Passive(std::string name, int turns, std::set<TarjetTeam> triggerTurns, std::vector<Modification> modifications)
{
    _name = name;
    _turns = turns;
    _triggerTurns = triggerTurns;
    _modifications = modifications;
}

Passive::~Passive() {}