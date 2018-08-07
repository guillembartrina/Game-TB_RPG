
#include "Scene_Play.hpp"

Scene_Play::Scene_Play(SceneHandler &sceneHandler, Resources &resources, MapData *mapData, std::vector<std::list<UnitData>> *unitsData) : Scene(sceneHandler, resources), _mapData(*mapData), _unitsData(*unitsData) {}

Scene_Play::~Scene_Play() {}

void Scene_Play::init()
{
    t_title.setFont(_resources.Font("font1"));
    t_title.setString("Press 'e' to exit");
    t_title.setCharacterSize(20);
    t_title.setFillColor(sf::Color::White);
    t_title.setPosition(50, 700);

    rs_info.setPosition(5, 620);
    rs_info.setSize(sf::Vector2f(180, 175));
    rs_info.setFillColor(sf::Color::Blue);

    t_currentTeam.setFont(_resources.Font("font1"));
    t_currentTeam.setCharacterSize(30);
    t_currentTeam.setFillColor(sf::Color::Yellow);
    t_currentTeam.setPosition(10, 610);

    //_mapSize = sf::Vector2i(_mapData._map.size(), _mapData._map[0].size());
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
            if (_map.correctCoord(_mapData._teams[i][j]))
                _teams[i][j].init(*it, i, _mapData._teams[i][j]);
            else
                std::cerr << "ERROR: Bad <" << it->_name << "> unit position coord" << std::endl;
            ++j;
            ++it;
        }
    }

    _map.setMap(_resources, _mapData, _teams);

    _map._pointer = Coord(0, 0);
    _selected = false;
    _map._selector = Coord(0, 0);

    _currentTeam = 0;
    t_currentTeam.setString("Team " + std::to_string(_currentTeam));
    initPhase(_currentTeam);

    _currentUnit = nullptr;

    _currentTurnPhase = TurnPhase::TP_SELECT;
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
                        case TurnPhase::TP_SELECT:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        {
                            if (_map._pointer.y > 0)
                            {
                                --_map._pointer.y;
                                _map.setPointer(_map._pointer);
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
                        case TurnPhase::TP_SELECT:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        {
                            if (_map._pointer.x > 0)
                            {
                                --_map._pointer.x;
                                _map.setPointer(_map._pointer);
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
                        case TurnPhase::TP_SELECT:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        {
                            if (_map._pointer.x < int(_map._WCells) - 1)
                            {
                                ++_map._pointer.x;
                                _map.setPointer(_map._pointer);
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
                        case TurnPhase::TP_SELECT:
                        case TurnPhase::TP_SELECTED:
                        case TurnPhase::TP_ACTION:
                        {
                            if (_map._pointer.y < int(_map._HCells) - 1)
                            {
                                ++_map._pointer.y;
                                _map.setPointer(_map._pointer);
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
                        case TurnPhase::TP_SELECT:
                        {
                            if (!_map.getCell(_map._pointer).empty() && _map.getCell(_map._pointer)._unit->_team == _currentTeam && _map.getCell(_map._pointer)._unit->_active)
                            {
                                _map.selectCell(_map._pointer);
                                _map._selector = _map._pointer;
                                _selected = true;
                                _currentUnit = _map.getCell(_map._selector)._unit;
                                _currentTurnPhase = TurnPhase::TP_SELECTED;
                            }
                        }
                            break;
                        case TurnPhase::TP_SELECTED:
                        {
                            if (_map.getCell(_map._pointer)._action == ActionType::AT_MOVE)
                            {
                                _map.eraseSelection();
                                _map.moveUnit(_currentUnit, _map._pointer);
                                _currentTurnPhase = TurnPhase::TP_ACTION;
                                _map.selectCell(_map._pointer, false);
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
                            _currentTurnPhase = TurnPhase::TP_SELECT;
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

    t_title.setString(std::to_string(_map.getCell(_map._pointer)._checked) + " " + std::to_string(_map.getCell(_map._pointer)._distance) + " " + std::to_string(_map.getCell(_map._pointer)._action));
}

void Scene_Play::draw(sf::RenderWindow &window) const
{
    window.draw(rs_info);
    window.draw(t_title);
    window.draw(t_currentTeam);

    _map.draw(window);
}

void Scene_Play::pause() {}

void Scene_Play::resume() {}

void Scene_Play::initPhase(unsigned int team)
{
    for(unsigned int i = 0; i < _teams[team].size(); ++i)
    {
        if(_teams[team][i]._alive)
        {
            _teams[team][i]._active = true;
            _teams[team][i]._base._sprite.setColor(sf::Color(153, 255, 153));
        }
    }
}
