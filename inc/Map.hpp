#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "Coord.hpp"
#include "Resources.hpp"

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

    //Unit* _unit;

    //BFS
    bool _checked;
    unsigned int _distance;

    //PointerType _pointer;

    Cell();

    Cell(Coord position, TerrainType type);
};

class Map
{
public:

    Map();
    Map(sf::FloatRect mapRect);
    ~Map();

    void setMap(Resources& resources, const MapData& map);

    void setPointer(const Coord& coord);

    Cell& getCell(const Coord& coord);

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

    std::vector<sf::RectangleShape> _mapTiles;

    bool _printPointer;

    sf::RectangleShape rs_pointer;
};

#endif