#ifndef SCENE__PLAY_HPP
#define SCENE__PLAY_HPP

#include "Scene.hpp"

#include "Map.hpp"

class Scene_Play : public Scene
{
public:

    Scene_Play(SceneHandler& sceneHandler, Resources& resources, MapData mapData);
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

};

#endif