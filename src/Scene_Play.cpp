
#include "Scene_Play.hpp"

Scene_Play::Scene_Play(SceneHandler& sceneHandler, Resources& resources, MapData* mapData, std::vector<std::list<UnitData>>* unitsData) : Scene(sceneHandler, resources), _mapData(*mapData), _unitsData(*unitsData) {}

Scene_Play::~Scene_Play() {}

void Scene_Play::init()
{
    t_title.setFont(_resources.Font("font1"));
    t_title.setString("Press 'e' to exit");
    t_title.setCharacterSize(20);
    t_title.setFillColor(sf::Color::White);
    t_title.setPosition(50, 700);

    _mapSize = sf::Vector2i(_mapData._map.size(), _mapData._map[0].size());
    _map = Map(sf::FloatRect(10, 10, 780, 600));

    int numTeams = _unitsData.size();

    _teams = std::vector<std::vector<Unit>>(numTeams);

    for(int i = 0; i < numTeams; ++i)
    {
        _teams[i] = std::vector<Unit>(_unitsData[i].size());

        int j = 0;

        std::list<UnitData>::iterator it = _unitsData[i].begin();
        while(it != _unitsData[i].end())
        {
            if(correctCoord(_mapData._teams[i][j])) _teams[i][j].init(*it, i, _mapData._teams[i][j]);
            else std::cerr << "ERROR: Bad <" << it->_name << "> unit position coord" << std::endl;
            ++j;
            ++it;
        }
    }

    _map.setMap(_resources, _mapData, _teams);

    _pointer = Coord(0, 0);
    _selected = false;
    _selector = Coord(0, 0);

    _currentTeam = 0;
}

void Scene_Play::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::E:
                    _sceneHandler.popScene();
                    break;
                case sf::Keyboard::Up:
                {
                    if(_pointer.y > 0)
                    {  
                        --_pointer.y;
                        _map.setPointer(_pointer);
                    }
                }
                    break;
                case sf::Keyboard::Left:
                {
                    if(_pointer.x > 0)
                    {  
                        --_pointer.x;
                        _map.setPointer(_pointer);
                    }
                }
                    break;
                case sf::Keyboard::Right:
                {
                    if(_pointer.x < _mapSize.x-1)
                    {  
                        ++_pointer.x;
                        _map.setPointer(_pointer);
                    }
                }
                    break;
                case sf::Keyboard::Down:
                {
                    if(_pointer.y < _mapSize.y-1)
                    {  
                        ++_pointer.y;
                        _map.setPointer(_pointer);
                    }
                }
                    break;
                case sf::Keyboard::Space:
                {
                    if(!_map.getCell(_pointer).empty() && _map.getCell(_pointer)._unit->_team == _currentTeam)
                    {
                        if(!_selected)
                        {
                            _map.selectCell(_pointer);
                            _selector = _pointer;
                            _selected = true;
                        }
                        else
                        {
                            _map.eraseSelection();
                            _selected = false;
                        }
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

    t_title.setString(std::to_string(_map.getCell(_pointer)._checked) + " " + std::to_string(_map.getCell(_pointer)._distance) + " " + std::to_string(_map.getCell(_pointer)._action));
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    window.draw(t_title);

    _map.draw(window);
}

void Scene_Play::pause() {}

void Scene_Play::resume() {}

bool Scene_Play::correctCoord(const Coord& coord)
{
    if(coord.x >= 0 && coord.x < int(_mapSize.x) && coord.y >= 0 && coord.y < int(_mapSize.y)) return true;

    return false;
}
