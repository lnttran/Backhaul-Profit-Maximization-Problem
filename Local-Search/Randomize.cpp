#include "Randomize.h"

void Randomize::runRandomized(fstream &csvFile, const string &file, string test, bool ifRandomized, int maxIter, int k)
{

    clock_t CPUStart = clock();
    auto start = steady_clock::now();

    vector<multimap<double, int>> orderedDistances(data.d.size() - 1);

    string name = "";
    if (ifRandomized)
        name = "randomGreedy_shortestPath";
    else
        name = "Roulette_selection";

    for (int i = 1; i < data.d.size() - 1; i++)
    {
        for (int j = 1; j < data.d[i].size() - 1; j++)
        {
            if (i != j)
            {
                orderedDistances[i].insert(make_pair(data.d[i][j], j));
            }
        }
    }

    int iter = 1;

    // textFile << "============= New Random Greedy =============" << endl;

    Path *bestPath;
    if (ifRandomized)
        bestPath = new Path(greedyRandomized(orderedDistances, iter, k));
    else
        bestPath = new Path(rouletteSelection(orderedDistances, iter, k));

    bestPath->calculateProfit();
    // bestPath->locSrcProfit = bestPath->profit;
    // recordText(textFile, file, name, *bestPath, iter);
    bestPath->insertSearch_Tran_2();
    // textFile << endl;
    bestPath->replaceSearch();
    // textFile << endl
    //          << endl;

    while (iter < maxIter)
    {
        iter++;
        Path *newPath;
        if (ifRandomized)
            newPath = new Path(greedyRandomized(orderedDistances, iter, k));
        else
            newPath = new Path(rouletteSelection(orderedDistances, iter, k));

        newPath->calculateProfit();
        newPath->insertSearch_Tran_2();
        newPath->replaceSearch();

        if (newPath->profit > bestPath->profit)
        {
            delete bestPath;
            bestPath = newPath;
            newPath = nullptr;
        }
        else
        {
            delete newPath;
        }
    }

    double randomizedProfit = bestPath->profit;

    int sizeSet = 0;
    StopFactor stopFactor = bestPath->disturbance(data.numOfNode, 3000, 0.7, sizeSet, true, true);

    auto stop = steady_clock::now();
    clock_t CPUEnd = clock();
    double cpu_time_used = static_cast<double>(CPUEnd - CPUStart) * 1000.0 / CLOCKS_PER_SEC;
    double run_time = (duration<double>(stop - start).count()) * 1000.0;
    recordData(csvFile, file, test, name, *bestPath, randomizedProfit, stopFactor, sizeSet, cpu_time_used, run_time);

    delete bestPath;
    return;
}

Path Randomize::greedyRandomized(vector<multimap<double, int>> &orderedDistances, int iter, int k)
{
    int numNodes = data.d.size();
    int endNode = numOfNode;

    int *nodesVisited = new int[numNodes]{};
    nodesVisited[1] = 1;

    int currNode = 1;
    bool findNode = true;
    Path path(data);
    path.push_back(currNode);
    // int k = k;

    // srand(time(0) + iter);
    srand(42);
    while (findNode)
    {

        vector<pair<double, int>> RCL;
        int count = 0;
        for (auto &c : orderedDistances[currNode])
        {
            double nodeDistance = path.totalDist + c.first + data.d[c.second][endNode];
            if (!nodesVisited[c.second] && nodeDistance < data.DIS)
            {
                RCL.push_back(c);
                if (++count == k)
                    break;
            }
        }

        if (RCL.empty())
        {
            path.totalDist += data.d[currNode][endNode];
            path.visited[currNode][endNode] = 0;
            findNode = false;
        }
        else
        {
            int r = rand() % RCL.size();
            path.visited[currNode][RCL[r].second] = 0;
            // cout << "  New Distance: " << RCL[r].first << " = " << d[currNode][RCL[r].second] << endl;
            currNode = RCL[r].second;
            nodesVisited[currNode] = 1;
            path.totalDist += RCL[r].first;
            path.push_back(currNode);
        }
    }
    path.push_back(endNode);
    return path;
}

