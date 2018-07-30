
#include "Scene_UnitsPick.hpp"

Scene_UnitsPick::Scene_UnitsPick(SceneHandler& sceneHandler, Resources& resources, Database& database, MapData mapData) : Scene(sceneHandler, resources), _database(database), _mapData(mapData) {}

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

    _unitsList1 = List(sf::FloatRect(30, 170, 340, 600), 4, 1);
    _unitsList2 = List(sf::FloatRect(430, 170, 340, 600), 4, 1);

    loadUnitsToList();
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
                    if(_unitsList1.allPicked() && _unitsList2.allPicked())
                    {
                        std::list<UnitData> team1;
                        std::list<std::list<Item>::iterator>::iterator it1 = _unitsList1.getPicked().begin();
                        while(it1 != _unitsList1.getPicked().end())
                        {
                            team1.insert(team1.end(), _database.getUnits().at((*it1)->_id));
                            ++it1;
                        }
                        std::list<UnitData> team2;
                        std::list<std::list<Item>::iterator>::iterator it2 = _unitsList2.getPicked().begin();
                        while(it2 != _unitsList2.getPicked().end())
                        {
                            team1.insert(team1.end(), _database.getUnits().at((*it2)->_id));
                            ++it2;
                        }

                        _sceneHandler.addScene(std::unique_ptr<Scene>(new Scene_Play(_sceneHandler, _resources, _mapData, std::make_pair(team1, team2))));
                    }
                }
                    break;
                case sf::Keyboard::E:
                    _sceneHandler.popScene();
                    break;
                case sf::Keyboard::Y:
                    _unitsList1.up();
                    break;
                case sf::Keyboard::H:
                    _unitsList1.down();
                    break;
                case sf::Keyboard::J:
                    _unitsList1.pick();
                    break;
                case sf::Keyboard::I:
                    _unitsList2.up();
                    break;
                case sf::Keyboard::K:
                    _unitsList2.down();
                    break;
                case sf::Keyboard::L:
                    _unitsList2.pick();
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
    _unitsList1.update(deltatime);
    _unitsList2.update(deltatime);
}

void Scene_UnitsPick::draw(sf::RenderWindow& window) const
{
    window.draw(t_title);
    window.draw(t_info);
    _unitsList1.draw(window);
    _unitsList2.draw(window);
}

void Scene_UnitsPick::pause() {}

void Scene_UnitsPick::resume() {}

void Scene_UnitsPick::loadUnitsToList()
{
    _database.loadUnits(_resources);

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

        _unitsList1.add(tmp);
        _unitsList2.add(tmp);

        ++it;
    }
}
