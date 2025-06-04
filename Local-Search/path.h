#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <vector>
#include <chrono>
#include <set>
#include <algorithm>
#include <fstream>
#include <random>
#include <utility>
#include <unordered_set>
#include <deque>
#include "readData.h"

using namespace std;
using matrixD = vector<vector<double>>;
using matrixI = vector<vector<int>>;

enum StopFactor
{
    NO_REMOVAL,
    TIME_LIMIT,
    ITERATION_LIMIT,
    COUNT_OCCURURANCE_LIMIT,
    ERROR
};

// make some changes for merging
class Path
{
public:
    const readData &data;
    vector<int> path;
    matrixD visited;
    matrixI cargo;
    double totalDist;
    double profit;
    double locSrcProfit;

    struct VectorHash
    {
        size_t operator()(const std::vector<int> &v) const
        {
            size_t hash = 0;
            for (int num : v)
            {
                hash = hash * 101 + static_cast<size_t>(num); // Include both element and order
            }
            return hash;
        }
    };

    // constructor
    Path(const readData &obj);
    // Copy constructor
    Path(const Path &obj);
    // Assignment operator
    Path &operator=(const Path &obj);

    double totalWeight() const;
    size_t size() const;
    bool isFound(int j) const;
    void push_back(int i);
    double calculateProfit();
    void printCargo(ostream &os) const;
    void printCargoComma(ostream &os) const;
    void printVisited(ostream &os) const;
    double insertSearch_Tran(fstream &textFile);
    double insertSearch_Tran_2();
    double insertSearch_Daniel();
    void fillCapacity();
    // double replaceSearch();
    Path replaceSearch();
    double fillCapacity_Daniel();
    Path deterministicAlgo(bool, bool);
    StopFactor disturbance(int, int, double, int &, bool, bool);
    void removeNode(int removeNodePosition, bool calProfit);
    Path removeNodeRandom(int numOfNodeToRemove);
    friend ostream &operator<<(std::ostream &os, const Path &obj);
    void resetPathButRemainVistedCargo();

private:
    double calculateProfit(const matrixD &visitParam, const matrixI &cargoParam, const double &distParam);
    double updateTotalDistance();
    pair<vector<int>, double> findMax(const int &pickup, const int &dropoff);
    Path findMax_2(const int &pickup, const int &dropoff);
    void fillCapacity_Insertion(Path &tempPath);
    double fillCapacity_Daniel(vector<int> path, matrix &visited, vector<vector<int>> &cargo, double &originProfit, double totalDist);
};

#endif