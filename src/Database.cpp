#include "Database.hpp"

Database::Database()
{
    _weaponsLoaded = false;
    _unitsLoaded = false;
    _mapsLoaded = false;
}

Database::~Database() {}

void Database::loadWeapons()
{
    if(_weaponsLoaded) return;

    ifstream file("rsc/weapons.json");
    string page = "";
    string tmp;
    while(getline(file, tmp)) page += tmp;

    jute::jValue data = jute::parser::parse(page);

    for(int i = 0; i < data["Weapons"].size(); ++i)
    {
        std::list<Weapon>::iterator current = _weapons.insert(_weapons.end(), Weapon());

        current->_name = data["Weapons"][i]["name"].as_string(); //WARN: same names
        current->_type = WeaponType(data["Weapons"][i]["type"].as_int());

        int size;

        size = data["Weapons"][i]["range"]["range"].size();

        for(int j = 0; j < size; ++j)
        {
            current->_range.insert(current->_range.end(), data["Weapons"][i]["range"]["range"][j].as_int());
        }

        current->_specialRange = false;

        if(data["Weapons"][i]["range"]["specialRange"].as_bool())
        {
            current->_specialRange = true;
            size = data["Weapons"][i]["range"]["specialRangeCoords"].size();

            for(int j = 0; j < size; ++j)
            {
                current->_specialRangeCoords.insert(current->_specialRangeCoords.end(), Coord(data["Weapons"][i]["range"]["specialRangeCoords"][j][0].as_int(), data["Weapons"][i]["range"]["specialRangeCoords"][j][1].as_int()));
            }
        }

        current->_tarjetsEnemy = false;

        if(data["Weapons"][i]["enemy"]["tarjetsEnemy"].as_bool())
        {
            current->_tarjetsEnemy = true;

            for(int j = 0; j < DamageType::DT_ELEMS; ++j)
            {
                current->_enemy[j] = data["Weapons"][i]["enemy"]["enemy"][j].as_int();
            }
        }

        current->_tarjetsAlly = false;

        if(data["Weapons"][i]["ally"]["tarjetsAlly"].as_bool())
        {
            current->_tarjetsAlly = true;

            for(int j = 0; j < DamageType::DT_ELEMS; ++j)
            {
                current->_ally[j] = data["Weapons"][i]["ally"]["ally"][j].as_int();
            }
        }
    }

    _weaponsLoaded = true;
}

void Database::loadUnits(Resources& resources)
{
    if(_unitsLoaded) return;

    loadWeapons();

    ifstream file("rsc/units.json");
    string page = "";
    string tmp;

    while(getline(file, tmp)) page += tmp;

    jute::jValue data = jute::parser::parse(page);

    for(int i = 0; i < data["Units"].size(); ++i)
    {
        std::list<UnitData>::iterator current = _units.insert(_units.end(), UnitData());

        current->_name = data["Units"][i]["name"].as_string(); //WARN: same names
        
        int size;

        size = data["Units"][i]["weapon"]["weaponCompatibility"].size();

        for(int j = 0; j < size; ++j)
        {
            current->_weaponCompatibility.insert(current->_weaponCompatibility.end(), WeaponType(data["Units"][i]["weapon"]["weaponCompatibility"][j].as_int()));
        }

        size = data["Units"][i]["weapon"]["byNameWeaponCompatibility"].size();

        for(int j = 0; j < size; ++j)
        {
            current->_byNameWeaponCompatibility.insert(current->_byNameWeaponCompatibility.end(), data["Units"][i]["weapon"]["byNameWeaponCompatibility"][j].as_string());
        }

        std::list<Weapon>::iterator it = _weapons.begin();
        std::string weaponName = data["Units"][i]["weapon"]["weapon"].as_string();
        bool found = false;
        while(!found && it != _weapons.end())
        {
            if(it->_name == weaponName)
            {
                found = true;
                current->_weapon = *it;
            }
        }

        if(!found) std::cerr << "No weapon <" << weaponName << "> found for: " << current->_name << "." << std::endl;

        current->_movementType = MovementType(data["Units"][i]["movement"]["movementType"].as_int());

        size = data["Units"][i]["movement"]["movementRange"].size();

        for(int j = 0; j < size; ++j)
        {
            current->_movementRange.insert(current->_movementRange.end(), data["Units"][i]["movement"]["movementType"][j].as_int());
        }

        if(data["Units"][i]["movement"]["specialMovement"].as_bool())
        {
            size = data["Units"][i]["movement"]["specialMovementCoords"].size();

            for(int j = 0; j < size; ++j)
            {
                current->_specialMovementCoords.insert(current->_specialMovementCoords.end(), Coord(data["Units"][i]["movement"]["specialMovementCoords"][j][0].as_int(), data["Units"][i]["movement"]["specialMovementCoords"][j][1].as_int()));
            }
        }
        
        size = data["Units"][i]["baseAttributes"].size();

        for(int j = 0; j < size; ++j)
        {
            current->_baseAttributes[j] = data["Units"][i]["baseAttributes"][j].as_int();
        }

        current->_sprite.addAnimation("idle", resources.Texture(data["Units"][i]["sprite"].as_string()), 9, sf::Vector2u(64, 64), sf::seconds(0.1f), true);
        current->_sprite.setActiveAnimation("idle");
    }

    _unitsLoaded = true;
}

