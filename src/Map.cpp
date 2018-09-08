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

    _pointer = Coord(0, 0);
    _selector = Coord(0, 0);
    _printPointer = false;
    _printSelector = false;

    _pendingUpdate = false;
}

Map::~Map() {}

void Map::setMap(Resources& resources, const MapData& mapData, std::vector<std::vector<Unit>>& teams)
{
    _base = mapData;
    _WCells = mapData._map.size();
    _HCells = mapData._map[0].size();

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

            TerrainType terrain = mapData._map[i][j];

            _map[i][j] = Cell(Coord(i, j), terrain);

            tmpTerrain.setTextureRect(sf::IntRect(terrain*64, 0, 64, 64));
            tmpTerrain.setPosition(_mapRect.left + i * _tileSize.x, _mapRect.top + j * _tileSize.y);

            tmpAction.setPosition(_mapRect.left + i * _tileSize.x, _mapRect.top + j * _tileSize.y);

            _map[i][j].rs_shape = tmpTerrain;
            _map[i][j].rs_action = tmpAction;
        }      
    }

    rs_pointer.setTexture(&resources.Texture("pointer"));
    rs_pointer.setTextureRect(sf::IntRect(0, 0, 64, 64));
    rs_pointer.setSize(_tileSize);
    rs_pointer.setPosition(_mapRect.left, _mapRect.top);

    _printPointer = true;

    rs_selector.setTexture(&resources.Texture("pointer"));
    rs_selector.setTextureRect(sf::IntRect(64, 0, 64, 64));
    rs_selector.setSize(_tileSize);
    rs_selector.setPosition(_mapRect.left, _mapRect.top);

    _printSelector = false;

    for(unsigned int i = 0; i < teams.size(); ++i)
    {
        for(unsigned int j = 0; j < teams[i].size(); ++j)
        {
            Coord pos = teams[i][j]._position;

            if(_map[pos.x][pos.y].empty() && terrainPass(teams[i][j]._base._movementType, _map[pos.x][pos.y]._type).first)
            {
                _map[pos.x][pos.y]._unit = &teams[i][j];
                _map[pos.x][pos.y]._checked = false;
                teams[i][j]._base._sprite.setScale(sf::Vector2f(_tileSize.x/64, _tileSize.y/64));
                teams[i][j]._base._sprite.setPosition(_mapRect.left + pos.x*_tileSize.x, _mapRect.top + pos.y*_tileSize.y);
                teams[i][j]._base._sprite.setActiveAnimation("idle");
                teams[i][j]._base._sprite.stopAnimation();
            }
            else
            {
                std::cerr << "ERROR: non empty or not passable cell for unit <" << teams[i][j]._base._name << ">" << std::endl;
            }
        }
    }

    _mapLoaded = true;
    _pendingUpdate = true;
}

Cell& Map::getCell(const Coord& coord)
{
    return _map[coord.x][coord.y];
}

Coord& Map::pointer()
{
    _updatePointer = true;

    return _pointer;
}

Cell& Map::getPointerCell()
{
    return _map[_pointer.x][_pointer.y];
}

Coord& Map::selector()
{
    _updateSelector = true;

    return _selector;
}

Cell& Map::getSelectorCell()
{
    return _map[_selector.x][_selector.y];
}

