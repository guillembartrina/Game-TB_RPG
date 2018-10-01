#include "Database.hpp"

Database::Database(Resources& resources) : resources(resources)
{
    _weaponsLoaded = false;
    _unitsLoaded = false;
    _mapsLoaded = false;
}

Database::~Database() {}

void Database::load()
{
    loadPassives();
    loadWeapons();
    loadUnits();
    loadMaps();
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

    if(size == 0) throw Error("No weapons found on database");

    for(int i = 0; i < size; ++i)
    {
        _weapons[i]._name = data["Weapons"][i]["name"].as_string(); //WARN: same names
        _weapons[i]._type = WeaponType(data["Weapons"][i]["type"].as_int());

        jute::jValue tmpValue;
        int tmpSize;

        tmpValue = data["Weapons"][i]["range"]["range"];
        tmpSize = tmpValue.size();

        for(int j = 0; j < tmpSize; ++j)
        {
            _weapons[i]._range.insert(_weapons[i]._range.end(), tmpValue[j].as_int());
        }

        if(data["Weapons"][i]["range"]["specialRange"].as_bool())
        {
            tmpValue = data["Weapons"][i]["range"]["specialRangeCoords"];
            tmpSize = tmpValue.size();

            for(int j = 0; j < tmpSize; ++j)
            {
                _weapons[i]._specialRange.insert(_weapons[i]._specialRange.end(), Coord(tmpValue[j][0].as_int(), tmpValue[j][1].as_int()));
            }
        }

        _weapons[i]._tarjetsEnemy = false;

        if(data["Weapons"][i]["enemy"]["tarjetsEnemy"].as_bool())
        {
            _weapons[i]._tarjetsEnemy = true;

            tmpValue = data["Weapons"][i]["enemy"];

            _weapons[i]._enemy = std::vector<Effect>(tmpValue["effects"].size() + tmpValue["otherEffects"].size());

            tmpSize = tmpValue["effects"].size();

            for(int j = 0; j < tmpSize; ++j)
            {
                _weapons[i]._enemy[j] = getEffect(PredefinedEffect(tmpValue["effects"][j][0].as_int()), tmpValue["effects"][j][1].as_int());
            }

            int offset = tmpSize;
            tmpSize = tmpValue["otherEffects"].size();

            for(int j = 0; j < tmpSize; ++j)
            { 
                readEffect(tmpValue["otherEffects"][j], _weapons[i]._enemy[j + offset]);
            }
        }

        _weapons[i]._tarjetsAlly = false;

        if(data["Weapons"][i]["ally"]["tarjetsAlly"].as_bool())
        {
            _weapons[i]._tarjetsAlly = true;

            tmpValue = data["Weapons"][i]["ally"];
            tmpSize = tmpValue["effects"].size();

            _weapons[i]._ally = std::vector<Effect>(tmpSize);

            for(int j = 0; j < tmpSize; ++j)
            {
                _weapons[i]._ally[j] = getEffect(PredefinedEffect(tmpValue["effects"][j][0].as_int()), tmpValue["effects"][j][1].as_int());
            }

            int offset = tmpSize;
            tmpSize = tmpValue["otherEffects"].size();

            for(int j = 0; j < tmpSize; ++j)
            {
                readEffect(tmpValue["otherEffects"][j], _weapons[i]._ally[j + offset]);
            }

        }

    }
    _weaponsLoaded = true;
}

