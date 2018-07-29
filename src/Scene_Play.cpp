
#include "Scene_Play.hpp"

Scene_Play::Scene_Play(SceneHandler& sceneHandler, Resources& resources, MapData mapData) : Scene(sceneHandler, resources), _mapData(mapData) {}

Scene_Play::~Scene_Play() {}

void Scene_Play::init()
{
    t_title.setFont(_resources.Font("font1"));
    t_title.setString("Press 'e' to exit");
    t_title.setCharacterSize(20);
    t_title.setFillColor(sf::Color::White);
    t_title.setPosition(50, 50);

    _map = Map(sf::FloatRect(10, 10, 780, 600));

    _map.setMap(_resources, _mapData);
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
