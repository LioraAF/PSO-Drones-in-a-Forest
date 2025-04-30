//
// Created by liora on 5/30/2024.
//

#ifndef HW2_FOREST_H
#define HW2_FOREST_H
#include "ForestCell.h"
#include <memory>

struct DroneMapNode {
    DroneMapNode* leftSon;
    DroneMapNode* rightSon;
    std::unique_ptr<Drone> drone;           // pointer which deletes the pointed-to content when it dies
                                            // practicing the drone field with std, the rest are normal pointers

    explicit DroneMapNode(Drone const& otherDrone);
    DroneMapNode(DroneMapNode const& other) = delete;               // copyConstructor - not allowed to prevent double pointers to sons (or otherwise copying the entire subtree)
    DroneMapNode(DroneMapNode&& other) noexcept;                             // moveConstructor
    DroneMapNode& operator=(DroneMapNode const& other) = delete;    // copyAssignment - not allowed for same reason
    DroneMapNode& operator=(DroneMapNode&& other) noexcept ;                  // moveAssignment
    ~DroneMapNode();

    bool operator < (Drone const& other) const;
    bool operator < (DroneMapNode const& other) const;
    bool operator > (Drone const& other) const;
    bool operator > (DroneMapNode const& other) const;
    friend std::ostream& operator << (ofstream& out, DroneMapNode dmn);

    DroneMapNode* search(Drone const& drone);
    bool insert(Drone const& drone);
    DroneMapNode* removeSearch(Drone const& drone);
    DroneMapNode* removeNode();
    Drone* findNextVal() const;
    DroneMapNode* deleteNextVal();
    void printInOrder() const;
    void printInOrder(ofstream& out) const;
    DroneMapNode* min();
    DroneMapNode* max();

    void initForestCellMap(ForestCell* forestCellP) const;
    void runDroneSearch(ForestCell* forestCellP) const;
    void updateGlobalBest() const;
};


struct DroneMap {
    DroneMapNode* root;
    unsigned int count;
    bool valid_file;

    explicit DroneMap(const char* fileName);
    explicit DroneMap(DroneMap const&) = delete;
    DroneMap(DroneMap&&) noexcept ;
    DroneMap& operator=(DroneMap const&) = delete;
    DroneMap& operator=(DroneMap&&) = delete;
    ~DroneMap();
    explicit operator bool() const;

    void insert(Drone& drone);
    bool isEmpty() const;
    DroneMapNode* search(Drone const& drone) const;
    bool contains(Drone const& drone) const;
    DroneMapNode* min();
    DroneMapNode* max();
    void remove(Drone const& drone);
    void clear();
    void print() const;
    unsigned int size() const;
};


class Forest {
private:
    const unsigned int maxIter;
    DroneMap droneMap;
    ForestCell* forestCellMap;
    ForestCell* target;
public:
    static unsigned int NUM_CELLS;
    static unsigned int MAX_X;
    static unsigned int MAX_Y;
    static unsigned int MIN_X;
    static unsigned int MIN_Y;

public:
    Forest() = delete;
    Forest(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY, unsigned int maxIter, const FlyingPoint& FPtarget, DroneMap&& droneMap);
    Forest(unsigned int maxIter, const FlyingPoint& FPtarget, DroneMap&& droneMap);
    Forest(const Forest& other) = delete;
    Forest& operator=(const Forest& other) = delete;
    Forest& operator=(Forest&& other) = delete;
    ~Forest();

    unsigned int runDroneSearch();
    unsigned int getDroneCount() const;
    DroneMapNode* getDroneMapRoot() const;
private:
    static ForestCell* createForestCellMap();
    void initForestCellMap();
};




#endif //HW2_FOREST_H
