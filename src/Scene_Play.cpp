
#include "Scene_Play.hpp"

Scene_Play::Scene_Play(SceneHandler &sceneHandler, Resources &resources, MapData *mapData, std::vector<std::list<UnitData>> *unitsData) : Scene(sceneHandler, resources), _mapData(*mapData), _unitsData(*unitsData) {}

Scene_Play::~Scene_Play() {}

void Scene_Play::init()
{
    t_title.setFont(_resources.Font("font1"));
    t_title.setString("Press 'e' to exit");
    t_title.setCharacterSize(30);
    t_title.setFillColor(sf::Color::White);
    t_title.setPosition(650, 700);

    t_currentTeam.setFont(_resources.Font("font1"));
    t_currentTeam.setCharacterSize(40);
    t_currentTeam.setStyle(sf::Text::Style::Bold);
    t_currentTeam.setFillColor(sf::Color::Yellow);
    t_currentTeam.setPosition(12, 606);

    rs_info.setPosition(105, 620);
    rs_info.setSize(sf::Vector2f(340, 175));
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
    t_dataUnit[DataUnit::DU_TEAM].setPosition(270, 610);
    t_dataUnit[DataUnit::DU_HP].setPosition(120, 650);
    t_dataUnit[DataUnit::DU_RESIST_F].setPosition(120, 666);
    t_dataUnit[DataUnit::DU_RESIST_M].setPosition(120, 682);
    t_dataUnit[DataUnit::DU_WEAPON].setPosition(250, 650);
    t_dataUnit[DataUnit::DU_WEAPON_RANGE].setPosition(270, 666);
    t_dataUnit[DataUnit::DU_WEAPON_SPECIAL_RANGE].setPosition(270, 682);
    t_dataUnit[DataUnit::DU_MOVEMENT].setPosition(250, 710);
    t_dataUnit[DataUnit::DU_MOVEMENT_RANGE].setPosition(270, 726);
    t_dataUnit[DataUnit::DU_MOVEMENT_SPECIAL_RANGE].setPosition(270, 742);
    t_dataUnit[DataUnit::DU_GOD].setPosition(300, 610);
    t_dataUnit[DataUnit::DU_GOD].setString("");

    t_infoAbility.setFont(_resources.Font("font1"));
    t_infoAbility.setString("[A] to ABILITY");
    t_infoAbility.setCharacterSize(30);
    t_infoAbility.setFillColor(sf::Color::Yellow);
    t_infoAbility.setPosition(480, 640);

    t_infoFinish.setFont(_resources.Font("font1"));
    t_infoFinish.setString("[F] to FINISH");
    t_infoFinish.setCharacterSize(30);
    t_infoFinish.setFillColor(sf::Color::Red);
    t_infoFinish.setPosition(480, 660);

    _abilities = List(sf::FloatRect(460, 625, 200, 165), 2, 1);

    _map = Map(sf::FloatRect(10, 10, 780, 600));

    int numTeams = _unitsData.size();

    _teams = std::vector<std::vector<Unit>>(numTeams);

    for (int i = 0; i < numTeams; ++i)
    {
        _teams[i] = std::vector<Unit>(_unitsData[i].size());

        int j = 0;

        std::list<UnitData>::iterator it = _unitsData[i].begin();
        while (it != _unitsData[i].end())
        {
            if (_map.correctCoord(_mapData._teams[i][j])) _teams[i][j].init(*it, i, _mapData._teams[i][j]);
            else std::cerr << "ERROR: Bad <" << it->_name << "> unit position coord" << std::endl;
            ++j;
            ++it;
        }
    }

    _map.setMap(_resources, _mapData, _teams);

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
                case sf::Keyboard::Up:
                {
                    switch(_currentTurnPhase)
                    {
                        case TurnPhase::TP_BEGIN:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        {
                            if (_map.pointer().y > 0)
                            {
                                --_map.pointer().y;
                                if(!_map.getCell(_map.pointer()).empty()) setDataUnit(*_map.getCell(_map.pointer())._unit);
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
                        {
                            if (_map.pointer().x > 0)
                            {
                                --_map.pointer().x;
                                if(!_map.getCell(_map.pointer()).empty()) setDataUnit(*_map.getCell(_map.pointer())._unit);
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
                        {
                            if (_map.pointer().x < int(_map._WCells) - 1)
                            {
                                ++_map.pointer().x;
                                if(!_map.getCell(_map.pointer()).empty()) setDataUnit(*_map.getCell(_map.pointer())._unit);
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
                        {
                            if (_map.pointer().y < int(_map._HCells) - 1)
                            {
                                ++_map.pointer().y;
                                if(!_map.getCell(_map.pointer()).empty()) setDataUnit(*_map.getCell(_map.pointer())._unit);
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
                            if(!_map.getCell(_map.pointer()).empty() && _map.getCell(_map.pointer())._unit->_team == _currentTeam && _map.getCell(_map.pointer())._unit->_active)
                            {
                                bool tarjets = !_map.getCell(_map.pointer())._unit->_states[UnitState::FIXED] && _map.selectCell(_map.pointer());
                                if(tarjets)
                                {
                                    _selected = true;
                                    _currentUnit = _map.getCell(_map.selector())._unit;
                                    _currentTurnPhase = TurnPhase::TP_SELECTED;
                                }
                                else
                                {
                                    _map.eraseSelection();
                                    _map.selectCell(_map.pointer(), false);
                                    _currentTurnPhase = TurnPhase::TP_ACTION;
                                    std::cerr << "No movement tarjets" << std::endl;
                                }
                            }
                        }
                            break;
                        case TurnPhase::TP_SELECTED:
                        {
                            if(_map.getCell(_map.pointer())._action == ActionType::AT_MOVE)
                            {
                                _map.eraseSelection();
                                _map.moveUnit(_currentUnit, _map.pointer());
                                _map.selectCell(_map.pointer(), false);
                                _currentTurnPhase = TurnPhase::TP_ACTION;
                            }
                        }
                            break;
                        case TurnPhase::TP_ACTION:
                        {
                            if(_map.getCell(_map.pointer())._action == ActionType::AT_ENEMY)
                            {
                                std::cerr << "Action to ENEMY" << std::endl;
                                _currentUnit->_active = false;
                                _currentUnit->_base._sprite.setColor(sf::Color(102, 102, 102));
                                _map.eraseSelection();
                                _selected = false;
                                --_remainUnits;
                                if(_remainUnits == 0) initPhase((++_currentTeam)%_teams.size());
                                _currentTurnPhase = TurnPhase::TP_BEGIN;
                            }

                            if(_map.getCell(_map.pointer())._action == ActionType::AT_ALLY)
                            {
                                std::cerr << "Action to ALLY" << std::endl;
                                _currentUnit->_active = false;
                                _currentUnit->_base._sprite.setColor(sf::Color(102, 102, 102));
                                _map.eraseSelection();
                                _selected = false;
                                --_remainUnits;
                                if(_remainUnits == 0) initPhase((++_currentTeam)%_teams.size());
                                _currentTurnPhase = TurnPhase::TP_BEGIN;
                            }
                        }
                            break;
                        case TurnPhase::TP_ABILITY:
                        {
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
                            _map.selectCell(_currentUnit->_position, false);
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
                        case TurnPhase::TP_ACTION:
                        {
                            _currentUnit->_active = false;
                            _currentUnit->_base._sprite.setColor(sf::Color(102, 102, 102));
                            _map.eraseSelection();
                            _selected = false;
                            --_remainUnits;
                            if(_remainUnits == 0) initPhase((++_currentTeam)%_teams.size());
                            _currentTurnPhase = TurnPhase::TP_BEGIN;                            
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

    if(_currentTurnPhase == TurnPhase::TP_ABILITY) _abilities.update(deltatime);

    t_title.setString(std::to_string(_map.getCell(_map.pointer())._checked) + " " + std::to_string(_map.getCell(_map.pointer())._distance) + " " + std::to_string(_map.getCell(_map.pointer())._action));
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    window.draw(rs_info);
    window.draw(t_title);
    window.draw(t_currentTeam);
    if(_currentTurnPhase != TurnPhase::TP_SELECTED) window.draw(t_infoFinish);
    if(_currentTurnPhase == TurnPhase::TP_ACTION) window.draw(t_infoAbility);
    if(_currentTurnPhase == TurnPhase::TP_ABILITY) _abilities.draw(window);

    for(const sf::Text& i : t_dataUnit)
    {
        window.draw(i);
    }

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
            _teams[i][j]._active = false;
            _teams[i][j]._base._sprite.setColor(sf::Color::White);
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
            }
            else
            {
                _teams[team][i]._active = false;
                _teams[team][i]._base._sprite.setColor(sf::Color(255, 255, 153));
            }
        }
    }

    _currentUnit = nullptr;
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

    std::string tmp = "";
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

    t_dataUnit[DataUnit::DU_MOVEMENT].setString("MOVEMENT: " + std::to_string(unit._movementType));

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
}
