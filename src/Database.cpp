#include "Database.hpp"

Database::Database()
{
    _weaponsLoaded = false;
    _unitsLoaded = false;
    _mapsLoaded = false;
}

Database::~Database() {}

void Database::load(Resources& resources)
{
    loadUnits(resources);
    loadMaps(resources);
}

void Database::loadWeapons()
{
    if(_weaponsLoaded) return;

    ifstream file("rsc/weapons.json");
    string page = "";
    string tmp;
    while(getline(file, tmp)) page += tmp;

    jute::jValue data = jute::parser::parse(page);

    int size = data["Weapons"].size();

    _weapons = std::vector<Weapon>(size);

    for(int i = 0; i < size; ++i)
    {
        _weapons[i]._name = data["Weapons"][i]["name"].as_string(); //WARN: same names
        _weapons[i]._type = WeaponType(data["Weapons"][i]["type"].as_int());

        int tmp;

        tmp = data["Weapons"][i]["range"]["range"].size();

        for(int j = 0; j < tmp; ++j)
        {
            _weapons[i]._range.insert(_weapons[i]._range.end(), data["Weapons"][i]["range"]["range"][j].as_int());
        }

        if(data["Weapons"][i]["range"]["specialRange"].as_bool())
        {
            tmp = data["Weapons"][i]["range"]["specialRangeCoords"].size();

            for(int j = 0; j < tmp; ++j)
            {
                _weapons[i]._specialRange.insert(_weapons[i]._specialRange.end(), Coord(data["Weapons"][i]["range"]["specialRangeCoords"][j][0].as_int(), data["Weapons"][i]["range"]["specialRangeCoords"][j][1].as_int()));
            }
        }

        _weapons[i]._tarjetsEnemy = false;

        if(data["Weapons"][i]["enemy"]["tarjetsEnemy"].as_bool())
        {
            _weapons[i]._tarjetsEnemy = true;

            for(int j = 0; j < DamageType::DT_ELEMS; ++j)
            {
                _weapons[i]._enemy[j] = data["Weapons"][i]["enemy"]["enemy"][j].as_int();
            }
        }

        _weapons[i]._tarjetsAlly = false;

        if(data["Weapons"][i]["ally"]["tarjetsAlly"].as_bool())
        {
            _weapons[i]._tarjetsAlly = true;

            for(int j = 0; j < DamageType::DT_ELEMS; ++j)
            {
                _weapons[i]._ally[j] = data["Weapons"][i]["ally"]["ally"][j].as_int();
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

    int size = data["Units"].size();

    _units = std::vector<UnitData>(size);

    for(int i = 0; i < size; ++i)
    {
        _units[i]._name = data["Units"][i]["name"].as_string(); //WARN: same names
        
        int tmp;

        tmp = data["Units"][i]["weapon"]["weaponCompatibility"].size();

        for(int j = 0; j < tmp; ++j)
        {
            _units[i]._weaponCompatibility.insert(_units[i]._weaponCompatibility.end(), WeaponType(data["Units"][i]["weapon"]["weaponCompatibility"][j].as_int()));
        }

        tmp = data["Units"][i]["weapon"]["byNameWeaponCompatibility"].size();

        for(int j = 0; j < tmp; ++j)
        {
            _units[i]._byNameWeaponCompatibility.insert(_units[i]._byNameWeaponCompatibility.end(), data["Units"][i]["weapon"]["byNameWeaponCompatibility"][j].as_string());
        }

        std::string weaponName = data["Units"][i]["weapon"]["weapon"].as_string();
        bool found = false;
        for(unsigned int j = 0; !found && j < _weapons.size(); ++j)
        {
            if(_weapons[j]._name == weaponName)
            {
                found = true;
                _units[i]._weapon = _weapons[j];
            }
        }

        if(!found) std::cerr << "No weapon <" << weaponName << "> found for: " << _units[i]._name << "." << std::endl;

        _units[i]._movementType = MovementType(data["Units"][i]["movement"]["movementType"].as_int());

        tmp = data["Units"][i]["movement"]["movementRange"].size();

        for(int j = 0; j < tmp; ++j)
        {
            _units[i]._movementRange.insert(_units[i]._movementRange.end(), data["Units"][i]["movement"]["movementRange"][j].as_int());
        }

        tmp = data["Units"][i]["movement"]["specialMovementRange"].size();

        for(int j = 0; j < tmp; ++j)
        {
            _units[i]._specialMovementRange.insert(_units[i]._specialMovementRange.end(), Coord(data["Units"][i]["movement"]["specialMovementRange"][j][0].as_int(), data["Units"][i]["movement"]["specialMovementRange"][j][1].as_int()));
        }
        
        tmp = data["Units"][i]["attributes"].size();

        for(int j = 0; j < tmp; ++j)
        {
            _units[i]._attributes[j] = data["Units"][i]["attributes"][j].as_int();
        }

        _units[i]._sprite.addAnimation("idle", resources.Texture(data["Units"][i]["sprite"].as_string()), 8, sf::Vector2u(64, 64), sf::seconds(0.1f), true);
        _units[i]._sprite.setActiveAnimation("idle");
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

    int size = data["Maps"].size();

    _maps = std::vector<MapData>(size);

    for(int i = 0; i < size; ++i)
    {
        _maps[i]._name = data["Maps"][i]["name"].as_string();

        int h = data["Maps"][i]["map"].size();
        int w = 0;
        if(h > 0) w = data["Maps"][i]["map"][0].size();

        _maps[i]._map = std::vector<std::vector<TerrainType>>(w, std::vector<TerrainType>(h));

        for(int j = 0; j < h; ++j)
        {
            for(int k = 0; k < w; ++k)
            {
                _maps[i]._map[k][j] = TerrainType(data["Maps"][i]["map"][j][k].as_int());
            }
        }

        sf::Image tmpI;

        tmpI.create(w*6, h*6);

        for(int j = 0; j < w; ++j)
        {
            for(int k = 0; k < h; ++k)
            {
                TerrainType tt = _maps[i]._map[j][k];

                sf::Color color = hsv(int(tt+60)*40, 0.9f, 0.9f);

                for(int l = 0; l < 6; ++l)
                {
                    for(int m = 0; m < 6; ++m)
                    {
                        tmpI.setPixel((j*6)+l, (k*6)+m, color);
                    }
                }
            }
        }

        tmpI.saveToFile(resources.getResourcePath() + "textures/" + _maps[i]._name + ".png");

        _maps[i]._preview.setTexture(resources.Texture(_maps[i]._name));
        

        int numTeams =  data["Maps"][i]["teams"].size();

        _maps[i]._teams = std::vector<std::vector<Coord>>(numTeams);

        for(int j = 0; j < numTeams; ++j)
        {
            int teamSize = data["Maps"][i]["teams"][j].size();
            _maps[i]._teams[j] = std::vector<Coord>(teamSize);

            for(int k = 0; k < teamSize; ++k) _maps[i]._teams[j][k] = Coord(data["Maps"][i]["teams"][j][k][0].as_int(), data["Maps"][i]["teams"][j][k][1].as_int());
        }
    }

    _mapsLoaded = true;
}

std::vector<UnitData>& Database::getUnits()
{
    return _units;
}

std::vector<MapData>& Database::getMaps()
{
    return _maps;
}

void Database::printWeapons()
{
    std::vector<Weapon>::iterator it = _weapons.begin();

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
        if(!it->_specialRange.empty())
        {
            std::cerr << " > Special:";
        }
        for(unsigned int i = 0; i < it->_specialRange.size(); ++i)
        {
            std::cerr << " {" << it->_specialRange[i].x << ", " << it->_specialRange[i].y << "}";
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
    std::vector<UnitData>::iterator it = _units.begin();

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
        if(it->_specialMovementRange.size() > 0)
        {
            std::cerr << ", Special:";
            std::vector<Coord>::iterator it4 = it->_specialMovementRange.begin();
            while(it4 != it->_specialMovementRange.end())
            {
                std::cerr << " {" << it4->x << "," << it4->y << "}";
                ++it4;
            }
        }
        std::cerr << std::endl;
        std::cerr << "Attributes:" << std::endl;
        std::vector<int>::iterator it5 = it->_attributes.begin();
        while(it5 != it->_attributes.end())
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
    std::vector<MapData>::iterator it = _maps.begin();

    std::cerr << "MAPS:" << std::endl;

    while(it != _maps.end())
    {
        std::cerr << "-------------------------------------" << std::endl;
        std::cerr << "Name: " << it->_name << std::endl;
        std::cerr << "Map:" << endl;
        
        for(unsigned int i = 0; i < it->_map[0].size(); ++i)
        {
            std::cerr << "   ";
            for(unsigned int j = 0; j < it->_map.size(); ++j)
            {
                std::cerr << " " << it->_map[j][i];
            }
            std::cerr << std::endl;
        }
        std::cerr << "-------------------------------------" << std::endl;
        ++it;
    }
}

sf::Color Database::hsv(int hue, float sat, float val)
{
    hue %= 360;
    while(hue<0) hue += 360;

    if(sat<0.f) sat = 0.f;
    if(sat>1.f) sat = 1.f;

    if(val<0.f) val = 0.f;
    if(val>1.f) val = 1.f;

    int h = hue/60;
    float f = float(hue)/60-h;
    float p = val*(1.f-sat);
    float q = val*(1.f-sat*f);
    float t = val*(1.f-sat*(1-f));

    switch(h)
    {
        default:
        case 0:
        case 6: return sf::Color(val*255, t*255, p*255);
        case 1: return sf::Color(q*255, val*255, p*255);
        case 2: return sf::Color(p*255, val*255, t*255);
        case 3: return sf::Color(p*255, q*255, val*255);
        case 4: return sf::Color(t*255, p*255, val*255);
        case 5: return sf::Color(val*255, p*255, q*255);
    }
}