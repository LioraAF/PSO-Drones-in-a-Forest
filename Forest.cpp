//
// Created by liora on 5/30/2024.
//

#include "Forest.h"

// Static fields:
unsigned int Forest::MAX_X = 72;
unsigned int Forest::MAX_Y = 42;
unsigned int Forest::MIN_X = 0;
unsigned int Forest::MIN_Y = 0;
unsigned int Forest::NUM_CELLS = (Forest::MAX_X - Forest::MIN_X) * (Forest::MAX_Y - Forest::MIN_Y);


// Constructors:
Forest::Forest(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY, unsigned int maxIter, const FlyingPoint& FPtarget, DroneMap&& droneMap): maxIter(maxIter), droneMap(std::move(droneMap)),
                            forestCellMap(createForestCellMap()), target( &(forestCellMap[FPtarget.getForestCellID(MAX_X)])) {
    MIN_X = minX;
    MIN_Y = minY;
    MAX_X = maxX;
    MAX_Y = maxY;
    NUM_CELLS = (MAX_X-MIN_X) * (MAX_Y-MIN_Y);
    ForestCell::setGimelGimel(MIN_X,MIN_Y,MAX_X,MAX_Y);
}
Forest::Forest(unsigned int maxIter, const FlyingPoint& FPtarget, DroneMap&& droneMap): maxIter(maxIter), droneMap(std::move(droneMap)),
                                                                                        forestCellMap(createForestCellMap()), target( &(forestCellMap[FPtarget.getForestCellID(MAX_X)])) {
    NUM_CELLS = (MAX_X-MIN_X) * (MAX_Y-MIN_Y);
    ForestCell::setGimelGimel(MIN_X,MIN_Y,MAX_X,MAX_Y);
}
Forest::~Forest() { delete[] forestCellMap; }

// Methods:
ForestCell* Forest::createForestCellMap() {
    ForestCell* map = new ForestCell[NUM_CELLS];
    unsigned int i = 0;
    while (i<NUM_CELLS) {
        map[i] = ForestCell(i % (MAX_X-MIN_X), i / (MAX_X-MIN_X));
        i++;
    }
    return map;
}
void Forest::initForestCellMap() {
    droneMap.root->initForestCellMap(forestCellMap);
}
unsigned int Forest::runDroneSearch() {

    // Add drones to cellMap count:
    initForestCellMap();

    // Run:
    for (unsigned int j = 0; j < maxIter; j++) {
        droneMap.root->runDroneSearch(forestCellMap);
        if ((*target)() == 1)
            return j;
        droneMap.root->updateGlobalBest();
    }
    return maxIter;

}
unsigned int Forest::getDroneCount() const {
    return droneMap.count;
}
DroneMapNode* Forest::getDroneMapRoot() const {
    return droneMap.root;
}