bool Map::selectCell(const Coord& coord, BfsType type)
{
    bool tarjets = false;
    Unit& current = *_map[coord.x][coord.y]._unit;

    selector() = coord;
    _printSelector = true;

    switch(type)
    {
        case BfsType::BT_MOVEMENT:
            tarjets = bfsMovement(current._position, current._team, current._movementType, current._movementRange, current._specialMovementRange);
            break;
        case BfsType::BT_ACTION:
            tarjets = bfsAction(current._position, current._team, current._base._weapon._tarjetsEnemy, current._base._weapon._tarjetsAlly, current._base._weapon._range, current._base._weapon._specialRange);
            break;
    }

    //bfs(current._position, current._team, current._movementType, current._movementRange);

   _pendingUpdate = true;

   return tarjets;
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

bool Map::correctCoord(const Coord& coord)
{
    if(coord.x >= 0 && unsigned(coord.x) < _WCells && coord.y >= 0 && unsigned(coord.y) < _HCells) return true;

    return false;
}

void Map::moveUnit(Unit* unit, const Coord& coord)
{
    if(correctCoord(coord) && _map[coord.x][coord.y].empty())
    {
        _map[unit->_position.x][unit->_position.y]._unit = nullptr;
        _map[coord.x][coord.y]._unit = unit;
        unit->_position = coord;
        
        unit->_base._sprite.setPosition(_mapRect.left + coord.x*_tileSize.x, _mapRect.top + coord.y*_tileSize.y);
    }
}

void Map::effect(const Coord& tarjet, Effect& effect)
{
    if(effect._haveSprite)
    {
        effect._sprite.setScale(sf::Vector2f(_tileSize.x/64, _tileSize.y/64));
        effect._sprite.setActiveAnimation("effect");

        for(unsigned int i = 0; i < effect._area.size(); ++i)
        {
            Coord coord = tarjet + effect._area[i];
            if(correctCoord(coord))
            {
                _effects.insert(_effects.end(), effect._sprite);
                _effects.back().setPosition(_mapRect.left + coord.x*_tileSize.x, _mapRect.top + coord.y*_tileSize.y);
            }
        }
    }

    if(effect._haveSound) effect._sound.play();
}

void Map::update(const sf::Time deltatime)
{
    if(_mapLoaded)
    {
        if(_updatePointer)
        {
            rs_pointer.setPosition(_mapRect.left + _pointer.x * _tileSize.x, _mapRect.top + _pointer.y * _tileSize.y);
            _updatePointer = false;
        }

        if(_updateSelector)
        {
            rs_selector.setPosition(_mapRect.left + _selector.x * _tileSize.x, _mapRect.top + _selector.y * _tileSize.y);
            _updateSelector = false;
        }

        if(_pendingUpdate)
        {
            for(unsigned int i = 0; i < _map.size(); ++i)
            {
                for(unsigned int j = 0; j < _map[0].size(); ++j)
                {
                    if(_map[i][j]._action != ActionType::AT_NONE) _map[i][j].rs_action.setTextureRect(sf::IntRect(int(_map[i][j]._action)*64, 0, 64, 64));
                }
            }

            _pendingUpdate = false;
        }

        std::list<AnimatedSprite>::iterator it = _effects.begin();
        while(it != _effects.end())
        {
            it->updateAnimation(deltatime);

            if(it->hasAnimationEnded())
            {
                it->firstAnimationFrame();
                it = _effects.erase(it);
            }
            ++it;
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
                    window.draw(_map[i][j]._unit->_base._sprite);
                }
            }
        }

        std::list<AnimatedSprite>::const_iterator it = _effects.begin();
        while(it != _effects.end())
        {
            window.draw(*it);
            ++it;
        }

        if(_printSelector) window.draw(rs_selector);
        if(_printPointer) window.draw(rs_pointer);
    }
}

bool Map::redCells(const Coord& origin, const std::set<int>& range, const std::vector<std::pair<Coord, Effect>>& coords)
{
    for(unsigned int i = 0; i < coords.size(); ++i)
    {
        Coord pos = origin + coords[i].first;
        if(correctCoord(pos))
        {
            _map[pos.x][pos.y]._action = ActionType::AT_ENEMY;
        }
    }

    bool tarjets = false;

    if(!range.empty())
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

            if(!cell._checked)
            {
                cell._distance = dist;
                cell._checked = true;

                if(cell._distance <= unsigned(maxRange))
                {
                    if(range.find(cell._distance) != range.end())
                    {
                        cell._action = ActionType::AT_ENEMY;
                        tarjets = true;
                    }

                    if(current.x < int(_WCells-1)) { q.push(current + Coord(1, 0)); ++count2; }
                    if(current.x > 0) { q.push(current + Coord(-1, 0)); ++count2; }
                    if(current.y < int(_HCells-1)) { q.push(current + Coord(0, 1)); ++count2; }
                    if(current.y > 0) { q.push(current + Coord(0, -1)); ++count2; }
                }
            }

            if(count1 == 0)
            {
                ++dist;
                count1 = count2;
                count2 = 0;
            }
        }
    }

    return tarjets;
}

