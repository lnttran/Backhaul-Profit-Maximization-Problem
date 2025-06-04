#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include "../readData.h"
using matrix = vector<vector<double>>;

using namespace std;

const double INF = numeric_limits<double>::infinity();

class Shortest_path
{
public:
    // Declare private attributes of Algorithm class here
    int numOfNode;
    int travelCost;
    int totalCapacity;
    int DIS;
    double vehicleWeight;
    double priceCharged;
    matrix w;
    matrix d;
    const readData &data;

public:
    // Constructor to copy values from readData class
    Shortest_path(const readData &obj) : data(obj)
    {
        numOfNode = obj.numOfNode;
        travelCost = obj.travelCost;
        totalCapacity = obj.totalCapacity;
        DIS = obj.DIS;
        vehicleWeight = obj.vehicleWeight;
        priceCharged = obj.priceCharged;
        w = obj.w;
        d = obj.d;
    }

    void dijkstra(int start, string filename);
};

// Dijkstra's Algorithm

#endif