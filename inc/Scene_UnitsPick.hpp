#ifndef SCENE__UNITSPICK_HPP
#define SCENE__UNITSPICK_HPP

#include "Scene.hpp"

#include "List.hpp"
#include "Database.hpp"
#include "Scene_Play.hpp"

class Scene_UnitsPick : public Scene
{
public:

    Scene_UnitsPick(SceneHandler& sceneHandler, Resources& resources, Database& database, MapData mapData);
    ~Scene_UnitsPick();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    sf::Text t_title;
    sf::Text t_info;

    List _unitsList1;
    List _unitsList2;

    Database& _database;

    void loadUnitsToList();

    MapData _mapData;

};

#endif