void Database::loadMaps(Resources& resources)
{
    if(_mapsLoaded) return;

    ifstream file("rsc/maps.json");
    string page = "";
    string tmp;

    while(getline(file, tmp)) page += tmp;

    jute::jValue data = jute::parser::parse(page);

    for(int i = 0; i < data["Maps"].size(); ++i)
    {
        std::list<MapData>::iterator current = _maps.insert(_maps.end(), MapData());

        current->_name = data["Maps"][i]["name"].as_string();

        int h = data["Maps"][i]["map"].size();
        int w = 0;
        if(h > 0) w = data["Maps"][i]["map"][0].size();

        current->_map = std::vector<std::vector<TerrainType>>(w, std::vector<TerrainType>(h));

        for(int j = 0; j < h; ++j)
        {
            for(int k = 0; k < w; ++k)
            {
                current->_map[k][j] = TerrainType(data["Maps"][i]["map"][j][k].as_int());
            }
        }

        sf::Image tmpI;

        tmpI.create(w*6, h*6);

        for(int j = 0; j < w; ++j)
        {
            for(int k = 0; k < h; ++k)
            {
                TerrainType tt = current->_map[j][k];

                for(int l = 0; l < 6; ++l)
                {
                    for(int m = 0; m < 6; ++m)
                    {
                        tmpI.setPixel((j*6)+l, (k*6)+m, sf::Color((tt%3)*100, (tt%6)*50, (tt%26)*10));
                    }
                }
            }
        }

        tmpI.saveToFile(resources.getResourcePath() + "textures/" + current->_name + ".png");

        current->_preview.setTexture(resources.Texture(current->_name));

    }

    _mapsLoaded = true;
}

std::list<UnitData>& Database::getUnits()
{
    return _units;
}

std::list<MapData>& Database::getMaps()
{
    return _maps;
}

