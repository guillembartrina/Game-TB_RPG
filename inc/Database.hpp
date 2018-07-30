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

    void loadWeapons();
    void loadUnits(Resources& resources);
    void loadMaps(Resources& resources);

    std::vector<UnitData>& getUnits();
    std::vector<MapData>& getMaps();

    void printWeapons();
    void printUnits();
    void printMaps();

private:

    bool _weaponsLoaded;
    bool _unitsLoaded;
    bool _mapsLoaded;

    std::vector<Weapon> _weapons;
    std::vector<UnitData> _units;
    std::vector<MapData> _maps;

};

#endif