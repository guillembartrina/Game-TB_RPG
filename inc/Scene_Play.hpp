#ifndef SCENE__PLAY_HPP
#define SCENE__PLAY_HPP

#include <list>

#include "Scene.hpp"

#include "Map.hpp"
#include "Unit.hpp"

class Scene_Play : public Scene
{
public:

    Scene_Play(SceneHandler& sceneHandler, Resources& resources, MapData* mapData, std::vector<std::list<UnitData>>* unitsData);
    ~Scene_Play();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    MapData _mapData;
    Map _map;
    //sf::Vector2i _mapSize;

    std::vector<std::list<UnitData>> _unitsData;
    std::vector<std::vector<Unit>> _teams;

    unsigned int _currentTeam;

    TurnPhase _currentTurnPhase;

    bool _selected;
    Unit* _currentUnit;

    sf::Text t_title;

    sf::RectangleShape rs_info;
    sf::Text t_currentTeam;

    void initPhase(unsigned int team);
};

#endif