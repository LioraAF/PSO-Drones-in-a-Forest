//
// Created by liora on 5/31/2024.
//

#include "ForestCell.h"

// Static fields:
unsigned int ForestCell::MAX_X = 72;
unsigned int ForestCell::MAX_Y = 42;
unsigned int ForestCell::MIN_X = 0;
unsigned int ForestCell::MIN_Y = 0;


// Constructors:
ForestCell::ForestCell(): ForestCell(0,0) {}
ForestCell::ForestCell(unsigned int x, unsigned int y): lowX(x), highX(x+1), lowY(y), highY(y+1), dronesCount(0) {}
ForestCell::ForestCell(const ForestCell& other) = default;
ForestCell::ForestCell(ForestCell&& other) noexcept : lowX(other.lowX), highX(other.highX), lowY(other.lowY), highY(other.highY), dronesCount(other.dronesCount) {}
ForestCell& ForestCell::operator=(const ForestCell& other) = default;
ForestCell& ForestCell::operator=(ForestCell&& other) noexcept {
    lowX = other.lowX;
    lowY = other.lowY;
    highX = other.highX;
    highY = other.highY;
    dronesCount = other.dronesCount;
    return *this;
}
ForestCell& ForestCell::operator--() { this->dronesCount--; return *this; }
ForestCell& ForestCell::operator++() { this->dronesCount++; return *this; }
const ForestCell ForestCell::operator--(int) {
    ForestCell temp = *this;
    this->dronesCount--;
    return temp;
}
const ForestCell ForestCell::operator++(int) {
    ForestCell temp = *this;
    this->dronesCount++;
    return temp;
}
unsigned int ForestCell::operator() () const { return dronesCount; }
void ForestCell::setGimelGimel(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY) {
    MIN_X = minX;
    MIN_Y = minY;
    MAX_X = maxX;
    MAX_Y = maxY;
}


// Methods:
unsigned int ForestCell::getCellid() const {
    return (lowX-1)*(MAX_X-MIN_X) + lowY-1;
}
unsigned int ForestCell::getCellid(unsigned int x, unsigned int y) {
    return x*(MAX_X-MIN_X) + y;
}
unsigned int ForestCell::getCellid(const FlyingPoint& fp) { return getCellid((int)fp.getX(), (int)fp.getY()); }
unsigned int ForestCell::getDronesCount() const { return dronesCount; }

