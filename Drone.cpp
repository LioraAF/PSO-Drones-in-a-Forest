//
// Created by liora on 5/30/2024.
//

#include <iomanip>
#include "Drone.h"

// Static fields:
FlyingPoint Drone::GLOBAL_BEST = FlyingPoint();
FlyingPoint Drone::targetFP = FlyingPoint();
unsigned int Drone::numOfDrones = 0;
unsigned int Drone::seed = (unsigned)time(nullptr);

// Constructors:
Drone::Drone(): Drone(0,0,0,0) {}
Drone::Drone(double x, double y, double Vx, double Vy): PERSONAL_BEST(FlyingPoint(x,y,Vx,Vy)), currFP(x,y,Vx,Vy), serialNum(numOfDrones++){}

// Methods:
std::ostream& operator << (ofstream& out, Drone& d) {
    out << d.toString();
    return out;
}
bool Drone::operator<(Drone const& other) const { return this->getSerialNum() < other.getSerialNum(); }

double Drone::calcX() const { return currFP.x + currFP.Vx; }
double Drone::calcY() const { return currFP.y + currFP.Vy; }
double Drone::calcVyBase(double alpha, double beta, double gama) const { return (alpha*currFP.Vx + beta*rand0to1()*(PERSONAL_BEST.x-currFP.x) + gama*rand0to1()*(GLOBAL_BEST.x-currFP.x)); }
double Drone::calcVxBase(double alpha, double beta, double gama) const { return (alpha*currFP.Vy + beta*rand0to1()*(PERSONAL_BEST.y-currFP.y) + gama*rand0to1()*(GLOBAL_BEST.y-currFP.y)); }
//double Drone::calcVx() const { return (0.25*currFP.Vx + rand0to1()*(PERSONAL_BEST.x-currFP.x) + rand0to1()*(GLOBAL_BEST.x-currFP.x)); }
//double Drone::calcVx(double alpha, double beta, double gama) const { return (alpha*currFP.Vx + beta*rand0to1()*(PERSONAL_BEST.x-currFP.x) + gama*rand0to1()*(GLOBAL_BEST.x-currFP.x)); }
//double Drone::calcVy() const { return (0.25*currFP.Vy + rand0to1()*(PERSONAL_BEST.y-currFP.y) + rand0to1()*(GLOBAL_BEST.y-currFP.y)); }
//double Drone::calcVy(double alpha, double beta, double gama) const { return (alpha*currFP.Vy + beta*rand0to1()*(PERSONAL_BEST.y-currFP.y) + gama*rand0to1()*(GLOBAL_BEST.y-currFP.y)); }

void Drone::setSeed(unsigned int s) {
    seed = s;
}
// runIteration - calculates new coordinates, makes sure they are withing range, and sets currFlyingPoint
void Drone::runIteration(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY) {
    double x=calcX(), y=calcY(), Vx=calcVx(), Vy=calcVy();
    if (x>maxX)
        x=maxX;
    else if (x<minX)
        x=minX;
    if (y>maxY)
        y=maxY;
    else if (y<minY)
        y=minY;
    setCurrFlyingPoint(x,y,Vx,Vy);
}
unsigned int Drone::getSerialNum() const {return serialNum;}
void Drone::setTargetFP( FlyingPoint& fp ) { targetFP = fp; }
FlyingPoint& Drone::getTargetFP() { return targetFP;}
void Drone::setCurrFlyingPoint(double x, double y, double vx, double vy) {
    currFP.setXandY(x,y);
    currFP.setVxandVy(vx, vy);
}
FlyingPoint& Drone::getFlyingPoint() {
    return currFP;
}
double Drone::currDistanceFromTarget() {
    return currFP.calcDistance(targetFP);
}
double Drone::PersonalBestDistanceFromTarget() {
    return PERSONAL_BEST.calcDistance(targetFP);
}
void Drone::updatePersonalBest() {
    if ( currDistanceFromTarget() < PersonalBestDistanceFromTarget() )
        PERSONAL_BEST = currFP;
}
double Drone::GlobalBestDistanceFromTarget() {
    return GLOBAL_BEST.calcDistance(targetFP);
}
void Drone::updateGlobalBest() {
    if ( PersonalBestDistanceFromTarget() < GlobalBestDistanceFromTarget() )
        GLOBAL_BEST = PERSONAL_BEST;
}
std::string Drone::toString() const {
    std::stringstream ss;
    ss << getDroneChar() << ' ' << (std::round(currFP.x * 100))/100.0 << ' ' << (std::round(currFP.y * 100))/100.0;
    std::string droneString = std::move(ss).str();
    return droneString;
}


// Helping Methods:
double rand0to1() {
    srand(Drone::seed);
    return ((double)rand()/(double)RAND_MAX);
}


// SubClasses:
SingleRotor::SingleRotor(double x, double y, double Vx, double Vy): Drone(x,y,Vx,Vy) {}
SingleRotor* SingleRotor::clone() const {
    return new SingleRotor(*this);
}
char SingleRotor::getDroneChar() const { return 'S';}
double SingleRotor::calcVx() const { return calcVxBase(0.01, -0.1, -0.25); }
double SingleRotor::calcVy() const { return calcVyBase(0.01, -0.1, -0.25); }

MultiRotor::MultiRotor(double x, double y, double Vx, double Vy): Drone(x,y,Vx,Vy) {}
MultiRotor* MultiRotor::clone() const {
    return new MultiRotor(*this);
}
char MultiRotor::getDroneChar() const { return 'M';}
double MultiRotor::calcVx() const { return calcVxBase(0.05, 0.1, 0); }
double MultiRotor::calcVy() const { return calcVyBase(0.05, 0.1, 0); }

FixedWing::FixedWing(double x, double y, double Vx, double Vy): Drone(x,y,Vx,Vy) {}
FixedWing* FixedWing::clone() const {
    return new FixedWing(*this);
}
char FixedWing::getDroneChar() const { return 'W';}
double FixedWing::calcVx() const { return calcVxBase(0.25, 1, 1); }
double FixedWing::calcVy() const { return calcVyBase(0.25, 1, 1); }

Hybrid::Hybrid(double x, double y, double Vx, double Vy): Drone(x,y,Vx,Vy), SingleRotor(x,y,Vx,Vy), FixedWing(x,y,Vx,Vy) {}
Hybrid* Hybrid::clone() const {
    return new Hybrid(*this);
}
char Hybrid::getDroneChar() const { return 'H';}
double Hybrid::calcVx() const { return SingleRotor::calcVx(); }
double Hybrid::calcVy() const { return SingleRotor::calcVy(); }
