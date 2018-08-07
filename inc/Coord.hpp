#ifndef COORD_HPP
#define COORD_HPP

class Coord
{
public:

    int x;
    int y;

    Coord() {}

    Coord(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    
    ~Coord() {}

    Coord operator+(const Coord& c) const
    {
        return Coord(x + c.x, y + c.y);
    }

    Coord operator*(int e) const
    {
        return Coord(x * e, y * e);
    }

    bool operator== (const Coord& c) const
    {
        return (x == c.x && y == c.y);
    }

};

#endif