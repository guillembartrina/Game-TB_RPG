#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>

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

    bool _empty;
    Unit* _unit;

    //BFS
    bool _checked;
    unsigned int _distance;

    ActionType _action;
    sf::RectangleShape rs_action;

    //PointerType _pointer;

    sf::RectangleShape rs_shape;

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

    bool existCell(const Coord& coord);
    Cell& getCell(const Coord& coord);

    void selectCell(const Coord& coord);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

private:

    sf::FloatRect _mapRect;

    unsigned int _WCells;
    unsigned int _HCells;
    sf::Vector2f _tileSize;

    sf::RectangleShape rs_background;

    bool _mapLoaded;

    std::vector<std::vector<Cell>> _map;

    bool _printPointer;
    sf::RectangleShape rs_pointer;

    bool _printSelector;
    sf::RectangleShape rs_selector;

    //void bfs(unsigned int team, const std::set<int>& range, int maxRange, MovementType type, const Coord& currentCell, int currentDistance);

    bool canPassUnitTerrain(MovementType mType, TerrainType tType);
};

#endif