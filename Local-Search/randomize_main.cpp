#include <iostream> //input and output
#include <fstream>  //read file
#include <sstream>
#include <algorithm>
#include <climits>
#include <set>
#include <unordered_map>
#include <string>
#include <ctime>  //to measure CPU time
#include <chrono> //to measure run time
#include "readData.h"
#include "Randomize.h"
#include "Parameters.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    cout << argc << endl;
    for (int i = 1; i < argc; ++i)
        cout << argv[i] << endl;
    readData route;
    unordered_map<string, double> optimalProfits = route.getOptimalProfitMap(argv[3]);
    fstream csvFile(argv[2], ios::trunc | ios::out);
    if (!csvFile)
    {
        std::cerr << "Error opening a file." << std::endl;
        return 1;
    }

    csvFile << "Filename, Type of Algorithm, nn selection, iter selection, Profit post-fill, Profit after Local Search, Optimal Profit, Error, sizeOfPathSet, StopFactor, CPU Time(ms), Run Time(ms), Route, Requests" << endl;

    auto it = filesystem::directory_iterator(argv[1]);

    for (const auto &entry : it)
    {
        filesystem::path path = entry.path();
        if (entry.is_regular_file())
        {

            string filename = path.string();
            route.readSingleFile(filename, optimalProfits);
            Randomize randomAlgo(route);

            string testSelection = argv[4];
            if (testSelection == "roulette")
            {
                randomAlgo.runRandomized(csvFile, filename, "30% File Size,1 * File Size", false, route.numOfNode, 0.3 * route.numOfNode);
                randomAlgo.runRandomized(csvFile, filename, "Sqrt of size,1 * File Size", false, route.numOfNode, sqrt(route.numOfNode));
                randomAlgo.runRandomized(csvFile, filename, "All,1 * File Size", false, route.numOfNode, INT_MAX);
                randomAlgo.runRandomized(csvFile, filename, "30% File Size,2 * File Size", false, 2 * route.numOfNode, 0.3 * route.numOfNode);
                randomAlgo.runRandomized(csvFile, filename, "Sqrt of size,2 * File Size", false, 2 * route.numOfNode, sqrt(route.numOfNode));
                randomAlgo.runRandomized(csvFile, filename, "All,2 * File Size", false, 2 * route.numOfNode, INT_MAX);
                randomAlgo.runRandomized(csvFile, filename, "30% File Size,5 * File Size", false, 5 * route.numOfNode, 0.3 * route.numOfNode);
                randomAlgo.runRandomized(csvFile, filename, "Sqrt of size,5 * File Size", false, 5 * route.numOfNode, sqrt(route.numOfNode));
                randomAlgo.runRandomized(csvFile, filename, "All,5 * File Size", false, 5 * route.numOfNode, INT_MAX);
            }
            else if (testSelection == "random")
            {
                randomAlgo.runRandomized(csvFile, filename, "30% File Size,1 * File Size", true, route.numOfNode, 0.3 * route.numOfNode);
                randomAlgo.runRandomized(csvFile, filename, "Sqrt of size,1 * File Size", true, route.numOfNode, sqrt(route.numOfNode));
                randomAlgo.runRandomized(csvFile, filename, "All,1 * File Size", true, route.numOfNode, INT_MAX);
                randomAlgo.runRandomized(csvFile, filename, "30% File Size,2 * File Size", true, 2 * route.numOfNode, 0.3 * route.numOfNode);
                randomAlgo.runRandomized(csvFile, filename, "Sqrt of size,2 * File Size", true, 2 * route.numOfNode, sqrt(route.numOfNode));
                randomAlgo.runRandomized(csvFile, filename, "All,2 * File Size", true, 2 * route.numOfNode, INT_MAX);
                randomAlgo.runRandomized(csvFile, filename, "30% File Size,5 * File Size", true, 5 * route.numOfNode, 0.3 * route.numOfNode);
                randomAlgo.runRandomized(csvFile, filename, "Sqrt of size,5 * File Size", true, 5 * route.numOfNode, sqrt(route.numOfNode));
                randomAlgo.runRandomized(csvFile, filename, "All,5 * File Size", true, 5 * route.numOfNode, INT_MAX);
            }
        }
    }

    return 0;
}
