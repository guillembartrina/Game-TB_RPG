#include "List.hpp"

Item::Item() {}

void Item::addText(sf::Vector2f coord, sf::Text text)
{
    _texts.insert(_texts.end(), std::make_pair(coord, text));
}

void Item::addSprite(sf::Vector2f coord, sf::Sprite sprite)
{
    _sprites.insert(_sprites.end(), std::make_pair(coord, sprite));
}

void Item::addAnimatedSprite(sf::Vector2f coord, AnimatedSprite animatedSprite, std::string animationName)
{
    std::list<std::pair<sf::Vector2f, AnimatedSprite>>::iterator it = _animatedSprites.insert(_animatedSprites.end(), std::make_pair(coord, animatedSprite));
    it->second.setActiveAnimation(animationName);
}

Element::Element() {}

void Element::setItem(Item& item)
{
    _item = &item;
}

List::List() {}

List::List(sf::FloatRect listRect, int numPageElements, unsigned int numPickableItems)
{
    _listRect = listRect;
    _numPageElements = numPageElements;
    _pickableItems = (numPickableItems != 0);
    _numPickableItems = numPickableItems;

    _active = false;

    c_rectOutline = sf::Color(255, 153, 0);
    c_element = sf::Color(51, 102, 153);
    c_pickedElement = sf::Color(0, 153, 153);
    c_pickerOutline = sf::Color(255, 0, 0);
    c_elementOutline = sf::Color(255, 255, 0);

    _page = std::vector<Element>(_numPageElements);

    for(unsigned int i = 0; i < _numPageElements; ++i)
    {
        _page[i]._active = false;
        _page[i]._item = nullptr;
        _page[i].rs_background.setPosition(_listRect.left, _listRect.top + i*(_listRect.height/_numPageElements));
        _page[i].rs_background.setSize(sf::Vector2f(_listRect.width, _listRect.height/_numPageElements));
        _page[i].rs_background.setFillColor(c_element);
        _page[i].rs_background.setOutlineColor(c_elementOutline);
    }

    rs_background.setPosition(sf::Vector2f(_listRect.left, _listRect.top));
    rs_background.setSize(sf::Vector2f(_listRect.width, _listRect.height));
    rs_background.setFillColor(sf::Color::Black);
    rs_background.setOutlineColor(c_rectOutline);
    rs_background.setOutlineThickness(4.f);
}

List::~List() {}

void List::add(Item& item)
{

    _items.insert(_items.end(), item);
    _items.back()._id = _items.size()-1;
    _items.back()._picked = false;

    if(!_active)
    {
        _currentItem = _items.begin();
        _currentPageElement = 0;

        _page[0]._active = true;
        _page[0].setItem(_items.back());
        _page[0]._selected = true;

        _active = true;
    }
    else
    {
        if(_items.size() <= _numPageElements)
        {
            _page[_items.size()-1]._active = true;
            _page[_items.size()-1].setItem(_items.back());
            _page[_items.size()-1]._selected = false;
        }
    }
    
}

void List::up()
{
    if(_active)
    {
        if(_currentPageElement == 0)
        {
            if(_currentItem->_id != 0)
            {
                --_currentItem;

                for(unsigned int i = _numPageElements-1; i > 0; --i)
                {
                    _page[i]._item = _page[i-1]._item;
                    _page[i]._selected = false;
                }

                _page.front().setItem(*_currentItem);

            }
        }
        else
        {
            _page[_currentPageElement]._selected = false;
            --_currentItem;
            --_currentPageElement;
            _page[_currentPageElement]._selected = true;
        }
    }
}

void List::down()
{
    if(_active)
    {
        if(_currentPageElement == _numPageElements-1)
        {
            if(_currentItem->_id != _items.size()-1)
            {
                ++_currentItem;

                for(unsigned int i = 0; i < _numPageElements-1; ++i)
                {
                    _page[i]._item = _page[i+1]._item;
                    _page[i]._selected = false;
                }

                _page.back().setItem(*_currentItem);
            }
        }
        else
        {
            if(_page[_currentPageElement+1]._active)
            {
                _page[_currentPageElement]._selected = false;
                ++_currentItem;
                ++_currentPageElement;
                _page[_currentPageElement]._selected = true;
            }
        }
    }
}

