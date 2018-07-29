#include "Map.hpp"

Cell::Cell()
{
    _position = Coord(0, 0);
    _type = TerrainType::TT_WALL;
}

Cell::Cell(Coord position, TerrainType type)
{
    _position = position;
    _type = type;
}

Map::Map()
{
    _mapLoaded = false;
}

Map::Map(sf::FloatRect mapRect)
{
    _mapLoaded = false;
    _mapRect = mapRect;

    rs_background.setPosition(_mapRect.left, _mapRect.top);
    rs_background.setSize(sf::Vector2f(_mapRect.width, _mapRect.height));
    rs_background.setFillColor(sf::Color::Black);
    rs_background.setOutlineColor(sf::Color::Yellow);
    rs_background.setOutlineThickness(4.f);
}

Map::~Map() {}

void Map::setMap(Resources& resources, const MapData& map)
{
    _WCells = map._map.size();
    _HCells = map._map[0].size();

    _map = std::vector<std::vector<Cell>>(_WCells, std::vector<Cell>(_HCells));
    _mapTiles = std::vector<sf::RectangleShape>(_WCells * _HCells);

    _tileSize = sf::Vector2f(_mapRect.width/_WCells, _mapRect.height/_HCells);

    sf::RectangleShape tmpTerrain;
    tmpTerrain.setSize(_tileSize);
    tmpTerrain.setTexture(&resources.Texture("terrain"));

    for(unsigned int i = 0; i < _WCells; ++i)
    {
        for(unsigned int j = 0; j < _HCells; ++j)
        {

            TerrainType terrain = map._map[i][j];

            _map[i][j] = Cell(Coord(i, j), terrain);

            tmpTerrain.setTextureRect(sf::IntRect(terrain*64, 0, 64, 64));
            tmpTerrain.setPosition(_mapRect.left + i * _tileSize.x, _mapRect.top + j * _tileSize.y);

            _mapTiles[(i*_HCells) + j] = tmpTerrain;
        }      
    }

    _mapLoaded = true;
}

void Map::update(const sf::Time deltatime)
{

}

void Map::draw(sf::RenderWindow& window) const
{
    window.draw(rs_background);

    if(_mapLoaded)
    {
        for(unsigned int i = 0; i < _mapTiles.size(); ++i)
        {
            window.draw(_mapTiles[i]);
        }
    }
}