// DroneMap functions
DroneMap::DroneMap(const char* fileName): root(nullptr), count(0), valid_file(true) {

    ifstream fin;
    std::string line;

    // Reading file 1 - target and max number of iterations
    fin.open(fileName);

    // Line1 - dronesCount
    getline(fin, line);
    int n=0;
    std::stringstream s1(line);
    int tempCount;
    while (s1 >> tempCount) {
        n++;
        // reports if there are inputs that aren't integers
        if (s1.fail()) {
            fin.close();
            valid_file = false;
            return;
        }
    }
    // reports if first line has more than one element
    if (n != 1) {
        fin.close();
        valid_file = false;
        return;
    }

    // Next lines - drones
    int i=1;
    while (getline(fin, line)) {
        // reports if there are more lines than expected drones
        if (i >= tempCount) {
            fin.close();
            valid_file = false;
            return;
        }
        std::stringstream s2(line);
        float inputNum;
        char droneChar;
        s2 >> droneChar;
        if (s2.fail() || (droneChar!='S' && droneChar!='M' && droneChar!='W' && droneChar!='H')) {
            fin.close();
            valid_file = false;
            return;
        }
        n = 0;
        float x, y, Vx, Vy;
        while (s2 >> inputNum) {
            n++;
            // reports inputs that aren't numbers
            if (n==4)
                if (s2.fail()) {
                    fin.close();
                    valid_file = false;
                    return;
                }
            switch (n) {
                case 1:
                    x = inputNum;
                    break;
                case 2:
                    y = inputNum;;
                    break;
                case 3:
                    Vx = inputNum;
                    break;
                case 4:
                    Vy = inputNum;
                    break;
                    // reports if drone line has more than four elements
                default:
                    fin.close();
                    valid_file = false;
            }
        }
        // reports if drone line has less than four elements
        if (n != 4) {
            fin.close();
            valid_file = false;
            return;
        }

        // adds drone to droneMap:
        Drone *drone;
        switch (droneChar) {
            case 'S':
                drone = new SingleRotor(x, y, Vx, Vy);
                break;
            case 'M':
                drone = new MultiRotor(x, y, Vx, Vy);
                break;
            case 'W':
                drone = new FixedWing(x, y, Vx, Vy);
                break;
            case 'H':
                drone = new Hybrid(x, y, Vx, Vy);
                break;
            default:
                fin.close();
                valid_file = false;
                return;
        }
        insert(*drone);
    }

    fin.close();
    valid_file = true;
}
DroneMap::DroneMap(DroneMap&& other) noexcept : root(other.root), count(other.count), valid_file(other.valid_file) {
    other.root = nullptr;
}
DroneMap::~DroneMap() { delete root; }

DroneMap::operator bool() const { return valid_file; }

void DroneMap::insert(Drone& drone) {
    if (isEmpty()) {
        root = new DroneMapNode(drone);
        count++;
    }
    else
        if (root->insert(drone))
            count++;
}
bool DroneMap::isEmpty() const { return root == nullptr; }
DroneMapNode* DroneMap::search(Drone const& drone) const {
    if (isEmpty())
        return nullptr;
    else
        return root->search(drone);
}
bool DroneMap::contains(Drone const& drone) const {
    if (search(drone) == nullptr)
        return false;
    return true;
}
DroneMapNode* DroneMap::min() { return root->min(); }
DroneMapNode* DroneMap::max() { return root->max(); }
void DroneMap::remove(Drone const& drone) {
    if (contains(drone)) {
        root = root->removeSearch(drone);
        count--;
    }
}
void DroneMap::clear() {
    delete root;
    root = nullptr;
    count = 0;
}
void DroneMap::print() const{
    if (!isEmpty()) {
        root->printInOrder();
    }
}
unsigned int DroneMap::size() const { return count; }


// DroneMapNode functions
DroneMapNode::DroneMapNode(Drone const& otherDrone): leftSon(nullptr), rightSon(nullptr), drone(std::unique_ptr<Drone>(otherDrone.clone())) {}        // make_unique - call new and wrap it
                                                                                                                                                        // unique_ptr - receives pointer and wraps it
DroneMapNode::DroneMapNode(DroneMapNode&& otherDrone) noexcept : leftSon(otherDrone.leftSon), rightSon(otherDrone.rightSon), drone(std::move(otherDrone.drone)){
    otherDrone.leftSon = nullptr;
    // other way to write this is: leftSon(std::exchange(other.leftSon, nullptr))
    otherDrone.rightSon = nullptr;

    // with std::move, now otherDrone.drone is a nullptr
}
DroneMapNode& DroneMapNode::operator=(DroneMapNode&& other) noexcept {
    leftSon = other.leftSon;
    rightSon = other.rightSon;
    drone = std::move(other.drone);
    return *this;
}
DroneMapNode::~DroneMapNode() {
    delete leftSon;
    delete rightSon;
}

bool DroneMapNode::operator < (Drone const& other) const { return *drone < other; }
bool DroneMapNode::operator < (DroneMapNode const& other) const { return *drone < *other.drone; }
bool DroneMapNode::operator > (Drone const& other) const { return other < *drone; }
bool DroneMapNode::operator > (DroneMapNode const& other) const { return *other.drone < *drone; }
std::ostream& operator << (ofstream& out, DroneMapNode dmn) {
    out << *dmn.drone;
    return out;
}

