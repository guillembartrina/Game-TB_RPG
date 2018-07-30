
#include "Scene_MapPick.hpp"

Scene_MapPick::Scene_MapPick(SceneHandler& sceneHandler, Resources& resources, Database& database) : Scene(sceneHandler, resources), _database(database) {}

Scene_MapPick::~Scene_MapPick() {}

void Scene_MapPick::init()
{
    t_title.setFont(_resources.Font("font"));
    t_title.setString("PICK A MAP");
    t_title.setCharacterSize(40);
    t_title.setFillColor(sf::Color::Yellow);
    t_title.setPosition(50, 50);

    t_info.setFont(_resources.Font("font1"));
    t_info.setString("Press 'space' to pick, 'arrows' to move up/down, 'N' to next stage, 'E' to exit");
    t_info.setCharacterSize(34);
    t_info.setFillColor(sf::Color::White);
    t_info.setPosition(50, 100);

    _mapList = List(sf::FloatRect(370, 170, 400, 600), 3, 1);

    loadMapsToList();
}

void Scene_MapPick::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::N:
                {
                    if(_mapList.allPicked())
                    {
                        _sceneHandler.addScene(std::unique_ptr<Scene>(new Scene_UnitsPick(_sceneHandler, _resources, _database, _database.getMaps().at(_mapList.getPicked().front()->_id))));
                    }
                }
                    break;
                case sf::Keyboard::E:
                {
                    _sceneHandler.popScene();
                }
                    break;
                case sf::Keyboard::Up:
                {
                    _mapList.up();
                }
                    break;
                case sf::Keyboard::Down:
                {
                    _mapList.down();
                }
                    break;
                case sf::Keyboard::Space:
                {
                    _mapList.pick();
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

void Scene_MapPick::update(const sf::Time deltatime)
{
    _mapList.update(deltatime);
}

void Scene_MapPick::draw(sf::RenderWindow& window) const
{
    window.draw(t_title);
    window.draw(t_info);
    _mapList.draw(window);
}

void Scene_MapPick::pause() {}

void Scene_MapPick::resume() {}

void Scene_MapPick::loadMapsToList()
{
    _database.loadMaps(_resources);

    std::vector<MapData>::iterator it = _database.getMaps().begin();

    while(it != _database.getMaps().end())
    {
        Item tmp;

        sf::Text text;
        text.setFont(_resources.Font("font1"));
        text.setCharacterSize(20);

        text.setString("Name: " + it->_name);
        tmp.addText(sf::Vector2f(6, -6), text);

        tmp.addSprite(sf::Vector2f(115, 20), it->_preview);

        _mapList.add(tmp);

        ++it;
    }

}
