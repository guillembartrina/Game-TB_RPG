#include "Map.hpp"

Cell::Cell()
{
    _position = Coord(0, 0);
    _type = TerrainType::TT_WALL;
    _action = ActionType::AT_NONE;
    _unit = nullptr;
}

Cell::Cell(Coord position, TerrainType type)
{
    _position = position;
    _type = type;
    _action = ActionType::AT_NONE;
    _unit = nullptr;
}

bool Cell::empty() const
{
    return (_unit == nullptr);
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

    _printPointer = false;
    _printSelector = false;
}

Map::~Map() {}

void Map::setMap(Resources& resources, const MapData& map, std::vector<std::vector<Unit>>& teams)
{
    _WCells = map._map.size();
    _HCells = map._map[0].size();

    _map = std::vector<std::vector<Cell>>(_WCells, std::vector<Cell>(_HCells));

    _tileSize = sf::Vector2f(_mapRect.width/_WCells, _mapRect.height/_HCells);

    sf::RectangleShape tmpTerrain;
    tmpTerrain.setSize(_tileSize);
    tmpTerrain.setTexture(&resources.Texture("terrain"));

    sf::RectangleShape tmpAction;
    tmpAction.setSize(_tileSize);
    tmpAction.setTexture(&resources.Texture("action"));

    for(unsigned int i = 0; i < _WCells; ++i)
    {
        for(unsigned int j = 0; j < _HCells; ++j)
        {

            TerrainType terrain = map._map[i][j];

            _map[i][j] = Cell(Coord(i, j), terrain);

            tmpTerrain.setTextureRect(sf::IntRect(terrain*64, 0, 64, 64));
            tmpTerrain.setPosition(_mapRect.left + i * _tileSize.x, _mapRect.top + j * _tileSize.y);

            tmpAction.setPosition(_mapRect.left + i * _tileSize.x, _mapRect.top + j * _tileSize.y);

            _map[i][j].rs_shape = tmpTerrain;
            _map[i][j].rs_action = tmpAction;
        }      
    }

    rs_pointer.setTexture(&resources.Texture("pointer"));
    rs_pointer.setTextureRect(sf::IntRect(0, 0, 32, 32));
    rs_pointer.setSize(_tileSize);
    rs_pointer.setPosition(_mapRect.left, _mapRect.top);

    _printPointer = true;

    rs_selector.setTexture(&resources.Texture("pointer"));
    rs_selector.setTextureRect(sf::IntRect(32, 0, 32, 32));
    rs_selector.setSize(_tileSize);
    rs_selector.setPosition(_mapRect.left, _mapRect.top);

    _printSelector = false;

    for(unsigned int i = 0; i < teams.size(); ++i)
    {
        for(unsigned int j = 0; j < teams[i].size(); ++j)
        {
            if(existCell(teams[i][j]._position) && getCell(teams[i][j]._position).empty() && canPassUnitTerrain(teams[i][j]._movementType, getCell(teams[i][j]._position)._type))
            {
                getCell(teams[i][j]._position)._unit = &teams[i][j];
                teams[i][j]._sprite.setScale(sf::Vector2f(_tileSize.x/64, _tileSize.y/64));
                teams[i][j]._sprite.setActiveAnimation("idle");
                teams[i][j]._sprite.stopAnimation();
            }
            else
            {
                std::cerr << "Error casting unit, no spawn cell avaliabe, not movement type passable or non-empty cell" << std::endl;
            }
        }
    }

    _mapLoaded = true;
}

void Map::setPointer(const Coord& coord)
{
    if(_mapLoaded)
    {
        rs_pointer.setPosition(_mapRect.left + coord.x * _tileSize.x, _mapRect.top + coord.y * _tileSize.y);
    }
}

bool Map::existCell(const Coord& coord)
{
    if(coord.x >= 0 && coord.x < int(_WCells) && coord.y >= 0 && coord.y < int(_HCells)) return true;

    return false;
}

Cell& Map::getCell(const Coord& coord)
{
    return _map[coord.x][coord.y];
}

void Map::selectCell(const Coord& coord)
{
    //Unit& current = *getCell(coord)._unit;

    rs_selector.setPosition(_mapRect.left + coord.x*_tileSize.x, _mapRect.top + coord.y*_tileSize.y);
    _printSelector = true;

    std::cerr << "Activ" << _printSelector;

    /*

    if(!current._movementRange.empty())
    {
        std::set<int>::iterator it = current._movementRange.begin();
        int maxRange = *it;
        while(it != current._movementRange.end())
        {
            if(*it > maxRange) maxRange = *it;
            ++it;
        }
        //bfs(unit._team, unit._movementRange, maxRange, unit._movementType, unit._position, 0);
    }

    if(!current._specialMovement)
    {
        for(unsigned int i = 0; i < current._specialMovementCoords.size(); ++i)
        {
            if(existCell(current._specialMovementCoords[i]) && getCell(current._specialMovementCoords[i])._empty)
            {
                //bfs(unit._team, {0}, 0, unit._movementType, unit._specialMovementCoords[i], 0);
            }
        }
    }
    */
}

void Map::update(const sf::Time deltatime)
{
    if(_mapLoaded)
    {
        for(unsigned int i = 0; i < _map.size(); ++i)
        {
            for(unsigned int j = 0; j < _map[0].size(); ++j)
            {
                if(_map[i][j]._action != ActionType::AT_NONE) _map[i][j].rs_action.setTextureRect(sf::IntRect(_map[i][j]._action*32, 0, 32, 32));

                if(!_map[i][j].empty()) _map[i][j]._unit->_sprite.setPosition(_mapRect.left + i*_tileSize.x, _mapRect.top + j*_tileSize.y);
            }
        }
    }
}

void Map::draw(sf::RenderWindow& window) const
{
    window.draw(rs_background);

    if(_mapLoaded)
    {
        for(unsigned int i = 0; i < _map.size(); ++i)
        {
            for(unsigned int j = 0; j < _map[0].size(); ++j)
            {
                window.draw(_map[i][j].rs_shape);

                if(_map[i][j]._action != ActionType::AT_NONE) window.draw(_map[i][j].rs_action);
                if(!_map[i][j].empty()) 
                {
                    window.draw(_map[i][j]._unit->_sprite);
                }
            }
        }

        if(_printSelector) window.draw(rs_selector);
        if(_printPointer) window.draw(rs_pointer);
    }
}

bool Map::canPassUnitTerrain(MovementType movementType, TerrainType terrainType)
{
    bool pass = true;

    switch(terrainType)
    {
        case TerrainType::TT_WALL:
            pass = false;
            break;
        case TerrainType::TT_WATER:
            if(movementType == MovementType::MT_MOUNTED || movementType == MovementType::MT_WALKING) pass = false;
            break;
        case TerrainType::TT_FOREST:
            if(movementType == MovementType::MT_MOUNTED) pass = false;
            break;
        case TerrainType::TT_MOUNTAIN:
            if(movementType == MovementType::MT_MOUNTED || movementType == MovementType::MT_WALKING) pass = false;
            break;
        default:
            break;
    }

    return pass;
}