DroneMapNode* DroneMapNode::search(Drone const& droneToFind) {
    if (*this < droneToFind) {
        if (rightSon == nullptr)
            return nullptr;
        return rightSon->search(droneToFind);
    }
    if (*this > droneToFind){
        if (leftSon == nullptr)
            return nullptr;
        return leftSon->search(droneToFind);
    }
    else
        return this;
}
bool DroneMapNode::insert(Drone const& droneToInsert) {
    if (*this < droneToInsert) {
        if (rightSon == nullptr) {
            rightSon = new DroneMapNode(droneToInsert);
            return true;
        }
        else return (rightSon->insert(droneToInsert));
    }
    else if (*this > droneToInsert){
        if (leftSon == nullptr) {
            leftSon = new DroneMapNode(droneToInsert);
            return true;
        }
        else leftSon->insert(droneToInsert);
    }
    else {
        *this->drone = droneToInsert;
        return false;
    }
}
DroneMapNode* DroneMapNode::removeSearch(Drone const& droneToDel) {
    if (*this < droneToDel) {
        rightSon = rightSon->removeSearch(droneToDel);
    }
    if (*this > droneToDel) {
        leftSon = leftSon->removeSearch(droneToDel);
    }
    else
        return removeNode();
    return this;
}
DroneMapNode* DroneMapNode::removeNode() {
    if (leftSon==nullptr && rightSon==nullptr)
        return nullptr;
    else if (leftSon== nullptr)
        return rightSon;
    else if (rightSon== nullptr)
        return leftSon;
    else {
        drone = std::unique_ptr<Drone>(rightSon->findNextVal()->clone());
        rightSon = rightSon->deleteNextVal();
    }
    return this;
}
Drone* DroneMapNode::findNextVal() const {
    if (leftSon== nullptr)
        return drone.get();
    return leftSon->findNextVal();
}
DroneMapNode* DroneMapNode::deleteNextVal() {
    if (leftSon== nullptr)
        return rightSon;
    leftSon = leftSon->deleteNextVal();
    return this;
}
void DroneMapNode::printInOrder() const {
    if (leftSon!= nullptr)
        leftSon->printInOrder();
    cout << this << endl;
    if (rightSon!= nullptr)
        rightSon->printInOrder();
}
void DroneMapNode::printInOrder(ofstream& out) const {
    if (leftSon!= nullptr)
        leftSon->printInOrder(out);
    out << this->drone->toString() << endl;
    if (rightSon!= nullptr)
        rightSon->printInOrder(out);
}
DroneMapNode* DroneMapNode::min() {
    if (leftSon== nullptr)
        return this;
    else return leftSon->min();
}
DroneMapNode* DroneMapNode::max() {
    if (rightSon== nullptr)
        return this;
    else return rightSon->min();
}

void DroneMapNode::initForestCellMap(ForestCell* forestCellP) const {
    if (leftSon!= nullptr)
        leftSon->initForestCellMap(forestCellP);

    forestCellP[ForestCell::getCellid(this->drone->getFlyingPoint())]++;

    if (rightSon!= nullptr)
        rightSon->initForestCellMap(forestCellP);
}

// inOrder traversal, at this, lower the count of the previous cell that held the drone, calculate the new drone coordinates, update the personal best, up by one the number of drones in the new relevant forestCell
void DroneMapNode::runDroneSearch(ForestCell* forestCellP) const {
    if (leftSon!= nullptr)
        leftSon->runDroneSearch(forestCellP);

    forestCellP[ ForestCell::getCellid(this->drone->getFlyingPoint()) ]--;
    this->drone->runIteration(Forest::MIN_X, Forest::MIN_Y, Forest::MAX_X, Forest::MAX_Y);
    this->drone->updatePersonalBest();
    forestCellP[ ForestCell::getCellid(this->drone->getFlyingPoint()) ]++;

    if (rightSon!= nullptr)
        rightSon->runDroneSearch(forestCellP);
}
void DroneMapNode:: updateGlobalBest() const {
    if (leftSon!= nullptr)
        return leftSon->updateGlobalBest();

    this->drone->updateGlobalBest();

    if (rightSon!= nullptr)
        return rightSon->updateGlobalBest();
}

