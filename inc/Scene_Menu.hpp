#ifndef SCENE__MENU_HPP
#define SCENE__MENU_HPP

#include "Scene.hpp"

#include "Scene_MapPick.hpp"

class Scene_Menu : public Scene
{
public:

    Scene_Menu(SceneHandler& sceneHandler, Resources& resources);
    ~Scene_Menu();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    /* DATA */
    Database _database;

    /* VARS */
    int currentOption;

    /* SFML */
    sf::Text t_title;
    sf::Text t_play;
    sf::Text t_exit;
};

#endif