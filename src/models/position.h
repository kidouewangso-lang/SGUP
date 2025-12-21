#ifndef POSITION_H
#define POSITION_H

#include <string>
#include <cmath>

class Position {
private:
    double x;
    double y;

public:
    Position(double x = 0, double y = 0);
    
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    
    std::string toString() const;
    double distanceTo(const Position& other) const;
    
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    
    Position operator+(const Position& other) const;
    Position operator-(const Position& other) const;
};

#endif