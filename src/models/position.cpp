#include "Position.h"
#include <sstream>

Position::Position(double x, double y) : x(x), y(y) {}

double Position::getX() const { return x; }
double Position::getY() const { return y; }

void Position::setX(double x) { this->x = x; }
void Position::setY(double y) { this->y = y; }

std::string Position::toString() const {
    std::stringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}

double Position::distanceTo(const Position& other) const {
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}

bool Position::operator==(const Position& other) const {
    return (x == other.x && y == other.y);
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

Position Position::operator+(const Position& other) const {
    return Position(x + other.x, y + other.y);
}

Position Position::operator-(const Position& other) const {
    return Position(x - other.x, y - other.y);
}