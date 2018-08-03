
#include "Scene_Menu.hpp"

Scene_Menu::Scene_Menu(SceneHandler& sceneHandler, Resources& resources) : Scene(sceneHandler, resources) {}

Scene_Menu::~Scene_Menu() {}

void Scene_Menu::init()
{
    t_title.setFont(_resources.Font("font"));
    t_title.setString("TB RPG");
    t_title.setCharacterSize(90);
    t_title.setFillColor(sf::Color::Yellow);
    t_title.setPosition(170, 100);

    t_play.setFont(_resources.Font("font"));
    t_play.setString("PLAY");
    t_play.setCharacterSize(60);
    t_play.setFillColor(sf::Color::White);
    t_play.setPosition(250, 400);

    t_exit.setFont(_resources.Font("font"));
    t_exit.setString("EXIT");
    t_exit.setCharacterSize(60);
    t_exit.setFillColor(sf::Color::White);
    t_exit.setPosition(250, 500);

    currentOption = 0;

    _database.load(_resources);
}

void Scene_Menu::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Down:
                {
                    if(currentOption == 0) ++currentOption;
                }
                    break;
                case sf::Keyboard::Up:
                {
                    if(currentOption == 1) --currentOption;
                }
                    break;
                case sf::Keyboard::Return:
                {
                    if(currentOption == 0)
                    {
                        _sceneHandler.addScene(std::unique_ptr<Scene>(new Scene_MapPick(_sceneHandler, _resources, _database)), false);
                    }
                    else if(currentOption == 1)
                    {
                        _sceneHandler.popScene();
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

void Scene_Menu::update(const sf::Time deltatime)
{
    t_play.setFillColor(sf::Color::White);
    t_exit.setFillColor(sf::Color::White);

    if(currentOption == 0)
    {
        t_play.setFillColor(sf::Color::Green);
    }
    else if(currentOption == 1)
    {
        t_exit.setFillColor(sf::Color::Green);
    }
}

void Scene_Menu::draw(sf::RenderWindow& window) const
{
    window.draw(t_title);
    window.draw(t_play);
    window.draw(t_exit);
}

void Scene_Menu::pause() {}

void Scene_Menu::resume() {}