bool Map::bfsMovement(const Coord& origin, unsigned int team, MovementType type, const std::set<int>& range, const std::vector<Coord>& specialRange)
{
    bool tarjets = false;

    if(!range.empty())
    {
        std::set<int>::iterator it = range.begin();
        int maxRange = *it;
        while(it != range.end())
        {
            if(*it > maxRange) maxRange = *it;
            ++it;
        }

        std::queue<std::pair<Coord, int>> q;
        q.push(std::make_pair(origin, 0));

        int count1 = 1, count2 = 0;
        int dist = 0;

        while(!q.empty())
        {
            std::pair<Coord, int> current = q.front();
            q.pop();
            Cell& cell = getCell(current.first);

            --count1;

            bool seg = false;
            int mod = 0;

            if(!cell._checked)
            {
                cell._distance = dist + current.second;
                cell._checked = true;

                if(cell._distance <= unsigned(maxRange))
                {
                    std::pair<bool, int> res = terrainPass(type, cell._type);

                    if(res.first)
                    {
                        if(cell.empty() || cell._unit->_team == team)
                        {
                            seg = true;
                            mod = res.second;

                            if(range.find(cell._distance) != range.end())
                            {
                                if(cell.empty())
                                {
                                    cell._action = ActionType::AT_MOVE;
                                    tarjets = true;
                                }
                            }
                        }
                    } 
                }

                if(seg)
                {
                    if(current.first.x < int(_WCells-1)) { q.push(std::make_pair(current.first + Coord(1, 0), mod)); ++count2; }
                    if(current.first.x > 0) { q.push(std::make_pair(current.first + Coord(-1, 0), mod)); ++count2; }
                    if(current.first.y < int(_HCells-1)) { q.push(std::make_pair(current.first + Coord(0, 1), mod)); ++count2; }
                    if(current.first.y > 0) { q.push(std::make_pair(current.first + Coord(0, -1), mod)); ++count2; }
                }
            }

            if(count1 == 0)
            {
                ++dist;
                count1 = count2;
                count2 = 0;
            }
        }
    }

    for(unsigned int i = 0; i < specialRange.size(); ++i)
    {
        Coord pos = specialRange[i];
        if(correctCoord(pos) && _map[pos.x][pos.y].empty())
        {
            _map[pos.x][pos.y]._action = ActionType::AT_MOVE;
            tarjets = true;
        }
    }

    return tarjets;
}

bool Map::bfsAction(const Coord& origin, unsigned int team, bool tarjetEnemy, bool tarjetAlly, const std::set<int>& range, const std::vector<Coord>& specialRange)
{
    bool tarjets = false;
    if(!range.empty())
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
            Cell& cell = getCell(current);
            q.pop();

            --count1;

            bool seg = false;

            if(!cell._checked)
            {
                cell._distance = dist;
                cell._checked = true;

                if(cell._distance <= unsigned(maxRange))
                {
                    seg = true;
                    if(!cell.empty() && range.find(cell._distance) != range.end())
                    {
                        if(tarjetEnemy && cell._unit->_team != team)
                        {
                            cell._action = ActionType::AT_ENEMY;
                            tarjets = true;
                        }

                        if(tarjetAlly && cell._unit->_team == team)
                        {
                            cell._action = ActionType::AT_ALLY;
                            tarjets = true;
                        }
                    }
                }

                if(seg)
                {
                    if(unsigned(current.x) < _WCells-1) { q.push(current + Coord(1, 0)); ++count2; }
                    if(current.x > 0) { q.push(current + Coord(-1, 0)); ++count2; }
                    if(unsigned(current.y) < _HCells-1) { q.push(current + Coord(0, 1)); ++count2; }
                    if(current.y > 0) { q.push(current + Coord(0, -1)); ++count2; }
                }
            }

            if(count1 == 0)
            {
                ++dist;
                count1 = count2;
                count2 = 0;
            }
        }
    }

    for(unsigned int i = 0; i < specialRange.size(); ++i)
    {
        Coord pos = specialRange[i];
        if(correctCoord(pos) && _map[pos.x][pos.y].empty())
        {
            _map[pos.x][pos.y]._action = ActionType::AT_MOVE;
            tarjets = true;
        }
    }

    return tarjets;
}

