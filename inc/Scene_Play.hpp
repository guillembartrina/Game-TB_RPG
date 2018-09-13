#ifndef SCENE__PLAY_HPP
#define SCENE__PLAY_HPP

#include <list>

#include "Scene.hpp"
#include "Error.hpp"

#include "Map.hpp"
#include "Unit.hpp"
#include "List.hpp"

enum DataUnit
{
    DU_NAME,
    DU_TEAM,
    DU_HP,
    DU_RESIST_F,
    DU_RESIST_M,
    DU_WEAPON,
    DU_WEAPON_TYPE,
    DU_WEAPON_RANGE,
    DU_WEAPON_SPECIAL_RANGE,
    DU_MOVEMENT,
    DU_MOVEMENT_RANGE,
    DU_MOVEMENT_SPECIAL_RANGE,
    DU_GOD,
    DU_ELEMS
};

class Scene_Play : public Scene
{
public:

    Scene_Play(SceneHandler& sceneHandler, Resources& resources, MapData* mapData, std::vector<std::vector<UnitData>>* unitsData);
    ~Scene_Play();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    /* F */
    void initPhase(unsigned int team);
    void endTurn();
    void setDataUnit(const Unit& unit);

    std::string getDataModification(const Modification& modification);

    void effect(const Coord& tarjet, Effect& effect);
    void ability(const Coord& tarjet, Ability& ability);

    void kill(Unit* unit);

    /* DATA */
    MapData _mapData;
    Map _map;

    std::vector<std::vector<UnitData>> _unitsData;
    std::vector<std::vector<Unit>> _teams;

    /* VARS */
    unsigned int _currentTeam;
    unsigned int _remainUnits;
    TurnPhase _currentTurnPhase;
    bool _selected;
    Unit* _currentUnit;
    Ability* _currentAbility;

    /* SFML */
    sf::Text t_title;
    sf::RectangleShape rs_info;
    sf::Text t_currentTeam;
    std::vector<sf::Text> t_dataUnit;
    sf::Text t_infoAbility;
    sf::Text t_infoFinish;
    sf::Text t_infoPassives;
    sf::Text t_infoBack;
    List _passives;
    List _abilities;
    
};

#endif