void List::pick()
{
    if(_pickableItems && _active)
    {
        if(_currentItem->_picked)
        {
            _currentItem->_picked = false;

            std::list<std::list<Item>::iterator>::iterator it = _pickedItems.begin();
            bool found = false;
            while(!found && it != _pickedItems.end())
            {
                if(*it == _currentItem)
                {
                    found = true;
                    _pickedItems.erase(it);
                }
                ++it;
            }
        }
        else
        {
            if(_pickedItems.size() < _numPickableItems)
            {
                _currentItem->_picked = true;
                _pickedItems.insert(_pickedItems.end(), _currentItem);
            }
        }
    }
}

Item List::current()
{
    if(_active)
    {
        return *_currentItem;
    }

    return Item();
}

bool List::active() const
{
    return _active;
}

bool List::allPicked() const
{
    return (_pickedItems.size() == _numPickableItems);
}

std::list<std::list<Item>::iterator>& List::getPicked()
{
    return _pickedItems;
}

void List::update(const sf::Time deltatime)
{
    if(_active)
    {
        for(unsigned int i = 0; i < _numPageElements; ++i)
        {
            if(_page[i]._active)
            {
                if(_page[i]._selected)
                {
                    _page[i].rs_background.setOutlineColor(c_pickerOutline);
                    _page[i].rs_background.setOutlineThickness(-3.f);
                }
                else
                {
                    _page[i].rs_background.setOutlineColor(c_elementOutline);
                    _page[i].rs_background.setOutlineThickness(-1.f);
                }

                if(_page[i]._item->_picked) _page[i].rs_background.setFillColor(c_pickedElement);
                else _page[i].rs_background.setFillColor(c_element);

                sf::Vector2f scale = sf::Vector2f(_listRect.width/maxW, (_listRect.height/_numPageElements)/maxH);

                sf::Vector2f relativeCoord = sf::Vector2f(_page[i].rs_background.getPosition());

                std::list<std::pair<sf::Vector2f, sf::Text>>::iterator it = _page[i]._item->_texts.begin();
                while(it != _page[i]._item->_texts.end())
                {
                    it->second.setPosition(relativeCoord + sf::Vector2f(it->first.x * scale.x, it->first.y * scale.y));
                    it->second.setScale(scale);
                    ++it;
                }

                std::list<std::pair<sf::Vector2f, sf::Sprite>>::iterator it2 = _page[i]._item->_sprites.begin();
                while(it2 != _page[i]._item->_sprites.end())
                {
                    it2->second.setPosition(relativeCoord + sf::Vector2f(it2->first.x * scale.x, it2->first.y * scale.y));
                    it2->second.setScale(scale);
                    ++it2;
                }

                std::list<std::pair<sf::Vector2f, AnimatedSprite>>::iterator it3 = _page[i]._item->_animatedSprites.begin();
                while(it3 != _page[i]._item->_animatedSprites.end())
                {
                    it3->second.updateAnimation(deltatime);
                    it3->second.setPosition(relativeCoord + sf::Vector2f(it3->first.x * scale.x, it3->first.y * scale.y));
                    it3->second.setScale(scale);
                    ++it3;
                }
            }
        }
    }
}

void List::draw(sf::RenderWindow& window) const
{
    window.draw(rs_background);

    if(_active)
    {
        for(unsigned int i = 0; i < _numPageElements; ++i)
        {
            if(_page[i]._active)
            {
                window.draw(_page[i].rs_background);

                std::list<std::pair<sf::Vector2f, sf::Text>>::const_iterator it = _page[i]._item->_texts.begin();
                while(it != _page[i]._item->_texts.end())
                {
                    window.draw(it->second);
                    ++it;
                }

                std::list<std::pair<sf::Vector2f, sf::Sprite>>::const_iterator it2 = _page[i]._item->_sprites.begin();
                while(it2 != _page[i]._item->_sprites.end())
                {
                    window.draw(it2->second);
                    ++it2;
                }

                std::list<std::pair<sf::Vector2f, AnimatedSprite>>::const_iterator it3 = _page[i]._item->_animatedSprites.begin();
                while(it3 != _page[i]._item->_animatedSprites.end())
                {
                    window.draw(it3->second);
                    ++it3;
                }
            }
        }
    }
}

void List::clear()
{
    _active = false;

    for(unsigned int i = 0; i < _numPageElements; ++i)
    {
        _page[i]._active = false;
        _page[i]._item = nullptr;
    }

    _items.clear();
}