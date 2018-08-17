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

    /* BFS */
    bool _checked;
    unsigned int _distance;

    ActionType _action;

    /* SFML */
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

    Cell& getCell(const Coord& coord);
    Coord& pointer();
    Coord& selector();

    bool selectCell(const Coord& coord, bool movement = true);
    void eraseSelection();

    bool correctCoord(const Coord& coord);

    void moveUnit(Unit* unit, const Coord& coord);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    unsigned int _WCells;
    unsigned int _HCells;

private:

    /* F */
    bool bfsMovement(const Coord& origin, unsigned int team, MovementType type, const std::set<int>& range, const std::vector<Coord>& specialRange);
    bool bfsAction(const Coord& origin, unsigned int team, bool tarjetEnemy, bool tarjetAlly, const std::set<int>& range, const std::vector<Coord>& specialRange);

    std::pair<bool, int> terrainPass(MovementType mType, TerrainType tType);

    /* DATA */
    sf::FloatRect _mapRect;
    sf::Vector2f _tileSize;
    std::vector<std::vector<Cell>> _map;
    
    bool _printPointer;
    bool _printSelector;

    /* VARS */
    bool _mapLoaded;
    bool _pendingUpdate;

    Coord _pointer;
    bool _updatePointer;
    Coord _selector;
    bool _updateSelector;

    /* SFML */
    sf::RectangleShape rs_background;
    sf::RectangleShape rs_pointer;
    sf::RectangleShape rs_selector;

    //void bfs(const Coord& origin, unsigned int team, MovementType type, const std::set<int>& range);
    //void bfs_i(const Coord& current, unsigned int dist, unsigned int team, const std::set<int>& range, unsigned int maxRange, MovementType type);
};

#endif