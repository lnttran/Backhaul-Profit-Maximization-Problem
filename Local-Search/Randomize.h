#ifndef RANDOMIZE_H
#define RANDOMIZE_H
#include <numeric> // For std::iota
#include <map>
#include <time.h>
#include <cstdlib>
#include <random>
#include "Algorithm.h"
#include "path.h"

class Randomize : public Algorithm
{
private:
public:
    Randomize(const readData &obj) : Algorithm(obj)
    {
    }
    void runRandomized(fstream &, const string &, string, bool, int, int);
    Path greedyRandomized(vector<multimap<double, int>> &, int, int);
    Path rouletteSelection(vector<multimap<double, int>> &, int, int);

private:
    void recordText(fstream &, const string &, string, const Path &, int);
    void recordData(fstream &, const string &, string, string, const Path &, const double &, const StopFactor &, const int &, double, double);
};

#endif
