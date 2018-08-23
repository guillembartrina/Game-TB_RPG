#ifndef LIST_HPP
#define LIST_HPP

#include <list>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "AnimatedSprite.hpp"

const int maxW = 200;
const int maxH = 100;

struct Item
{
    unsigned int _id;
    bool _picked;

    std::list<std::pair<sf::Vector2f, sf::Text>> _texts;
    std::list<std::pair<sf::Vector2f, sf::Sprite>> _sprites;
    std::list<std::pair<sf::Vector2f, AnimatedSprite>> _animatedSprites;

    Item();

    void addText(sf::Vector2f coord, sf::Text text);
    void addSprite(sf::Vector2f coord, sf::Sprite sprite);
    void addAnimatedSprite(sf::Vector2f coord, AnimatedSprite animatedSprite, std::string animationName);
};

struct Element
{
    bool _active;
    bool _selected;

    Item* _item;

    sf::RectangleShape rs_background;

    Element();

    void setItem(Item& item);
};

class List
{
public:

    List();
    List(sf::FloatRect listRect, int numPageElements, unsigned int numPickableItems = 0);
    ~List();

    void add(Item& item);

    void up();
    void down();
    void pick();

    Item current();

    bool allPicked();
    std::list<std::list<Item>::iterator>& getPicked();

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void clear();

private:

    sf::FloatRect _listRect;

    sf::Color c_rectOutline;
    sf::Color c_element;
    sf::Color c_elementOutline;
    sf::Color c_pickedElement;
    sf::Color c_pickerOutline;

    sf::RectangleShape rs_background;

    bool _active;

    unsigned int _numPageElements;
    unsigned int _currentPageElement;
    std::list<Item>::iterator _currentItem;
    
    std::vector<Element> _page;

    std::list<Item> _items;

    bool _pickableItems;
    unsigned int _numPickableItems;

    std::list<std::list<Item>::iterator> _pickedItems;

};

#endif