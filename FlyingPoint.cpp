//
// Created by liora on 5/30/2024.
//

#include "FlyingPoint.h"

// Constructors:
FlyingPoint::FlyingPoint(): FlyingPoint(0,0,0,0) {}
FlyingPoint::FlyingPoint(double x, double y, double Vx, double Vy): x(x), y(y), Vx(Vx), Vy(Vy) {}
FlyingPoint::FlyingPoint(const FlyingPoint& other) = default;
FlyingPoint::FlyingPoint(FlyingPoint&& other) noexcept : x(other.x), y(other.y), Vx(other.Vx), Vy(other.Vy) {}
FlyingPoint& FlyingPoint::operator=(const FlyingPoint& other) = default;
FlyingPoint& FlyingPoint::operator=(FlyingPoint&& other) noexcept {
    x = other.x;
    y = other.y;
    Vx = other.Vx;
    Vy = other.Vy;
    return *this;
}


// Methods:
void FlyingPoint::setXandY(double newX, double newY) {
    x = newX;
    y = newY;
}
void FlyingPoint::setVxandVy(double vx, double vy) {
    this->Vx = vx;
    this->Vy = vy;
}
double FlyingPoint::getX() const { return x; }
double FlyingPoint::getY() const { return y; }
double FlyingPoint::calcDistance(const FlyingPoint& other) const { return sqrt( pow(x - other.x , 2) + pow(y - other.y , 2) ); }
unsigned int FlyingPoint::getForestCellID(unsigned int MAX_X) const {
    return (((unsigned int)x))*MAX_X + ((unsigned int)y);
}