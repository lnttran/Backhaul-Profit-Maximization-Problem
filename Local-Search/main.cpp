#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <climits>
#include <set>
#include <string>
#include <ctime>
#include <chrono>
#include "readData.h"
#include "Algorithm.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    cout << argc << endl;
    for (int i = 1; i < argc; ++i)
        cout << argv[i] << endl;

    readData route;
    unordered_map<string, double> optimalProfits = route.getOptimalProfitMap(argv[3]);

    // unordered_map<string, double> optimalProfits = route.getOptimalProfitMap("sparse_data_20%/optimalProfit.csv");
    // unordered_map<string, double> optimalProfits = route.getOptimalProfitMap("/users7/cse/tnlam/SummerProject2023/Local-Search/BPMP_Data_Files/optimalProfit.csv");

    fstream csvFile(argv[2], ios::trunc | ios::out); // name of the output file
    // fstream csvFile("test10.csv", ios::out); // debug purpose

    if (!csvFile)
    {
        std::cerr << "Error opening a file. hi" << std::endl;
        return 1;
    }

    // csvFile << "Filename, Type of Algorithm, Profit, Profit after Local Search, Optimal Profit, Error, CPU Time(ms), Run Time(ms), Route, Requests" << endl;
    csvFile << "Filename, Type of Algorithm, Profit, Profit after Local Search, Optimal Profit, Error, sizeOfPathSet, StopFactor, CPU Time(ms), Run Time(ms), Route, Requests" << endl;

    auto it = filesystem::directory_iterator(argv[1]);
    // auto it = filesystem::directory_iterator("/users7/cse/tnlam/SummerProject2023/Local-Search/BPMP_Data_Files/80_nodes");

    for (const auto &entry : it)
    {
        filesystem::path path = entry.path();
        if (entry.is_regular_file())
        {
            string filename = path.string();
            route.readSingleFile(filename, optimalProfits);
            Algorithm algo(route);
            bool localSearch = argv[4];
            bool insert = argv[5];
            bool replace = argv[6];
            // int maxIter = argv[6];
            // bool localSearch = false; // debug purpose
            // bool insert = true;
            // bool replace = true;
            int maxIter = route.numOfNode;

            algo.GreedyAlgo1(csvFile, filename, false, false, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo1(csvFile, filename, true, false, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo1(csvFile, filename, false, true, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo1_b(csvFile, filename, false, false, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo1_b(csvFile, filename, true, false, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo2_b(csvFile, filename, false, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo2_b(csvFile, filename, true, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo2_c(csvFile, filename, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo3(csvFile, filename, true, false, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo3(csvFile, filename, false, true, insert, replace, localSearch, maxIter);
            algo.GreedyAlgo4(csvFile, filename, insert, replace, localSearch, maxIter);
        }
    }

    // string filename = "/users7/cse/tnlam/SummerProject2023/Local-Search/BPMP_Data_Files/10_nodes/t10_01_data.txt";
    // route.readSingleFile(filename, optimalProfits);
    // // // route.printData();
    // Algorithm algo(route);
    // bool localSearch = false; // debug purpose
    // bool insert = true;
    // bool replace = true;
    // int maxIter = route.numOfNode;
    // // printf("\n===========Algo1_ratio_a=============\n");
    // algo.GreedyAlgo1(csvFile, filename, true, false, insert, replace, localSearch, maxIter);
    // // csvFile << endl;
    // // // printf("\n===========Algo1=============\n");
    // // // algo.GreedyAlgo1(csvFile, filename, false, false, insert, replace, localSearch, maxIter);
    // // // csvFile << endl;
    // // // algo.GreedyAlgo2_b(csvFile, filename, true, insert, replace, localSearch, maxIter);
    // // // printf("\n===========Algo2_c=============\n");
    // // // csvFile << endl;
    // // // algo.GreedyAlgo3(csvFile, filename, true);
    // // // csvFile << endl;
    // return 0;
}

// maxIter = numOfNodes, percentage = 0.6, max count of occurrences of the same removed path = numOfNodes + 1,time threshold : 1000ms, 2000ms, and 3000ms
// maxIter = numOfNodes*10, percentage = 0.6, max count of occurrences of the same removed path = numOfNodes + 1,time threshold : 1000ms, 2000ms, and 3000ms