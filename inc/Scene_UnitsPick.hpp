#ifndef SCENE__UNITSPICK_HPP
#define SCENE__UNITSPICK_HPP

#include "Scene.hpp"

#include "List.hpp"
#include "Database.hpp"
#include "Scene_Play.hpp"

class Scene_UnitsPick : public Scene
{
public:

    Scene_UnitsPick(SceneHandler& sceneHandler, Resources& resources, Database& database, MapData* mapData);
    ~Scene_UnitsPick();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    /* F */
    void loadUnitsToLists();

    /* DATA */
    Database& _database;
    MapData* _mapData;

    std::vector<List> _unitsLists;

    /* VARS */
    unsigned int _currentTeam;

    /* SFML*/
    sf::Text t_title;
    sf::Text t_info;
};

#endif