
#include "Scene_Play.hpp"

Scene_Play::Scene_Play(SceneHandler& sceneHandler, Resources& resources, const MapData& mapData, const std::pair<std::list<UnitData>, std::list<UnitData>>& unitsData) : Scene(sceneHandler, resources), _mapData(mapData), _unitsData(unitsData) {}

Scene_Play::~Scene_Play() {}

void Scene_Play::init()
{
    t_title.setFont(_resources.Font("font1"));
    t_title.setString("Press 'e' to exit");
    t_title.setCharacterSize(20);
    t_title.setFillColor(sf::Color::White);
    t_title.setPosition(50, 700);

    _mapSize = sf::Vector2u(_mapData._map.size(), _mapData._map[0].size());
    _map = Map(sf::FloatRect(10, 10, 780, 600));
    _map.setMap(_resources, _mapData);

    _team1 = std::vector<Unit>(_unitsData.first.size());
    _team2 = std::vector<Unit>(_unitsData.second.size());

    int i = 0;

    std::list<UnitData>::iterator it1 = _unitsData.first.begin();
    while(it1 != _unitsData.first.end())
    {
        _team1[i].init(*it1);
        ++i;
        ++it1;
    }

    i = 0;

    std::list<UnitData>::iterator it2 = _unitsData.second.begin();
    while(it2 != _unitsData.second.end())
    {
        _team2[i].init(*it2);
        ++i;
        ++it2;
    }   

    _pointer = Coord(0, 0);
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
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    window.draw(t_title);

    _map.draw(window);
}

void Scene_Play::pause() {}

void Scene_Play::resume() {}