void Database::loadUnits()
{
    if(_unitsLoaded) return;

    ifstream file("rsc/units.json");
    string page = "";
    string tmp;

    while(getline(file, tmp)) page += tmp;

    jute::jValue data = jute::parser::parse(page);

    int size = data["Units"].size();

    _units = std::vector<UnitData>(size);

    if(size == 0) throw Error("No units found on database");

    for(int i = 0; i < size; ++i)
    {
        _units[i]._name = data["Units"][i]["name"].as_string(); //WARN: same names
        
        jute::jValue tmpValue;
        int tmpSize;

        tmpValue = data["Units"][i]["weapon"]["weaponCompatibility"];
        tmpSize = tmpValue.size();

        for(int j = 0; j < tmpSize; ++j)
        {
            _units[i]._weaponCompatibility.insert(_units[i]._weaponCompatibility.end(), WeaponType(tmpValue[j].as_int()));
        }

        tmpValue = data["Units"][i]["weapon"]["byNameWeaponCompatibility"];
        tmpSize = tmpValue.size();

        for(int j = 0; j < tmpSize; ++j)
        {
            _units[i]._byNameWeaponCompatibility.insert(_units[i]._byNameWeaponCompatibility.end(), tmpValue[j].as_string());
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

        if(!found) throw Error("Weapon <" + weaponName + "> not found");
        
        tmpValue = data["Units"][i]["abilities"];
        tmpSize = tmpValue.size();

        _units[i]._abilities = std::vector<Ability>(tmpSize);

        for(int j = 0; j < tmpSize; ++j)
        {
            _units[i]._abilities[j]._name = tmpValue[j]["name"].as_string();

            int size = tmpValue[j]["range"].size();

            for(int k = 0; k < size; ++k)  _units[i]._abilities[j]._range.insert(tmpValue[j]["range"][k].as_int());

            size = tmpValue[j]["effects"].size();

            _units[i]._abilities[j]._effects = std::vector<std::pair<Coord, Effect>>(size);

            for(int k = 0; k < size; ++k)
            {
                _units[i]._abilities[j]._effects[k].first = Coord(tmpValue[j]["effects"][k][0][0].as_int(), tmpValue[j]["effects"][k][0][1].as_int());

                readEffect(tmpValue[j]["effects"][k][1], _units[i]._abilities[j]._effects[k].second);
            }
        }

        tmpValue = data["Units"][i]["movement"];
        _units[i]._movementType = MovementType(tmpValue["movementType"].as_int());

        tmpSize = tmpValue["movementRange"].size();

        for(int j = 0; j < tmpSize; ++j)
        {
            _units[i]._movementRange.insert(_units[i]._movementRange.end(), tmpValue["movementRange"][j].as_int());
        }

        tmpSize = tmpValue["specialMovementRange"].size();

        for(int j = 0; j < tmpSize; ++j)
        {
            _units[i]._specialMovementRange.insert(_units[i]._specialMovementRange.end(), Coord(tmpValue["specialMovementRange"][j][0].as_int(), tmpValue["specialMovementRange"][j][1].as_int()));
        }
        
        tmpSize = data["Units"][i]["attributes"].size();

        for(int j = 0; j < tmpSize; ++j)
        {
            _units[i]._attributes[j] = data["Units"][i]["attributes"][j].as_int();
        }

        _units[i]._sprite.addAnimation("idle", resources.Texture(data["Units"][i]["sprite"].as_string()), 4, sf::Vector2u(64, 64), sf::seconds(0.2f), true);
        _units[i]._sprite.addAnimation("attack", resources.Texture(data["Units"][i]["sprite"].as_string()), 4, 1, sf::Vector2u(64, 64), sf::seconds(0.1f), false);
        _units[i]._sprite.setActiveAnimation("idle");
    }

    _unitsLoaded = true;
}

void Database::loadMaps()
{
    if(_mapsLoaded) return;

    ifstream file("rsc/maps.json");
    string page = "";
    string tmp;

    while(getline(file, tmp)) page += tmp;

    jute::jValue data = jute::parser::parse(page);

    int size = data["Maps"].size();

    _maps = std::vector<MapData>(size);

    if(size == 0) throw Error("No maps found on database");

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

void Database::loadPassives()
{
    if(_passivesLoaded) return;

    ifstream file("rsc/passives.json");
    string page = "";
    string tmp;

    while(getline(file, tmp)) page += tmp;

    jute::jValue data = jute::parser::parse(page);

    int size = data["Passives"].size();

    _passives = std::vector<Passive>(size);

    for(int i = 0; i < size; ++i)
    {
        jute::jValue tmpValue = data["Passives"][i];
        _passives[i]._name = tmpValue["name"].as_string();
        _passives[i]._turns = tmpValue["turns"].as_int();

        int tmpSize = tmpValue["triggerTurns"].size();

        for(int j = 0; j < tmpSize; ++j) _passives[i]._triggerTurns.insert(TarjetTeam(tmpValue["triggerTurns"][j].as_int()));

        int modifications = tmpValue["modifications"].size();

        _passives[i]._modifications = std::vector<Modification>(modifications);

        for(int k = 0; k < modifications; ++k)
        {
            readModification(tmpValue["modifications"][k], _passives[i]._modifications[k]);
        }
    }

    _passivesLoaded = true;
}

std::vector<UnitData>& Database::getUnits()
{
    return _units;
}

std::vector<MapData>& Database::getMaps()
{
    return _maps;
}

void Database::printWeapons() const
{
    std::vector<Weapon>::const_iterator it = _weapons.begin();

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
        std::cerr << "Effects on enemy: ";
        /*
        for(int i = 0; i < DamageType::DT_ELEMS; ++i)
        {
            if(it->_enemy[i] != 0) std::cerr << " {" << i << ": " << it->_enemy[i] << "}";
        }
        */
        std::cerr << std::endl;
        std::cerr << "Effects on ally: ";
        /*
        for(int i = 0; i < DamageType::DT_ELEMS; ++i)
        {
            if(it->_ally[i] != 0) std::cerr << " {" << i << ": " << it->_ally[i] << "}";
        }
        */
        std::cerr << std::endl;
        std::cerr << "-------------------------------------" << std::endl;
        ++it;
    }
}

void Database::printUnits() const
{
    std::vector<UnitData>::const_iterator it = _units.begin();

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
            std::list<std::string>::const_iterator it2 = it->_byNameWeaponCompatibility.begin();
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
            std::vector<Coord>::const_iterator it4 = it->_specialMovementRange.begin();
            while(it4 != it->_specialMovementRange.end())
            {
                std::cerr << " {" << it4->x << "," << it4->y << "}";
                ++it4;
            }
        }
        std::cerr << std::endl;
        std::cerr << "Attributes:" << std::endl;
        std::vector<int>::const_iterator it5 = it->_attributes.begin();
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

void Database::printMaps() const
{
    std::vector<MapData>::const_iterator it = _maps.begin();

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

Effect Database::getEffect(PredefinedEffect id, int value)
{
    Effect effect;

    effect._area = {Coord(0, 0)};
    effect._haveSprite = true;
    effect._haveSound = true;

    if(id == PredefinedEffect::HEAL)
    {
        effect._sprite.addAnimation("effect", resources.Texture("heal"), 4, sf::Vector2u(64, 64), sf::seconds(0.1f), false);
        effect._sound = sf::Sound(resources.Sound("heal"));
    }
    else
    {
        effect._sprite.addAnimation("effect", resources.Texture("attack"), 4, sf::Vector2u(64, 64), sf::seconds(0.1f), false);
        effect._sound = sf::Sound(resources.Sound("attack"));
    }

    Modification modification;

    modification = Modification(id, value);

    effect._modifications = {modification};

    return effect;
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

void Database::readModification(jute::jValue source, Modification& modification)
{
    jute::jValue params = source["params"];
    switch(source["type"].as_int())
    {
        case 0:
        {
            if(params.size() != 2) throw Error("Bad modification params");
            modification = Modification(PredefinedEffect(params[0].as_int()), params[1].as_int());
        }
            break;
        case 1:
        {
            if(params.size() != 7) throw Error("Bad modification params");
            int size = params[5].size();
            std::vector<std::pair<UnitAttribute, float>> sum(size);
            for(int i = 0; i < size; ++i) sum[i] = std::make_pair(UnitAttribute(params[5][i][0].as_int()), params[5][i][1].as_double());
            size = params[6].size();
            std::vector<std::pair<UnitAttribute, float>> res(size);
            for(int i = 0; i < size; ++i) res[i] = std::make_pair(UnitAttribute(params[6][i][0].as_int()), params[6][i][1].as_double());
            modification = Modification(UnitAttribute(params[0].as_int()), params[1].as_bool(), params[2].as_int(), params[3].as_bool(), params[4].as_bool(), sum, res);
        }
            break;
        case 2:
            if(params.size() != 3) throw Error("Bad modification params");
            modification = Modification(UnitState(params[0].as_int()), params[1].as_bool(), params[2].as_bool());
            break;
        case 3:
        {
            if(params.size() != 1) throw Error("Bad modification params");

            Passive* add;
            std::string name = params[0].as_string();

            std::vector<Passive>::iterator it = _passives.begin();
            bool found = false;
            while(!found && it != _passives.end())
            {
                if(it->_name == name)
                {
                    add = &*it;
                    found = true;
                }
                ++it;
            }

            if(!found) throw Error("Passive <" + name + "> not found");

            modification = Modification(add);
        }
            break;
        case 4:
        {
            if(params.size() != 2) throw Error("Bad modification params");
            std::string del;
            del = params[1].as_string();
            modification = Modification(params[0].as_bool(), del);
        }
            break;
    }
}

void Database::readEffect(jute::jValue source, Effect& effect)
{
    int size = source["area"].size();
    effect._area = std::vector<Coord>(size);
    for(int k = 0; k < size; ++k) effect._area[k] = Coord(source["area"][k][0].as_int(), source["area"][k][1].as_int());

    size = source["modifications"].size();

    effect._modifications = std::vector<Modification>(size);

    for(int k = 0; k < size; ++k)
    {
        readModification(source["modifications"][k], effect._modifications[k]);
    }

    if(source["sprite"].as_string() != "")
    {
        effect._haveSprite = true;
        effect._sprite.addAnimation("effect", resources.Texture(source["sprite"].as_string()), 4, sf::Vector2u(64, 64), sf::seconds(0.1f), false);
    }

    if(source["sound"].as_string() != "")
    {
        effect._haveSound = true;
        effect._sound = sf::Sound(resources.Sound(source["sound"].as_string()));
    }
}