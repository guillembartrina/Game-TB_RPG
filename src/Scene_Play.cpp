
#include "Scene_Play.hpp"

Scene_Play::Scene_Play(SceneHandler &sceneHandler, Resources &resources, MapData *mapData, std::vector<std::vector<UnitData>> *unitsData) : Scene(sceneHandler, resources), _mapData(*mapData), _unitsData(*unitsData) {}

Scene_Play::~Scene_Play()
{
    _resources.Music("ambient").stop();
}

void Scene_Play::init()
{
    t_title.setFont(_resources.Font("font1"));
    t_title.setString("Press 'e' to exit");
    t_title.setCharacterSize(30);
    t_title.setFillColor(sf::Color::White);
    t_title.setPosition(10, 760);

    t_currentTeam.setFont(_resources.Font("font1"));
    t_currentTeam.setCharacterSize(40);
    t_currentTeam.setStyle(sf::Text::Style::Bold);
    t_currentTeam.setFillColor(sf::Color::Yellow);
    t_currentTeam.setPosition(12, 606);

    t_currentRemaining.setFont(_resources.Font("font1"));
    t_currentRemaining.setCharacterSize(30);
    t_currentRemaining.setFillColor(sf::Color::White);
    t_currentRemaining.setPosition(12, 640);

    rs_info.setPosition(105, 620);
    rs_info.setSize(sf::Vector2f(472, 175));
    rs_info.setFillColor(sf::Color(0, 51, 102));
    rs_info.setOutlineThickness(-1.f);
    rs_info.setOutlineColor(sf::Color::Yellow);

    t_dataUnit = std::vector<sf::Text>(DataUnit::DU_ELEMS);

    for(int i = 0; i < DataUnit::DU_ELEMS; ++i)
    {
        t_dataUnit[i].setFont(_resources.Font("font1"));
        t_dataUnit[i].setCharacterSize(32);
        t_dataUnit[i].setString("---");
    }

    t_dataUnit[DataUnit::DU_NAME].setPosition(120, 610);
    t_dataUnit[DataUnit::DU_TEAM].setPosition(120, 626);
    t_dataUnit[DataUnit::DU_HP].setPosition(120, 652);
    t_dataUnit[DataUnit::DU_RESIST_F].setPosition(120, 668);
    t_dataUnit[DataUnit::DU_RESIST_M].setPosition(120, 684);
    t_dataUnit[DataUnit::DU_WEAPON].setPosition(120, 714);
    t_dataUnit[DataUnit::DU_WEAPON_BASIC_E].setPosition(140, 730);
    t_dataUnit[DataUnit::DU_WEAPON_BASIC_A].setPosition(140, 746);
    t_dataUnit[DataUnit::DU_WEAPON_TYPE].setPosition(270, 610);
    t_dataUnit[DataUnit::DU_WEAPON_RANGE].setPosition(270, 626);
    t_dataUnit[DataUnit::DU_WEAPON_SPECIAL_RANGE].setPosition(270, 642);
    t_dataUnit[DataUnit::DU_MOVEMENT].setPosition(250, 666);
    t_dataUnit[DataUnit::DU_MOVEMENT_RANGE].setPosition(270, 682);
    t_dataUnit[DataUnit::DU_MOVEMENT_SPECIAL_RANGE].setPosition(270, 698);
    t_dataUnit[DataUnit::DU_GOD].setPosition(300, 800);
    t_dataUnit[DataUnit::DU_GOD].setString("");

    t_infoAbility.setFont(_resources.Font("font1"));
    t_infoAbility.setString("[A] to ABILITY");
    t_infoAbility.setCharacterSize(30);
    t_infoAbility.setFillColor(sf::Color::Yellow);
    t_infoAbility.setPosition(600, 640);

    t_infoPassives.setFont(_resources.Font("font1"));
    t_infoPassives.setString("[S/D] to surf PASSIVES");
    t_infoPassives.setCharacterSize(30);
    t_infoPassives.setFillColor(sf::Color::Green);
    t_infoPassives.setPosition(600, 660);

    t_infoBack.setFont(_resources.Font("font1"));
    t_infoBack.setString("[B] to go BACK");
    t_infoBack.setCharacterSize(30);
    t_infoBack.setFillColor(sf::Color::Blue);
    t_infoBack.setPosition(600, 680);

    t_infoFinish.setFont(_resources.Font("font1"));
    t_infoFinish.setString("[F] to FINISH");
    t_infoFinish.setCharacterSize(30);
    t_infoFinish.setFillColor(sf::Color::Red);
    t_infoFinish.setPosition(600, 700);

    _passives = List(sf::FloatRect(430, 627, 140, 160), 3);
    _abilities = List(sf::FloatRect(600, 625, 180, 165), 2, 1);

    _map = Map(sf::FloatRect(10, 10, 780, 600));

    _map.setMap(_resources, _mapData);

    _teams = std::vector<std::vector<Unit>>(_unitsData.size());

    for(unsigned int i = 0; i < _unitsData.size(); ++i)
    {
        _teams[i] = std::vector<Unit>(_unitsData[i].size());

        for(unsigned int j = 0; j < _unitsData[i].size(); ++j)
        {
            if(!_map.correctCoord(_mapData._teams[i][j])) throw Error("Bad position coord, unit <" + std::to_string(j) + "> of team <" + std::to_string(i) + ">");

            _teams[i][j].init(_unitsData[i][j], i, _mapData._teams[i][j]);
        }
    }

    _map.setUnits(_teams);

    _resources.Music("ambient").setLoop(true);
    _resources.Music("ambient").setVolume(20.f);
    _resources.Music("ambient").play();

    _selected = false;

    initPhase(0);
}

