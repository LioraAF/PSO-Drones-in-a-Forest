//
// Created by liora on 5/30/2024.
//

#ifndef HW2_DRONE_H
#define HW2_DRONE_H
#include "FlyingPoint.h"

class Drone {
private:
    static unsigned int numOfDrones;
    static FlyingPoint GLOBAL_BEST;
    static FlyingPoint targetFP;
    static unsigned int seed;

    FlyingPoint PERSONAL_BEST;
    FlyingPoint currFP;
    unsigned int serialNum;

public:
    Drone();
    Drone(double x, double y, double Vx, double Vy);
    Drone(const Drone& other) = default;
    Drone(Drone&& other) noexcept = default;
    Drone& operator=(const Drone& other) = default;
    Drone& operator=(Drone&& other) noexcept = default;
    virtual Drone* clone() const = 0;
    virtual ~Drone() = default;

    // drones are compared by serial number which they receive by order as they are added to the forest
    bool operator < (Drone const& other) const;
    friend std::ostream& operator << (ofstream& out, Drone& d);

    double calcX() const;
    double calcY() const;
    double calcVyBase(double alpha, double beta, double gama) const;
    double calcVxBase(double alpha, double beta, double gama) const;
    virtual double calcVx() const = 0;
    virtual double calcVy() const = 0;

    static void setSeed(unsigned int s);
    unsigned int getSerialNum() const;
    virtual char getDroneChar() const = 0;
    static void setTargetFP(FlyingPoint& fp);
    static FlyingPoint& getTargetFP();
    void runIteration(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY);
    void setCurrFlyingPoint(double x, double y, double vx, double vy);
    FlyingPoint& getFlyingPoint();
    double currDistanceFromTarget();
    double PersonalBestDistanceFromTarget();
    double static GlobalBestDistanceFromTarget();
    void updatePersonalBest();
    void updateGlobalBest();
    virtual std::string toString() const;

    friend double rand0to1();
};


// Helping Methods:
double rand0to1();


class SingleRotor: virtual public Drone {

public:
    SingleRotor(double x, double y, double Vx, double Vy);
    SingleRotor* clone() const override;
    double calcVx() const override;
    double calcVy() const override;
    char getDroneChar() const override;
};

class MultiRotor: virtual public Drone {

public:
    MultiRotor(double x, double y, double Vx, double Vy);
    MultiRotor* clone() const override;
    double calcVx() const override;
    double calcVy() const override;
    char getDroneChar() const override;
};

class FixedWing: virtual public Drone {

public:
    FixedWing(double x, double y, double Vx, double Vy);
    FixedWing* clone() const override;
    double calcVx() const override;
    double calcVy() const override;
    char getDroneChar() const override;
};

class Hybrid: public SingleRotor, public FixedWing{

public:
    Hybrid(double x, double y, double Vx, double Vy);
    Hybrid* clone() const override;
    double calcVx() const override;
    double calcVy() const override;
    char getDroneChar() const override;
};


#endif //HW2_DRONE_H
