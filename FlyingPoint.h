//
// Created by liora on 5/30/2024.
//

#ifndef HW2_FLYINGPOINT_H
#define HW2_FLYINGPOINT_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;


class FlyingPoint {

private:
    double x;
    double y;
    double Vx;
    double Vy;

public:
    FlyingPoint();
    FlyingPoint(double x, double y, double Vx=0, double Vy=0);
    FlyingPoint(const FlyingPoint& other);
    FlyingPoint(FlyingPoint&& other) noexcept;
    FlyingPoint& operator=(const FlyingPoint& other);
    FlyingPoint& operator=(FlyingPoint&& other) noexcept ;
    ~FlyingPoint() = default;

    void setXandY(double x, double y);
    void setVxandVy(double vx, double vy);
    double getX() const;
    double getY() const;
    double calcDistance(const FlyingPoint& other) const;
    unsigned int getForestCellID(unsigned int MAX_X) const;

    friend class Drone;
};

#endif //HW2_FLYINGPOINT_H
