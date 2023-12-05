# A* Pathfinding Algorithm in C++

## Overview

This repository contains a C++ implementation of the A* pathfinding algorithm. A* is a widely used algorithm for finding the shortest path between two points on a graph, considering both the cost to reach the point and a heuristic that estimates the cost from the point to the goal.

## Features

>- <h4> Efficient Pathfinding: </h4>  A* ensures an efficient and optimal pathfinding algorithm.
>- <h4> Configurable Heuristics: </h4> Users can easily customize the heuristic function according to the specific requirements of their application.
>- <h4> Flexible Graph Representation: </h4> The implementation supports various graph representations, allowing users to adapt it to different scenarios.

## Getting Started

### Prerequisites
C++ compiler (supporting C++11 or later)

<hr/>

### Usage

##### Clone the repository:
```console
git clone https://github.com/amandeepsirohi/A-Star-CPP.git
cd A-Star-CPP
```
##### Compile the code:
```console
g++ a_star.cpp -o astar
```
##### Run the executable:
```console
./astar
```

<hr/>

### Configuration

#### Heuristic Function
To customize the heuristic function, modify the heuristic function(calcHVal) located in a_star.cpp.
```console
double calcHVal(int row, int col, Pair dest)
{
    return ((double)sqrt((row - dest.first) * (row - dest.first) +
                         (col - dest.second) * (col - dest.second)));
}
```

