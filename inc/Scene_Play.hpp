#ifndef SCENE__PLAY_HPP
#define SCENE__PLAY_HPP

#include <list>

#include "Scene.hpp"

#include "Map.hpp"
#include "Unit.hpp"

class Scene_Play : public Scene
{
public:

    Scene_Play(SceneHandler& sceneHandler, Resources& resources, const MapData& mapData, const std::vector<std::list<UnitData>>& unitsData);
    ~Scene_Play();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    sf::Text t_title;

    MapData _mapData;
    Map _map;

    sf::Vector2i _mapSize;

    std::vector<std::list<UnitData>> _unitsData;

    std::vector<std::vector<Unit>> _teams;

    Coord _pointer;
    bool _selected;
    Coord _selector;

    unsigned int _currentTeam;

};

#endif