void Scene_Play::handleEvents(const sf::Event &event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch (event.key.code)
            {
                case sf::Keyboard::E:
                    _sceneHandler.popScene();
                    break;
                case sf::Keyboard::S:
                    _passives.up();
                    break;
                case sf::Keyboard::D:
                    _passives.down();
                    break;
                case sf::Keyboard::Up:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_BEGIN:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        case TurnPhase::TP_CAST:
                        {
                            if (_map.pointer().y > 0)
                            {
                                --_map.pointer().y;
                                if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);
                            }
                        }
                            break;
                        case TurnPhase::TP_ABILITY:
                        {
                            if(_abilities.active())
                            {
                                _abilities.up();
                                _map.eraseSelection();
                                _map.redCells(_currentUnit->_position, {}, _currentUnit->_base._abilities[_abilities.current()._id]._effects);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::Left:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_BEGIN:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        case TurnPhase::TP_CAST:
                        {
                            if (_map.pointer().x > 0)
                            {
                                --_map.pointer().x;
                                if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::Right:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_BEGIN:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        case TurnPhase::TP_CAST:
                        {
                            if (_map.pointer().x < int(_map._WCells) - 1)
                            {
                                ++_map.pointer().x;
                                if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::Down:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_BEGIN:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        case TurnPhase::TP_CAST:
                        {
                            if (_map.pointer().y < int(_map._HCells) - 1)
                            {
                                ++_map.pointer().y;
                                if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);
                            }
                        }
                            break;
                        case TurnPhase::TP_ABILITY:
                        {
                            if(_abilities.active())
                            {
                                _abilities.down();
                                _map.eraseSelection();
                                _map.redCells(_currentUnit->_position, {}, _currentUnit->_base._abilities[_abilities.current()._id]._effects);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::Space:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_BEGIN:
                        {
                            if(!_map.getPointerCell().empty())
                            {
                                if(_map.getPointerCell()._unit->_active && _map.getPointerCell()._unit->_team == _currentTeam)
                                {
                                    bool tarjets = !_map.getPointerCell()._unit->_states[UnitState::FIXED] && _map.selectCell(_map.pointer(), BfsType::BT_MOVEMENT);
                                    if(tarjets)
                                    {
                                        _selected = true;
                                        _currentUnit = _map.getSelectorCell()._unit;
                                        _currentTurnPhase = TurnPhase::TP_SELECTED;
                                    }
                                    else
                                    {
                                        _map.eraseSelection();
                                        _map.selectCell(_map.pointer(), BfsType::BT_ACTION);
                                        _currentTurnPhase = TurnPhase::TP_ACTION;
                                        std::cerr << "INFO: No movement tarjets" << std::endl;
                                    }
                                }
                            }
                        }
                            break;
                        case TurnPhase::TP_SELECTED:
                        {
                            if(_map.getPointerCell()._action == ActionType::AT_MOVE)
                            {
                                _map.eraseSelection();
                                _map.moveUnit(_currentUnit, _map.pointer());
                                _map.selectCell(_map.pointer(), BfsType::BT_ACTION);
                                _currentTurnPhase = TurnPhase::TP_ACTION;
                            }
                        }
                            break;
                        case TurnPhase::TP_ACTION:
                        {
                            if(_map.getPointerCell()._action != ActionType::AT_NONE)
                            {
                                Weapon& weapon = _currentUnit->_base._weapon;
                                
                                if(_map.getPointerCell()._action == ActionType::AT_ENEMY)
                                {
                                    for(unsigned int i = 0; i < weapon._enemy.size(); ++i)
                                    {
                                        effect(_map.pointer(), weapon._enemy[i]);
                                    }

                                    if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);
                                }   
                                else
                                {
                                    for(unsigned int i = 0; i < weapon._ally.size(); ++i)
                                    {
                                        effect(_map.pointer(), weapon._ally[i]);
                                    }

                                    if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);
                                }
                                
                                endTurn();
                            }
                        }
                            break;
                        case TurnPhase::TP_ABILITY:
                        {
                            _abilities.pick();
                            _map.eraseSelection();
                            _currentAbility = &_currentUnit->_base._abilities[_abilities.getPicked().front()->_id];
                            bool tarjets = _map.redCells(_currentUnit->_position, _currentAbility->_range, {});
                            if(tarjets) _currentTurnPhase = TurnPhase::TP_CAST;
                            else
                            {
                                std::cerr << "INFO: No ability tarjets" << std::endl;
                                endTurn();
                            }
                        }
                            break;
                        case TurnPhase::TP_CAST:
                        {
                            if(_map.getPointerCell()._action == ActionType::AT_ENEMY)
                            {
                                ability(_map.pointer(), *_currentAbility);
                                if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);
                                endTurn();
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::B:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_SELECTED:
                        {
                            _map.eraseSelection();
                            _selected = false;
                            _currentTurnPhase = TurnPhase::TP_BEGIN;
                        }
                            break;
                        case TurnPhase::TP_ABILITY:
                        {
                            _map.eraseSelection();
                            _map.selectCell(_currentUnit->_position, BfsType::BT_ACTION);
                            _currentTurnPhase = TurnPhase::TP_ACTION;
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::A:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_ACTION:
                        {
                            _map.eraseSelection();
                            _currentTurnPhase = TurnPhase::TP_ABILITY;
                            if(_currentUnit->_base._abilities.size() != 0) _map.redCells(_currentUnit->_position, {}, _currentUnit->_base._abilities.front()._effects); 
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::F:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_BEGIN:
                        {
                            initPhase((++_currentTeam)%_teams.size());
                            _currentTurnPhase = TurnPhase::TP_BEGIN;
                        }
                            break;
                        case TurnPhase::TP_ACTION:
                        {
                            endTurn();                        
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
}

void Scene_Play::update(const sf::Time deltatime)
{
    _map.update(deltatime);
    _passives.update(deltatime);

    if(_currentTurnPhase == TurnPhase::TP_ABILITY) _abilities.update(deltatime);

    for(unsigned int i = 0; i < _teams[_currentTeam].size(); ++i)
    {
        if(_teams[_currentTeam][i]._active) _teams[_currentTeam][i]._base._sprite.updateAnimation(deltatime);
    }

    t_title.setString(std::to_string(_map.getPointerCell()._checked) + " " + std::to_string(_map.getPointerCell()._distance) + " " + std::to_string(_map.getPointerCell()._action));
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    window.draw(rs_info);
    window.draw(t_title);
    window.draw(t_currentTeam);
    window.draw(t_currentRemaining);


    if(_currentTurnPhase != TurnPhase::TP_SELECTED) window.draw(t_infoFinish);
    if(_currentTurnPhase == TurnPhase::TP_ACTION) window.draw(t_infoAbility);
    window.draw(t_infoPassives);
    if(_currentTurnPhase == TurnPhase::TP_SELECTED) window.draw(t_infoBack);

    if(_currentTurnPhase == TurnPhase::TP_ABILITY) _abilities.draw(window);

    for(const sf::Text& i : t_dataUnit)
    {
        window.draw(i);
    }
    _passives.draw(window);

    _map.draw(window);
}

void Scene_Play::pause() {}

void Scene_Play::resume() {}

void Scene_Play::initPhase(unsigned int team)
{
    _currentTeam = team;
    _remainUnits = 0;
    t_currentTeam.setString("Team " + std::to_string(team));

    for(unsigned int i = 0; i < _teams.size(); ++i)
    {
        for(unsigned int j = 0; j < _teams[i].size(); ++j)
        {
            if(_teams[i][j]._alive)
            {
                _teams[i][j]._active = false;
                _teams[i][j]._base._sprite.setColor(sf::Color::White);
                _teams[i][j]._base._sprite.stopAnimation();

                if(i == team) _teams[i][j].update(TarjetTeam::TT_ALLY);
                else _teams[i][j].update(TarjetTeam::TT_ENEMY);
            }
        }
    }

    for(unsigned int i = 0; i < _teams[team].size(); ++i)
    {
        if(_teams[team][i]._alive)
        {
            if(!_teams[team][i]._states[UnitState::DAZED])
            {
                _teams[team][i]._active = true;
                _teams[team][i]._base._sprite.setColor(sf::Color(153, 255, 153));
                ++_remainUnits;
                _teams[team][i]._base._sprite.setActiveAnimation("idle");
                _teams[team][i]._base._sprite.playAnimation();
            }
            else
            {
                _teams[team][i]._active = false;
                _teams[team][i]._base._sprite.setColor(sf::Color(255, 255, 153));
            }
        }
    }

    if(!_map.getPointerCell().empty()) setDataUnit(*_map.getPointerCell()._unit);

    t_currentRemaining.setString("Remain: " + std::to_string(_remainUnits));

    _currentUnit = nullptr;
    _currentTurnPhase = TurnPhase::TP_BEGIN;
}

void Scene_Play::endTurn()
{
    _selected = false;
    _currentUnit->_active = false;
    _currentUnit->_base._sprite.setColor(sf::Color(102, 102, 102));
    _map.eraseSelection();
    --_remainUnits;
    t_currentRemaining.setString("Remain: " + std::to_string(_remainUnits));
    if(_remainUnits == 0) initPhase((++_currentTeam)%_teams.size());
    _currentTurnPhase = TurnPhase::TP_BEGIN;   
}

void Scene_Play::setDataUnit(const Unit& unit)
{
    t_dataUnit[DataUnit::DU_NAME].setString("NAME: " + unit._base._name);
    t_dataUnit[DataUnit::DU_TEAM].setString("TEAM: " + std::to_string(unit._team));
    t_dataUnit[DataUnit::DU_HP].setString("HP: " + std::to_string(unit._attributes[UnitAttribute::UA_HP]));
    t_dataUnit[DataUnit::DU_RESIST_F].setString("RES(F): " + std::to_string(unit._attributes[UnitAttribute::UA_RESIST_F]));
    t_dataUnit[DataUnit::DU_RESIST_M].setString("RES(M): " + std::to_string(unit._attributes[UnitAttribute::UA_RESIST_M]));
    t_dataUnit[DataUnit::DU_WEAPON].setString("WEAPON: " + unit._base._weapon._name);
    t_dataUnit[DataUnit::DU_WEAPON_BASIC_A].setString("Ally: error");
    t_dataUnit[DataUnit::DU_WEAPON_TYPE].setString("Type: " + WT_Strings[unit._base._weapon._type]);

    std::string tmp = "";
    for(unsigned int i = 0; i < unit._base._weapon._enemy.size(); ++i)
    {
        tmp += getDataEffect(unit._base._weapon._enemy[i]);
    }

    t_dataUnit[DataUnit::DU_WEAPON_BASIC_E].setString("Enemy: " + tmp);

    tmp = "";
    for(unsigned int i = 0; i < unit._base._weapon._ally.size(); ++i)
    {
        tmp += getDataEffect(unit._base._weapon._ally[i]);
    }

    t_dataUnit[DataUnit::DU_WEAPON_BASIC_A].setString("Ally: " + tmp);

    tmp = "";
    std::set<int>::const_iterator it = unit._base._weapon._range.begin();
    while(it != unit._base._weapon._range.end())
    {
        tmp += " ";
        tmp += std::to_string(*it);

        ++it;
    }

    t_dataUnit[DataUnit::DU_WEAPON_RANGE].setString("Range:" + tmp);

    tmp = "";
    std::vector<Coord>::const_iterator it1 = unit._base._weapon._specialRange.begin();
    while(it1 != unit._base._weapon._specialRange.end())
    {
        tmp += " ";
        tmp += "(" + std::to_string(it1->x) + "," + std::to_string(it1->y) + ")";
        ++it1;
    }

    t_dataUnit[DataUnit::DU_WEAPON_SPECIAL_RANGE].setString("S.Range:" + tmp);

    t_dataUnit[DataUnit::DU_MOVEMENT].setString("MOVEMENT: " + MT_Strings[unit._movementType]);

    tmp = "";
    it = unit._movementRange.begin();
    while(it != unit._movementRange.end())
    {
        tmp += " ";
        tmp += std::to_string(*it);

        ++it;
    }

    t_dataUnit[DataUnit::DU_MOVEMENT_RANGE].setString("Range: " + tmp);

    tmp = "";
    it1 = unit._specialMovementRange.begin();
    while(it1 != unit._specialMovementRange.end())
    {
        tmp += " ";
        tmp += "(" + std::to_string(it1->x) + "," + std::to_string(it1->y) + ")";
        ++it1;
    }

    t_dataUnit[DataUnit::DU_MOVEMENT_SPECIAL_RANGE].setString("S.Range:" + tmp);
    
    tmp = "";
    if(unit._states[UnitState::GOD]) tmp = "#GOD#";
    t_dataUnit[DataUnit::DU_GOD].setString(tmp);


    _passives.clear();

    for(unsigned int i = 0; i < unit._states.size(); ++i)
    {
        if(unit._states[i])
        {
            Item tmp;

            sf::Text text;
            text.setFont(_resources.Font("font1"));
            text.setCharacterSize(50);

            text.setString(US_Strings[i]);
            tmp.addText(sf::Vector2f(14, 10), text);

            _passives.add(tmp);
        }
    }

    std::list<Passive>::const_iterator it2 = unit._passives.begin();
    while(it2 != unit._passives.end())
    {
        Item tmp;

        sf::Text text;
        text.setFont(_resources.Font("font1"));
        text.setCharacterSize(40);

        std::string str = " [";
        std::set<TarjetTeam>::iterator it3 = it2->_triggerTurns.begin();
        while(it3 != it2->_triggerTurns.end())
        {
            str += " ";
            str += TT_Strings[*it3];
            ++it3;
        }
        str += " ]";

        text.setString(it2->_name + " " + str);
        text.setStyle(sf::Text::Style::Bold);
        tmp.addText(sf::Vector2f(14, -10), text);

        for(unsigned int i = 0; i < it2->_modifications.size(); ++i)
        {
            text.setString(getDataModification(it2->_modifications[i]));
            text.setStyle(0);
            tmp.addText(sf::Vector2f(14, 20 + 10*i), text);
        }

        _passives.add(tmp);

        ++it2;
    }

    _abilities.clear();

    for(unsigned int i = 0; i < unit._base._abilities.size(); ++i)
    {
        Item tmp;

        sf::Text text;
        text.setFont(_resources.Font("font1"));
        text.setCharacterSize(40);

        text.setString(unit._base._abilities[i]._name);

        tmp.addText(sf::Vector2f(10, -6), text);

        _abilities.add(tmp);
    }
}

std::string Scene_Play::getDataEffect(const Effect& effect)
{
    std::string out = "";
    for(unsigned int i = 0; i < effect._modifications.size(); ++i)
    {
        out += "[";
        out += getDataModification(effect._modifications[i]);
        out += "]";
    }

    return out;
}

std::string Scene_Play::getDataModification(const Modification& modification)
{
    std::string out = "";

    if(modification._modBasic)
    {
        out = PE_Strings[modification._bType] + " " + std::to_string(modification._bValue);
    }
    else if(modification._modAttributes)
    {
        out = "A: ";
        out += UA_Strings[modification._aTarjet] + " " + std::to_string(modification._aValue);
        out += '\n';
        
        for(unsigned int j = 0; j < modification._aSum.size(); ++j) out +=  "+" + UA_Strings[modification._aSum[j].first];
        for(unsigned int j = 0; j < modification._aRes.size(); ++j) out +=  "-" + UA_Strings[modification._aRes[j].first];
    }
    else if(modification._modStates)
    {
        out = "S: ";
        out +=  US_Strings[modification._sTarjet] + " " + std::to_string(modification._sValue);
    }
    else if(modification._modPassivesAdd)
    {
        out = "+P: ";
        out += modification._pAdd->_name;
    }
    else if(modification._modPassivesDel)
    {
        out = "-P: ";
        if(modification._pDelAll) out += "ALL";
        else out += modification._pDel;
    }
    else
    {
        out = "NOP";
    }

    return out;
}

void  Scene_Play::effect(const Coord& tarjet, Effect& effect)
{
    for(unsigned int i = 0; i < effect._area.size(); ++i)
    {
        Coord coord = tarjet + effect._area[i];

        if(_map.correctCoord(coord))
        {
            if(!_map.getCell(coord).empty())
            {
                _map.getCell(coord)._unit->applyModifications(effect._modifications);
            }
            else
            {
                std::cerr << "INFO: No units on tarjet effect coord <" << coord.x << "," << coord.y << ">" << std::endl;
            }
        }
    }

    _map.effect(tarjet, effect);
}

void Scene_Play::ability(const Coord& tarjet, Ability& ability)
{
    for(unsigned int i = 0; i < ability._effects.size(); ++i)
    {
        Coord coord = tarjet + ability._effects[i].first;

        effect(coord, ability._effects[i].second);
    }
}

void Scene_Play::kill(Unit* unit)
{
    unit->_alive = false;
    unit->_active = false;
    _map.getCell(unit->_position)._unit = nullptr;
}

