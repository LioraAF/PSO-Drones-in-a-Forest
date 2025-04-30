//
// Created by liora on 5/31/2024.
//

#ifndef HW2_FORESTCELL_H
#define HW2_FORESTCELL_H
#include "Drone.h"

class ForestCell {

private:
    unsigned int lowX;
    unsigned int highX;
    unsigned int lowY;
    unsigned int highY;
    unsigned int dronesCount;
    static unsigned int MAX_X;
    static unsigned int MAX_Y;
    static unsigned int MIN_X;
    static unsigned int MIN_Y;

public:
    ForestCell();
    ForestCell(unsigned int x, unsigned int y);         // <-- lower coordinates
    ForestCell(const ForestCell& other);
    ForestCell(ForestCell&& other) noexcept;
    ForestCell& operator=(const ForestCell& other);
    ForestCell& operator=(ForestCell&& other) noexcept;
    ForestCell& operator--();
    ForestCell& operator++();
    const ForestCell operator--(int);
    const ForestCell operator++(int);
    unsigned int operator()() const;
    static void setGimelGimel(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY);
    ~ForestCell() = default;

    static unsigned int getCellid(unsigned int x, unsigned int y);
    static unsigned int getCellid(const FlyingPoint& fp);
    unsigned int getCellid() const;
    unsigned int getDronesCount() const;
};

#endif //HW2_FORESTCELL_H
