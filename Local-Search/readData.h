#ifndef READDATA_H
#define READDATA_H

#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <filesystem>
#include <optional>
using namespace std;
using matrix = vector<vector<double>>;

class readData
{

public:
    // the weight in tones of a load that a potential customer would like to ship
    // from node i to node j as w(ij)
    // the vehicle weight v tons when empty
    // carrying capacity of Q tons
    // incurs a travel cost of c dollars per mile per ton
    // empty vehicle incurs a cost of c v d(ij) when traversing arc(i,j)
    // where as fully loaded vehicle incurs a cost of c(v+Q) d(ij)
    // the customer pays p d w(ij) where p is the price charged (revenue received)
    // in dollar per mile per ton
    // DIS

    int numOfNode = 0;
    int travelCost = 0;
    int totalCapacity = 0;
    int DIS = 0;
    double vehicleWeight = 0;
    double priceCharged = 0;
    double optimalProfit = 2;
    matrix w;
    matrix d;

    void readSingleFile(const string &filename, optional<unordered_map<string, double>> optimalProfits = nullopt);
    void printStats();
    void printData();
    void getOptimalProfit(const string &path, unordered_map<string, double> optimalProfits);
    unordered_map<string, double> getOptimalProfitMap(const string &path);

private:
    vector<string> splitString(const string &input, char delimiter);
    static void printPathInfo(double &totalWeight, double &currDist, bool distRatio, vector<int> path, int greedyNum);
    string trimFileName(const string &filename);
};

#endif