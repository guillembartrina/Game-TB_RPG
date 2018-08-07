#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <queue>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "Coord.hpp"
#include "Resources.hpp"
#include "Unit.hpp"

#include "Enums.hpp"

struct MapData
{
    std::string _name;
    std::vector<std::vector<TerrainType>> _map;

    std::vector<std::vector<Coord>> _teams;

    sf::Sprite _preview;
};

struct Cell
{
    Coord _position;
    TerrainType _type;

    Unit* _unit;

    //BFS
    bool _checked;
    unsigned int _distance;

    ActionType _action;

    sf::RectangleShape rs_shape;
    sf::RectangleShape rs_action;

    Cell();
    Cell(Coord position, TerrainType type);

    bool empty() const;
};

class Map
{
public:

    Map();
    Map(sf::FloatRect mapRect);
    ~Map();

    void setMap(Resources& resources, const MapData& map, std::vector<std::vector<Unit>>& teams);

    void setPointer(const Coord& coord);

    Cell& getCell(const Coord& coord);

    void selectCell(const Coord& coord, bool movement = true);
    void eraseSelection();

    bool correctCoord(const Coord& coord);

    void moveUnit(Unit* unit, const Coord& coord);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    unsigned int _WCells;
    unsigned int _HCells;

    Coord _pointer;
    Coord _selector;

private:

    sf::FloatRect _mapRect;

    sf::Vector2f _tileSize;

    sf::RectangleShape rs_background;

    bool _mapLoaded;
    bool _pendingUpdate;

    std::vector<std::vector<Cell>> _map;

    bool _printPointer;
    sf::RectangleShape rs_pointer;

    bool _printSelector;
    sf::RectangleShape rs_selector;

    void bfsMovement(const Coord& origin, unsigned int team, MovementType type, const std::set<int>& range, const std::vector<Coord>& specialRange);
    void bfsAction(const Coord& origin, unsigned int team, bool tarjetEnemy, bool tarjetAlly, const std::set<int>& range, const std::vector<Coord>& specialRange);

    std::pair<bool, int> terrainPass(MovementType mType, TerrainType tType);

    //void bfs(const Coord& origin, unsigned int team, MovementType type, const std::set<int>& range);
    //void bfs_i(const Coord& current, unsigned int dist, unsigned int team, const std::set<int>& range, unsigned int maxRange, MovementType type);
};

#endif