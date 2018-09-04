#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <list>
#include <assert.h>

#include "jute.hpp"

#include "Resources.hpp"
#include "Attr.hpp"
#include "Weapon.hpp"
#include "Unit.hpp"
#include "Map.hpp"
#include "Effect.hpp"

class Database
{
public:

    Database(Resources& resources);
    ~Database();

    void load();

    void loadWeapons();
    void loadUnits();
    void loadMaps();
    void loadPassives();

    std::vector<UnitData>& getUnits();
    std::vector<MapData>& getMaps();

    void printWeapons() const;
    void printUnits() const;
    void printMaps() const;

    Effect getEffect(PredefinedEffect id, int value);

private:

    /* F */
    sf::Color hsv(int hue, float sat, float val);
    void getModification(jute::jValue source, Modification& modification);

    /* DATA */
    Resources& resources;

    std::vector<Weapon> _weapons;
    std::vector<UnitData> _units;
    std::vector<MapData> _maps;
    std::vector<Passive> _passives;

    /* VARS */
    bool _weaponsLoaded;
    bool _unitsLoaded;
    bool _mapsLoaded;
    bool _passivesLoaded;
};

#endif