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

    _pendingUpdate = false;
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
            if(getCell(teams[i][j]._position).empty() && canPass(teams[i][j]._movementType, getCell(teams[i][j]._position)._type))
            {
                getCell(teams[i][j]._position)._unit = &teams[i][j];
                getCell(teams[i][j]._position)._checked = false;
                teams[i][j]._sprite.setScale(sf::Vector2f(_tileSize.x/64, _tileSize.y/64));
                teams[i][j]._sprite.setActiveAnimation("idle");
                teams[i][j]._sprite.stopAnimation();
            }
            else std::cerr << "ERROR: non empty or not passable cell for unit <" << teams[i][j]._name << ">" << std::endl;
        }
    }

    _mapLoaded = true;

    _pendingUpdate = true;
}

void Map::setPointer(const Coord& coord)
{
    if(_mapLoaded)
    {
        rs_pointer.setPosition(_mapRect.left + coord.x * _tileSize.x, _mapRect.top + coord.y * _tileSize.y);
    }
}

Cell& Map::getCell(const Coord& coord)
{
    return _map[coord.x][coord.y];
}

void Map::selectCell(const Coord& coord)
{
    Unit& current = *getCell(coord)._unit;

    rs_selector.setPosition(_mapRect.left + coord.x*_tileSize.x, _mapRect.top + coord.y*_tileSize.y);
    _printSelector = true;

    if(!current._movementRange.empty()) bfs(current._position, current._team, current._movementRange, current._movementType);

    /*

    if(!current._specialMovement)
    {
        for(unsigned int i = 0; i < current._specialMovementCoords.size(); ++i)
        {
            if(correctCoord(current._specialMovementCoords[i]) && getCell(current._specialMovementCoords[i]).empty())
            {
                //bfs(unit._team, {0}, 0, unit._movementType, unit._specialMovementCoords[i], 0);
            }
        }
    }
    */

   _pendingUpdate = true;
}

void Map::eraseSelection()
{
    _printSelector = false;

    for(unsigned int i = 0; i < _map.size(); ++i)
    {
        for(unsigned int j = 0; j < _map[0].size(); ++j)
        {
            _map[i][j]._checked = false;
            _map[i][j]._distance = 0;
            _map[i][j]._action = ActionType::AT_NONE;
        }
    }

    _pendingUpdate = true;
}

void Map::update(const sf::Time deltatime)
{
    if(_mapLoaded)
    {
        if(_pendingUpdate)
        {
            for(unsigned int i = 0; i < _map.size(); ++i)
            {
                for(unsigned int j = 0; j < _map[0].size(); ++j)
                {
                    if(_map[i][j]._action != ActionType::AT_NONE) _map[i][j].rs_action.setTextureRect(sf::IntRect(int(_map[i][j]._action)*32, 0, 32, 32));

                    if(!_map[i][j].empty()) _map[i][j]._unit->_sprite.setPosition(_mapRect.left + i*_tileSize.x, _mapRect.top + j*_tileSize.y);
                }
            }

            _pendingUpdate = false;
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

bool Map::correctCoord(const Coord& coord)
{
    if(coord.x >= 0 && coord.x < int(_WCells) && coord.y >= 0 && coord.y < int(_HCells)) return true;

    return false;
}

void Map::bfs(const Coord& origin, unsigned int team, const std::set<int>& range, MovementType type)
{

    std::set<int>::iterator it = range.begin();

    int maxRange = *it;
    while(it != range.end())
    {
        if(*it > maxRange) maxRange = *it;
        ++it;
    }

    std::queue<Coord> q;
    q.push(origin);

    int count1 = 1, count2 = 0;
    int dist = 0;

    while(!q.empty())
    {
        Coord current = q.front();
        q.pop();
        Cell& cell = getCell(current);

        --count1;

        bool seg = false;

        if(!cell._checked)
        {
            cell._distance = dist;

            if(dist > maxRange)
            {
                if(!cell.empty())
                {
                    if(cell._unit->_team != team) cell._action = ActionType::AT_ENEMY;
                    else cell._action = ActionType::AT_ALLY;
                }
            }
            else
            {
                std::cerr << ":" << dist << "::" << (range.find(dist) == range.end()) << std::endl;
                if(range.find(dist) == range.end())
                {
                    if(cell.empty())
                    {
                        cell._action = ActionType::AT_NONE;
                        if(canPass(type, cell._type)) seg = true;
                    }
                    else
                    {
                        if(cell._unit->_team != team)
                        {
                            cell._action = ActionType::AT_ENEMY;
                        }
                        else
                        {
                            cell._action = ActionType::AT_ALLY;
                            seg = true;
                        }
                    }
                }
                else
                {
                    if(dist == 0 || cell.empty())
                    {
                        if(canPass(type, cell._type))
                        {
                            cell._action = ActionType::AT_MOVE;
                            seg = true;
                        }
                    }
                    else
                    {
                        if(cell._unit->_team != team)
                        {
                            cell._action = ActionType::AT_ENEMY;
                        }
                        else
                        {
                            cell._action = ActionType::AT_ALLY;
                            seg = true;
                        }
                    }
                }

                
            }

            if(seg)
            {
                if(current.x < int(_WCells-1)) { q.push(current + Coord(1, 0)); ++count2; }
                if(current.x > 0) { q.push(current + Coord(-1, 0)); ++count2; }
                if(current.y < int(_HCells-1)) { q.push(current + Coord(0, 1)); ++count2; }
                if(current.y > 0) { q.push(current + Coord(0, -1)); ++count2; }
            }
            
            cell._checked = true;
        }

        if(count1 == 0)
        {
            ++dist;
            count1 = count2;
            count2 = 0;
        }
    }
}

bool Map::canPass(MovementType movementType, TerrainType terrainType)
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