Path Randomize::rouletteSelection(vector<multimap<double, int>> &orderedDistances, int iter, int k)
{
    int numNodes = data.d.size();
    int endNode = numOfNode;

    int *nodesVisited = new int[numNodes]{};
    nodesVisited[1] = 1;

    int currNode = 1;
    Path path(data);
    path.push_back(1);
    bool findNode = true;

    // srand(time(0) + iter);
    srand(42);
    while (findNode)
    {
        vector<pair<double, int>> RCL;
        int count = 0;
        for (auto &c : orderedDistances[currNode])
        {
            double nodeDistance = path.totalDist + c.first + data.d[c.second][endNode];
            if (!nodesVisited[c.second] && nodeDistance <= data.DIS)
            {
                RCL.push_back(c);
                if (++count == k)
                    break;
            }
        }

        if (RCL.empty())
        {
            path.totalDist += data.d[currNode][endNode];
            path.visited[currNode][endNode] = 0;
            findNode = false;
            path.push_back(endNode);
        }
        else
        {
            double distProduct = 1;
            double sumDistance = 0;
            // Calculate distProduct of the node in RCL
            for (const auto &i : RCL)
            {
                distProduct *= i.first;
            }

            map<int, double> adjustDistance;

            for (const auto &i : RCL)
            {
                adjustDistance[i.second] = (distProduct / i.first);
            }

            for (const auto &i : adjustDistance)
            {
                sumDistance += i.second;
            }

            map<int, double> ratio;

            for (const auto &i : RCL)
            {
                ratio[i.second] = (adjustDistance[i.second] / sumDistance);
            }

            double r = (double)rand() / RAND_MAX;

            // std::random_device rd;                                // Obtain a random seed from the hardware
            // std::mt19937 gen(rd());                               // Seed the random number generator
            // std::uniform_real_distribution<double> dis(0.0, 1.0); // Define the distribution

            // double r = dis(gen);

            double sumRatio = 0;
            for (const auto &i : RCL)
            {
                sumRatio += ratio[i.second];
                if (r <= sumRatio)
                {
                    path.visited[currNode][i.second] = 0;
                    // cout << "  New Distance: " << i.first << " = " << d[currNode][i.second] << endl;
                    currNode = i.second;
                    nodesVisited[currNode] = 1;
                    path.totalDist += i.first;
                    path.push_back(currNode);
                    break;
                }
            }
        }
    }

    return path;
};

void Randomize::recordText(fstream &textFile, const string &file, string algoType, const Path &path, int iter)
{
    textFile << "Filename:    " << file << endl;
    textFile << "Algorithm:   " << algoType << endl;
    textFile << "Profit:      " << path.profit << endl;
    textFile << "Weight:      " << path.totalWeight() << endl;
    textFile << "Distance:    " << path.totalDist << endl;
    textFile << "Iter Number: " << iter << endl;
    textFile << path;
    textFile << endl;
    textFile << "Cargo Accepted: ";
    path.printCargo(textFile);
    textFile << endl
             << endl;
}

void Randomize::recordData(fstream &outputFile, const string &file, string test, string algoType, const Path &path, const double &randomizedProfit, const StopFactor &stopFactor, const int &setSize, double cpuTime, double realTime) //, vector<int> requested)
{
    // cout << file << "," << algoType << "," << path.profit << "," << cpuTime << "," << realTime << ",";
    // csvFile << "Filename, Type of Algorithm, nn selection, iter selection, Profit post-fill, Profit after Local Search, Optimal Profit, Error, sizeOfPathSet, StopFactor, CPU Time(ms), Run Time(ms), Route, Requests" << endl;
    outputFile << file << "," << algoType << "," << test << "," << randomizedProfit << "," << path.profit << "," << data.optimalProfit << "," << errorToOptimal(path.profit) << "," << setSize << "," << stopFactor << "," << cpuTime << "," << realTime << ",";
    cout << file << "," << algoType << "," << test << "," << randomizedProfit << "," << path.profit << "," << data.optimalProfit << "," << errorToOptimal(path.profit) << "," << setSize << "," << stopFactor << "," << cpuTime << "," << realTime << ",";

    cout << path;
    outputFile << path;

    cout << ",";
    outputFile << ",";

    path.printCargo(outputFile);
    outputFile << endl;
    path.printCargo(cout);
    cout << endl;
    cout << endl;
    // output
}