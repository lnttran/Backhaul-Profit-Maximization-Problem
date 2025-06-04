#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include "readData.h"
#include <ctime>  //to measure CPU time
#include <chrono> //to measure run time
#include "path.h"
#include <cstring>
#include <string>
#include <sstream>
#include <set>
#include <unordered_map>
#include <optional>

using namespace std;
using namespace std::chrono;

using matrix = vector<vector<double>>;

class Algorithm
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
    Path maxPath;
    bool hasExistingPath = false;
    optional<Path> existingPath;

public:
    // Constructor to copy values from readData class
    Algorithm(const readData &obj) : data(obj), maxPath(obj)
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

    // Copy constructor
    Algorithm(const readData &obj, const Path &path)
        : data(obj), maxPath(obj), existingPath(path), hasExistingPath(true)
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

    Path GreedyAlgo1(fstream &, const string &, bool, bool, bool, bool, bool, int);
    Path GreedyAlgo1_b(fstream &, const string &, bool, bool, bool, bool, bool, int);
    Path GreedyAlgo2_b(fstream &, const string &, bool, bool, bool, bool, int);
    Path GreedyAlgo2_c(fstream &, const string &, bool, bool, bool, int);
    Path GreedyAlgo3(fstream &, const string &, bool, bool, bool, bool, bool, int);
    Path GreedyAlgo4(fstream &, const string &, bool, bool, bool, int);
    double errorToOptimal(double profit) const;
    Path getOptimalSolutionFromRunningAllAlgos(std::fstream &csvFile, const std::string &filename,
                                               bool insert, bool replace, bool localSearch, int maxIter);

private:
    void recordData(fstream &, const string &, const string &, const double &, const StopFactor &, const int &, const Path &, const double &, const double &);
    static void printPathInfo(double &, double &, bool, vector<int>, vector<vector<int>>, double);
    void recordText(fstream &, const string &, string, const Path &, double);
    std::vector<std::string> split(std::string, std::string);
    void updateMaxPath(const Path &path);
};

#endif