void Database::printWeapons()
{
    std::list<Weapon>::iterator it = _weapons.begin();

    std::cerr << "WEAPONS:" << std::endl;

    while(it != _weapons.end())
    {
        std::cerr << "-------------------------------------" << std::endl;
        std::cerr << "Name: " << it->_name << std::endl;
        std::cerr << "Type: " << it->_type << std::endl;
        std::cerr << "Range:";
        std::set<int>::iterator it1 = it->_range.begin();
        while(it1 != it->_range.end())
        {
            std::cerr << " " << *it1;
            ++it1;
        }
        if(it->_specialRange)
        {
            std::cerr << " > Special:";
        }
        for(unsigned int i = 0; i < it->_specialRangeCoords.size(); ++i)
        {
            std::cerr << " {" << it->_specialRangeCoords[i].x << ", " << it->_specialRangeCoords[i].y << "}";
        }
        std::cerr << std::endl;
        std::cerr << "Tarjets:";
        if(it->_tarjetsEnemy) std::cerr << " Enemy";
        if(it->_tarjetsAlly) std::cerr << " Ally";
        std::cerr << std::endl;
        std::cerr << "Effects on enemy:";
        for(int i = 0; i < DamageType::DT_ELEMS; ++i)
        {
            if(it->_enemy[i] != 0) std::cerr << " {" << i << ": " << it->_enemy[i] << "}";
        }
        std::cerr << std::endl;
        std::cerr << "Effects on ally:";
        for(int i = 0; i < DamageType::DT_ELEMS; ++i)
        {
            if(it->_ally[i] != 0) std::cerr << " {" << i << ": " << it->_ally[i] << "}";
        }
        std::cerr << std::endl;
        std::cerr << "-------------------------------------" << std::endl;
        ++it;
    }
}

void Database::printUnits()
{
    std::list<UnitData>::iterator it = _units.begin();

    std::cerr << "UNITS:" << std::endl;

    while(it != _units.end())
    {
        std::cerr << "-------------------------------------" << std::endl;
        std::cerr << "Name: " << it->_name << std::endl;
        std::cerr << "Weapon compatibility:";
        std::set<WeaponType>::iterator it1 = it->_weaponCompatibility.begin();
        while(it1 != it->_weaponCompatibility.end())
        {
            std::cerr << " " << *it1;
            ++it1;
        }
        if(it->_byNameWeaponCompatibility.size() > 0)
        {
            std::cerr << ", Special:";
            std::list<std::string>::iterator it2 = it->_byNameWeaponCompatibility.begin();
            while(it2 != it->_byNameWeaponCompatibility.end())
            {
                std::cerr << " " << *it2;
                ++it2;
            }
        }
        std::cerr << std::endl;
        std::cerr << "Weapon: " << it->_weapon._name << std::endl;
        std::cerr << "Movement type: " << it->_movementType << std::endl;
        std::set<int>::iterator it3 = it->_movementRange.begin();
        while(it3 != it->_movementRange.end())
        {
            std::cerr << " " << *it3;
            ++it3;
        }
        if(it->_specialMovement)
        {
            std::cerr << ", Special:";
            std::vector<Coord>::iterator it4 = it->_specialMovementCoords.begin();
            while(it4 != it->_specialMovementCoords.end())
            {
                std::cerr << " {" << it4->x << "," << it4->y << "}";
                ++it4;
            }
        }
        std::cerr << std::endl;
        std::cerr << "Attributes:" << std::endl;
        std::vector<int>::iterator it5 = it->_baseAttributes.begin();
        while(it5 != it->_baseAttributes.end())
        {
            std::cerr << "    " << *it5 << std::endl;;
            ++it5;
        }
        std::cerr << "Sprite:" << "not programmed" << std::endl;
        std::cerr << "-------------------------------------" << std::endl;
        ++it;
    }
}

void Database::printMaps()
{
    std::list<MapData>::iterator it = _maps.begin();

    std::cerr << "MAPS:" << std::endl;

    while(it != _maps.end())
    {
        std::cerr << "-------------------------------------" << std::endl;
        std::cerr << "Name: " << it->_name << std::endl;
        std::cerr << "Map:" << endl;
        std::vector<std::vector<TerrainType>>::iterator it1 = it->_map.begin();
        while(it1 != it->_map.end())
        {
            std::cerr << "   ";
            std::vector<TerrainType>::iterator it2 = it1->begin();
            while(it2 != it1->end())
            {
                std::cerr << " " << *it2;
                ++it2;
            }
            ++it1;
            std::cerr << std::endl;
        }
        std::cerr << "-------------------------------------" << std::endl;
        ++it;
    }
}