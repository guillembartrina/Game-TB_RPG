#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <list>

#include "jute.hpp"

#include "Resources.hpp"
#include "Attr.hpp"
#include "Weapon.hpp"
#include "Unit.hpp"
#include "Map.hpp"

class Database
{
public:

    Database();
    ~Database();

    void loadUnits(Resources& resources);
    void loadMaps(Resources& resources);

    std::list<UnitData>& getUnits();
    std::list<MapData>& getMaps();

    void printWeapons();
    void printUnits();
    void printMaps();

private:

    bool _weaponsLoaded;
    bool _unitsLoaded;
    bool _mapsLoaded;

    std::list<Weapon> _weapons;
    std::list<UnitData> _units;
    std::list<MapData> _maps;

    void loadWeapons();

};

#endif