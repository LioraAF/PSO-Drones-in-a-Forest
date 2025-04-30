# Drone Simulation in C++  
Simulates autonomous drones navigating a forest environment in search of a target, using object-oriented design and Particle Swarm Optimization (PSO)-inspired updates.

## Features
- PSO-based position updates using personal and global bests
- Multiple drone types: Single Rotor, Multi Rotor, Fixed Wing, Hybrid VTOL
- Object-oriented design: inheritance, virtual functions, and encapsulation
- Operator overloading (`++`, `--`, function call)
- Custom binary search tree for managing drone data
- Manual memory management (no STL used)

## Structure
This project was implemented across two assignments:

### ex2 – Drone Behavior & Tracking
- Simulates drone movement and target search
- Tracks personal/global bests
- Computes distances and updates drone state
- Counts active drones and forest cell occupancy

### ex3 – Drone Types & BST
- Introduces multiple drone types using inheritance
- Implements binary search tree to store and retrieve drones
- Extends forest to support polymorphic drone management

## 🚀 How to Compile
```bash
g++ main.cpp Forest.cpp ForestCell.cpp Drone.cpp FlyingPoint.cpp -o mysh
The program expects 3 or 4 input files as arguments:

./mysh <file1> <file2> <file3> [file4]
🔹 Arguments:
file1 – Forest boundaries, target coordinates, and max iterations
file2 – Drone definitions
file3 – Output file (where results will be written)
file4 (optional) – Random seed input

