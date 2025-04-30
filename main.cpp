
#include "Forest.h"

void printInvalidInput();
bool file1handler(const char* fileName, unsigned int& maxIters);
void file3writer(const char* fileName, unsigned int numIters, Forest& forest);
unsigned int input4handler(const char* input);

template <class T>
T parse(const std::string& s)
{
    T out;
    std::stringstream ss(s);
    ss >> out;
    return out;
}


int main(int argc, const char* argv[]) {

    // Receiving files
    if (argc !=4 && argc !=5) {
        printInvalidInput();
        return 1;
    }

    // Reading file 1 - target and max number of iterations
    unsigned int maxIterations;
    if (!file1handler(argv[1], maxIterations)) {
        printInvalidInput();
        return 1;
    }

    // Reading file 2 - drones
    DroneMap droneMap(argv[2]);
    if (!(bool)droneMap) {
        printInvalidInput();
        return 1;
    }

    // Reading file 4 - sRand
    if (argc==5)
        Drone::setSeed(input4handler(argv[5]));

    // Run drone search:
    Forest forest(maxIterations, Drone::getTargetFP(), std::move(droneMap));
    unsigned int numIters = forest.runDroneSearch();

    // Writing results to file 3
    file3writer(argv[3], numIters, forest);

    return 0;
}



bool file1handler(const char* fileName, unsigned int& maxIters) {
    ifstream fin;
    std::string line;

    // Reading file 1 - target and max number of iterations
    fin.open(fileName);

    // Line1 - gimel gimel (forest boundaries)
    getline(fin, line);
    int n=0;
    int temp;
    std::stringstream s1(line);
    while (s1 >> temp) {
        n++;
        // reports inputs that aren't numbers
        if (n==4)
            if (s1.fail()) {
                fin.close();
                return false;
            }
        switch (n) {
            case 1:
                Forest::MIN_X = temp;
                break;
            case 2:
                Forest::MIN_Y = temp;
                break;
            case 3:
                Forest::MAX_X = temp;
                if (Forest::MAX_X <= Forest::MIN_X) {
                    fin.close();
                    return false;
                }
                break;
            case 4:
                Forest::MAX_Y = temp;
                if (Forest::MAX_Y <= Forest::MIN_Y) {
                    fin.close();
                    return false;
                }
                break;
            default:
                break;
        }
    }
    // reports if first line has more or less than four elements
    if (n!=4) {
        fin.close();
        return false;
    }

    // Line2 - target
    getline(fin, line);
    n=0;
    double* targetValue = new double[2];
    std::stringstream s2(line);
    while (s2 >> targetValue[n]) {
        n++;
        // reports inputs that aren't numbers
        if (n==2)
            if (s2.fail()) {
                fin.close();
                return false;
            }
    }
    // reports if the second line has more or less than two elements
    if (n!=2) {
        fin.close();
        return false;
    }
    // reports if target it outside of forest boundaries
    if (targetValue[0] < Forest::MIN_X || targetValue[0] > Forest::MAX_X || targetValue[1] < Forest::MIN_Y || targetValue[1] > Forest::MAX_Y) {
        fin.close();
        return false;
    }
    FlyingPoint target = FlyingPoint(targetValue[0], targetValue[1]);
    Drone::setTargetFP(target);

    // Line3 - max number of iterations
    getline(fin, line);
    std::stringstream s3(line);
    n=0;
    while (s3 >> maxIters) {
        n++;
        // reports inputs that aren't numbers
        if (n==1)
            if (s3.fail()) {
                fin.close();
                return false;
            }
    }
    // reports if first line has more than one element
    if (n!=1) {
        fin.close();
        return false;
    }

    fin.close();
    return true;
}
void file3writer(const char* fileName, unsigned int numIters, Forest& forest) {
    ofstream fout;
    fout.open(fileName);

    fout << numIters << endl;
    forest.getDroneMapRoot()->printInOrder(fout);

    fout.close();
}
unsigned int input4handler(const char* input) {
    int numInt = parse<int>(input);
    double doubleInt = parse<double>(input);
    if ( numInt == doubleInt)
        return numInt;
    else return (unsigned)time(nullptr);
}

void printInvalidInput() {
    std::cerr << "Error; invalid input" << endl;
}