std::pair<bool, int> Map::terrainPass(MovementType movementType, TerrainType terrainType)
{
    bool pass = true;
    int mod = 0;

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
            if(movementType == MovementType::MT_WALKING) mod = 1;
            break;
        case TerrainType::TT_MOUNTAIN:
            if(movementType == MovementType::MT_MOUNTED || movementType == MovementType::MT_WALKING) pass = false;
            break;
        default:
            break;
    }

    return std::make_pair(pass, mod);
}

/*
void Map::bfs(const Coord& origin, unsigned int team, MovementType type, const std::set<int>& range)
{
    std::set<int>::iterator it = range.begin();

    int maxRange = *it;
    while(it != range.end())
    {
        if(*it > maxRange) maxRange = *it;
        ++it;
    }

    //bfs_i(origin, 0, team, range, maxRange, type);

    std::queue<std::pair<Coord, int>> q;
    q.push(std::make_pair(origin, 0));

    int count1 = 1, count2 = 0;
    int dist = 0;

    while(!q.empty())
    {
        std::pair<Coord, int> current = q.front();
        q.pop();
        Cell& cell = getCell(current.first);

        --count1;

        bool seg = false;
        int mod = 0;

        if(!cell._checked)
        {
            cell._distance = dist + current.second;

            if(dist + current.second > maxRange)
            {
                if(!cell.empty())
                {
                    if(cell._unit->_team != team) cell._action = ActionType::AT_ENEMY;
                    else cell._action = ActionType::AT_ALLY;
                }
            }
            else
            {
                if(range.find(dist + current.second) == range.end())
                {
                    if(cell.empty())
                    {
                        cell._action = ActionType::AT_NONE;
                        std::pair<bool, int> res = canPass(type, cell._type);
                        if(res.first) { seg = true; mod = res.second; }
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
                        std::pair<bool, int> res = canPass(type, cell._type);
                        if(res.first)
                        {
                            cell._action = ActionType::AT_MOVE;
                            seg = true;
                            mod = res.second;
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
                if(current.first.x < int(_WCells-1)) { q.push(std::make_pair(current.first + Coord(1, 0), mod)); ++count2; }
                if(current.first.x > 0) { q.push(std::make_pair(current.first + Coord(-1, 0), mod)); ++count2; }
                if(current.first.y < int(_HCells-1)) { q.push(std::make_pair(current.first + Coord(0, 1), mod)); ++count2; }
                if(current.first.y > 0) { q.push(std::make_pair(current.first + Coord(0, -1), mod)); ++count2; }
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
*/
/*
void Map::bfs_i(const Coord& current, unsigned int dist, unsigned int team, const std::set<int>& range, unsigned int maxRange, MovementType type)
{
    Cell& cell = getCell(current);

    bool seg = false;

    if(!cell._checked || (cell._checked && cell._distance > dist))
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
    }

    if(seg)
    {
        if(current.x < int(_WCells-1)) bfs_i(current + Coord(1, 0), dist+1, team, range, maxRange, type);
        if(current.x > 0) bfs_i(current + Coord(-1, 0), dist+1, team, range, maxRange, type);
        if(current.y < int(_HCells-1)) bfs_i(current + Coord(0, 1), dist+1, team, range, maxRange, type);
        if(current.y > 0) bfs_i(current + Coord(0, -1), dist+1, team, range, maxRange, type);
    }
}
*/