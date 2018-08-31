#ifndef PASSIVE_HPP
#define PASSIVE_HPP

#include <set>

#include "Modification.hpp"

#include "Enums.hpp"

class Modification; //DELETE?

class Passive
{
public:

    Passive();
    Passive(std::string name, int turns, std::set<TarjetTeam> triggerTurns, std::vector<Modification> modifications);
    ~Passive();

    std::string _name;
    int _turns;
    std::set<TarjetTeam> _triggerTurns;
    //bool _stackable;??


    std::vector<Modification> _modifications;
};
    
#endif