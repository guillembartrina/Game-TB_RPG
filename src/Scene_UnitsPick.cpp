
#include "Scene_UnitsPick.hpp"

Scene_UnitsPick::Scene_UnitsPick(SceneHandler& sceneHandler, Resources& resources, Database& database, MapData* mapData) : Scene(sceneHandler, resources), _database(database), _mapData(mapData) {}

Scene_UnitsPick::~Scene_UnitsPick() {}

void Scene_UnitsPick::init()
{
    t_title.setFont(_resources.Font("font"));
    t_title.setString("PICK THE UNITS");
    t_title.setCharacterSize(40);
    t_title.setFillColor(sf::Color::Yellow);
    t_title.setPosition(50, 50);

    t_info.setFont(_resources.Font("font1"));
    t_info.setString("'Y/H/J' for team 1, 'I/K/L' for team 2, 'N' to next stage, 'E' to exit");
    t_info.setCharacterSize(34);
    t_info.setFillColor(sf::Color::White);
    t_info.setPosition(50, 100);

    int numTeams = _mapData->_teams.size();
    _unitsLists = std::vector<List>(numTeams);

    int listW = 800/numTeams;

    for(int i = 0; i < numTeams; ++i) _unitsLists[i] = List(sf::FloatRect(10 + i*listW, 170, listW-20, 600), 4, _mapData->_teams[i].size());

    _currentTeam = 0;

    loadUnitsToLists();
}

void Scene_UnitsPick::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::N:
                {
                    if(_unitsLists[_currentTeam].allPicked())
                    {
                        if(_currentTeam == _unitsLists.size()-1)
                        {
                            int numTeams = _mapData->_teams.size();
                            std::vector<std::list<UnitData>> teams(numTeams);
                            for(int i = 0; i < numTeams; ++i)
                            {
                                std::list<std::list<Item>::iterator>::iterator it = _unitsLists[i].getPicked().begin();
                                while(it != _unitsLists[i].getPicked().end())
                                {
                                    teams[i].insert(teams[i].end(), _database.getUnits().at((*it)->_id));
                                    ++it;
                                }
                            }

                            _sceneHandler.addScene(std::unique_ptr<Scene>(new Scene_Play(_sceneHandler, _resources, _mapData, &teams)));
                        }
                        else
                        {
                            ++_currentTeam;
                        }
                    }
                }
                    break;
                case sf::Keyboard::Up:
                    _unitsLists[_currentTeam].up();
                    break;
                case sf::Keyboard::Down:
                    _unitsLists[_currentTeam].down();
                    break;
                case sf::Keyboard::Space:
                    _unitsLists[_currentTeam].pick();
                    break;
                
                case sf::Keyboard::E:
                    _sceneHandler.popScene();
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

void Scene_UnitsPick::update(const sf::Time deltatime)
{
    for(unsigned int i = _currentTeam; i < _unitsLists.size(); ++i)
    {
        _unitsLists[i].update(deltatime);
    }
}

void Scene_UnitsPick::draw(sf::RenderWindow& window) const
{
    window.draw(t_title);
    window.draw(t_info);

    for(unsigned int i = 0; i < _unitsLists.size(); ++i)
    {
        _unitsLists[i].draw(window);
    }
}

void Scene_UnitsPick::pause() {}

void Scene_UnitsPick::resume() {}

void Scene_UnitsPick::loadUnitsToLists()
{
    std::vector<UnitData>::iterator it = _database.getUnits().begin();

    while(it != _database.getUnits().end())
    {
        Item tmp;

        sf::Text text;
        text.setFont(_resources.Font("font1"));
        text.setCharacterSize(20);

        text.setString("Name: " + it->_name);
        tmp.addText(sf::Vector2f(6, -6), text);

        text.setString("Weapon: " + it->_weapon._name);
        tmp.addText(sf::Vector2f(6, 4), text);

        tmp.addAnimatedSprite(sf::Vector2f(120, 10), it->_sprite, "idle");

        for(unsigned int i = 0; i < _unitsLists.size(); ++i)
        {
            _unitsLists[i].add(tmp);
        }

        ++it;
    }
}
