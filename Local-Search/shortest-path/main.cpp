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
#include "../readData.h"
#include "Shortest_path.h"
#include <filesystem>

using namespace std;
using namespace std::chrono;
namespace fs = std::__fs::filesystem;
// int argc, char **argv
int main()
{
    // cout << argc << endl;
    // for (int i = 1; i < argc; ++i)
    //     cout << argv[i] << endl;

    readData route;

    // fstream csvFile(argv[2], ios::trunc | ios::out); // name of the output file
    // fstream csvFile("test10.csv", ios::out); // debug purpose

    // if (!csvFile)
    // {
    //     std::cerr << "Error opening a file. hi" << std::endl;
    //     return 1;
    // }

    // auto it = filesystem::directory_iterator(argv[1]);
    auto it = fs::directory_iterator("/Users/tranlam/development/repo/SummerProject2023/Local-Search/shortest-path/data/sp_data");

    for (const auto &entry : it)
    {
        fs::path path = entry.path();
        if (entry.is_regular_file())
        {
            string filename = path.string();
            cout << filename << endl;
            route.readSingleFile(filename);

            string outfilename = filename.substr(filename.find_last_of("/\\") + 1);
            Shortest_path shortestpath(route);
            shortestpath.dijkstra(1, outfilename);
        }
    }
    return 0;
}
