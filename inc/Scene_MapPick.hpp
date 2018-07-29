#ifndef SCENE__MAPPICK_HPP
#define SCENE__MAPPICK_HPP

#include "Scene.hpp"
#include "List.hpp"
#include "AnimatedSprite.hpp"
#include "Database.hpp"

#include "Scene_UnitsPick.hpp"

class Scene_MapPick : public Scene
{
public:

    Scene_MapPick(SceneHandler& sceneHandler, Resources& resources, Database& database);
    ~Scene_MapPick();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    sf::Text t_title;
    sf::Text t_info;

    Database& _database;
    
    List _mapList;

    void loadMapsToList();

};

#endif