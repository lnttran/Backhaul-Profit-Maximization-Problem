#include "path.h"

// constructor
Path::Path(const readData &obj) : data(obj)
{
    int ROWS = data.numOfNode + 1, COLS = data.numOfNode + 1;
    visited.resize(ROWS, vector<double>(COLS, -1));
    cargo.resize(ROWS, vector<int>(COLS, 0));

    // add the first node to the path.
    path.push_back(1);
    totalDist = 0;
    profit = 0;
    locSrcProfit = 0;
}

// Copy constructor
Path::Path(const Path &obj) : data(obj.data)
{
    path = obj.path;
    visited = obj.visited;
    cargo = obj.cargo;
    totalDist = obj.totalDist;
    profit = obj.profit;
    locSrcProfit = obj.locSrcProfit;
}

/**
 * @brief This function check if j is found in the path
 *
 * @param j is the node
 * @return true if found
 * @return false if not found
 */
bool Path::isFound(int j) const
{
    if (find(path.begin(), path.end(), j) != path.end())
        return true;

    return false;
}

double Path::updateTotalDistance()
{
    double result = 0;
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        result += data.d[path[i]][path[i + 1]];
    }

    this->totalDist = result;

    return this->totalDist;
}

size_t Path::size() const
{
    return path.size();
}

/**
 * @brief Push_back function
 *
 * @param i
 */
void Path::push_back(int i)
{
    if (i != path[path.size() - 1])
        path.push_back(i);
}

/**
 * @brief This function calculate path profit.
 *
 * @return double
 */
double Path::calculateProfit()
{
    return this->profit = calculateProfit(this->visited, this->cargo, this->totalDist);
}

Path &Path::operator=(const Path &obj)
{
    if (this == &obj) // Check for self-assignment
        return *this;

    path = obj.path;
    visited = obj.visited;
    cargo = obj.cargo;
    totalDist = obj.totalDist;
    profit = obj.profit;
    locSrcProfit = obj.locSrcProfit;

    return *this;
}

/**
 * @brief Overload << operator
 *
 * @param os
 * @param obj
 * @return ostream&
 */
ostream &operator<<(std::ostream &os, const Path &obj)
{
    for (const auto &i : obj.path)
    {
        os << i << " ";
    }

    return os;
}

double Path::totalWeight() const
{
    double total = 0;

    for (int i = 0; i < cargo.size(); ++i)
    {
        for (int j = 0; j < cargo[i].size(); ++j)
        {
            if (cargo[i][j] == 1)
            {
                total += data.w[i][j];
            }
        }
    }

    return total;
}

/**
 * @brief using ostream to print out cargo accepted.
 *
 * @param os
 */
void Path::printCargo(ostream &os) const
{
    for (size_t i = 0; i < cargo.size(); ++i)
    {
        for (size_t j = 0; j < cargo[i].size(); ++j)
        {
            if (cargo[i][j] == 1)
            {
                os << i << ":" << j << " ";
            }
        }
    }
}

void Path::printCargoComma(ostream &os) const
{
    for (size_t i = 0; i < cargo.size(); ++i)
    {
        for (size_t j = 0; j < cargo[i].size(); ++j)
        {
            if (cargo[i][j] == 1)
            {
                os << "(" << i << "," << j << ") ";
            }
        }
    }
}

/**
 * @brief using ostream to print out visited arcs and the weight carried on them.
 *
 * @param os
 */
void Path::printVisited(ostream &os) const
{
    for (size_t i = 0; i < visited.size(); ++i)
    {
        for (size_t j = 0; j < visited[i].size(); ++j)
        {
            if (visited[i][j] != -1)
            {
                os << i << ":" << j << "=>" << visited[i][j] << " ";
            }
        }
    }
}

/**
 * @brief This function used insertion method to find if there are another additiona; destination
 * can be added to the current path to maximising the max distance can travel.
 *
 * Author: Tran Lam
 *
 * @param textFile
 * @param path
 * @param visited
 * @param cargo
 * @param currProfit
 * @param currDist
 * @param totalWeight
 */
double Path::insertSearch_Tran(fstream &textFile)
{
    bool loop = true;
    bool change = false;
    while (loop)
    {
        pair<vector<int>, double> max(vector<int>(), profit);
        bool insertion = false;

        auto i = path.begin();
        for (; i != path.end() - 1; ++i)
        {
            auto j = next(i);

            pair<vector<int>, double> tempMax = findMax(*i, *j);

            if (max.second < tempMax.second)
            {
                max = tempMax;
                insertion = true;
            }
        }

        if (insertion)
        {
            change = true;
            int pickup = max.first[0];
            int dropoff = max.first[1];
            int insertNode = max.first[2];
            visited[pickup][dropoff] = -1;                            // Modify the edge to be unvisited since that path is change.
            visited[pickup][insertNode] = data.w[pickup][insertNode]; // Update the edge
            visited[insertNode][dropoff] = data.w[insertNode][dropoff];
            cargo[pickup][dropoff] = 0; // Modify to not pick up the cargo
            cargo[pickup][insertNode] = 1;
            cargo[insertNode][dropoff] = 1;
            totalDist = (totalDist - data.d[pickup][dropoff]) + data.d[pickup][insertNode] + data.d[insertNode][dropoff];
            profit = max.second;
            auto it = find(path.begin(), path.end(), dropoff);
            path.insert(it, insertNode);
        }
        else if (!insertion)
        {
            loop = false;
        }
    }

    if (change)
    {
        textFile << "After Insertion: ";
        for (const auto &i : path)
        {
            textFile << i << " ";
        }
        textFile << ". New distance: " << totalDist << ". New profit: " << profit << endl;
    }
    else if (!change)
        textFile << "No additional nodes to insert." << endl;

    return profit;
}

/**
 * @brief This function used insertion method to find if there are another additiona; destination
 * can be added to the current path to maximising the max distance can travel.
 *
 * Author: Tran Lam
 *
 * @param textFile
 * @param path
 * @param visited
 * @param cargo
 * @param currProfit
 * @param currDist
 * @param totalWeight
 */
double Path::insertSearch_Tran_2() // fstream &textFile
{
    // fillCapacity(textFile);
    bool loop = true;
    while (loop)
    {
        Path maxPath(*this);
        bool insertion = false;

        auto i = path.begin();
        for (; i != path.end() - 1; ++i)
        {
            auto j = next(i);

            Path tempMaxPath = findMax_2(*i, *j);

            if (maxPath.profit < tempMaxPath.profit)
            {
                maxPath = tempMaxPath;
                insertion = true;
            }
        }

        if (insertion)
        {
            *this = maxPath;
        }
        else if (!insertion)
        {
            loop = false;
        }
    }

    // this->locSrcProfit = this->profit;

    return this->profit;
}

/**
 * @brief
 *
 * Author: Daniel Ryan
 *
 * @param textFile
 */
double Path::insertSearch_Daniel()
{
    // acceptable cargo -> options{calculate max possible profit, only check for additional cargo for direct paths, keep track of shortest distance}

    // for options[0]:
    // loop through each node in path, find maximum cargo that can be picked up (calculate profit)
    // would also have to loop find max capacity available

    // cout << "\n===================Insert Search a======================" << endl;

    /// locSrcProfit = profit;

    double maxProfit = this->fillCapacity_Daniel(this->path, visited, cargo, this->profit, totalDist);
    this->profit = maxProfit;

    double epsilon = std::numeric_limits<double>::epsilon();
    double newDistance = totalDist; // new total distance
    matrixD maxVisited(visited);
    matrixI maxCargo(cargo);

    // newNode = possible node to be inserted after j in path
    while (true)
    {
        pair<int, int> newNode_f = {-1, -1}; // {j, newNode} where newNode is the possible node to be inserted after index j in path

        for (int newNode = 1; newNode < data.numOfNode; newNode++)
        {
            if (find(path.begin(), path.end(), newNode) != path.end())
                continue;

            for (int j = 0; j < path.size() - 1; j++)
            {

                // calculate new total distance
                double distance = totalDist - data.d[path[j]][path[j + 1]] + data.d[path[j]][newNode] + data.d[newNode][path[j + 1]];
                // check distance and capacity constrant
                if (distance < data.DIS && visited[path[j]][path[j + 1]] < data.totalCapacity)
                {
                    // updates so visited matricies now reflect new node
                    Path tempPath(*this);
                    matrixI newCargo(tempPath.cargo);
                    matrixD newVisited(tempPath.visited);

                    tempPath.path.insert(tempPath.path.begin() + j + 1, newNode); // new path

                    newVisited[tempPath.path[j]][newNode] = newVisited[tempPath.path[j]][tempPath.path[j + 2]];     // j -> newNode
                    newVisited[newNode][tempPath.path[j + 2]] = newVisited[tempPath.path[j]][tempPath.path[j + 2]]; // newNode -> j+1

                    newVisited[tempPath.path[j]][tempPath.path[j + 2]] = -1;

                    double newProfit = calculateProfit(newVisited, newCargo, distance);
                    newProfit = tempPath.fillCapacity_Daniel(tempPath.path, newVisited, newCargo, newProfit, distance);
                    // cout << " new profit: " << newProfit << endl;
                    if (maxProfit < newProfit)
                    {
                        newNode_f = make_pair(j, newNode);
                        // cout << " new max: " << newProfit << endl;
                        maxCargo = newCargo;
                        maxVisited = newVisited;
                        maxProfit = newProfit;
                        newDistance = distance;
                    }
                }
            }
        }

        // if a new node+cargo has been found, update matrices similar to before for profit calculation
        if (newNode_f.first == -1)
        {
            // textFile << "Final path after Insert Search:" << endl;
            // textFile << "   Path:  " << *this << endl;
            // textFile << "   Cargo: ";
            // printCargo(textFile);
            // textFile << "\n   Visited: ";
            // printVisited(textFile);
            // textFile << "\n   Profit: " << profit << endl;

            locSrcProfit = maxProfit;
            this->profit = locSrcProfit;
            return maxProfit;
        }

        // update visited, cargo matrix
        path.insert(path.begin() + newNode_f.first + 1, newNode_f.second);
        visited = maxVisited;
        cargo = maxCargo;
        totalDist = newDistance;
        locSrcProfit = maxProfit;
    }
}

/**
 * @brief Time comeplexity: O(n^3)
 *
 */
void Path::fillCapacity()
{
    fillCapacity_Insertion(*this);
}

// TOO: put fillCapacity it after greedy
Path Path::replaceSearch()
{
    fillCapacity_Insertion(*this);
    Path maxPath(*this);

    if (this->path.size() <= 2)
    {
        return *this;
    }

    // Remove:
    //     1 18 62 9 20 37 63 80->
    //     Insert : 1 18 62 20 37 63 80

    for (int i = 1; i < path.size() - 1; i++)
    {
        Path tempPath(*this);
        int previousNode = tempPath.path[i - 1];
        int followingNode = tempPath.path[i + 1];
        int nodeToRemove = tempPath.path[i];
        // remove node position i
        tempPath.removeNode(i, false);
        // tempPath.removeNode(i, true);
        // replace node i with other node.

        // if (tempPath.path == tempPath.findMax_2(previousNode, followingNode, nodeToRemove).path)
        // {
        //     continue;
        // }

        tempPath.findMax_2(previousNode, followingNode);

        if (tempPath.profit > maxPath.profit)
        {
            maxPath = tempPath;
        }
    }

    *this = maxPath;

    return *this;
}

// double Path::replaceSearch()
// {

//     /*
//     for each node in path, not including start and end
//         for each node not in path,
//             Drop cargo to and from the old node
//             Replace old node with new node in path, update visited matrices
//             Run local search to find cargo and get new profit
//             Keep track of this node if profit is higher than current profit

//     If a new node with a higher profit is found
//         Update visited & cargo matrices
//     */

//     // cout << "===================REPLACE SEARCH===================" << endl;
//     // cout << "Path: ";
//     // for (auto &c : path)
//     //     cout << c << "->";
//     // cout << endl;
//     // printCargo(cout);
//     // this->locSrcProfit = profit;
//     // cout << "\norignal profit: " << locSrcProfit << endl;

//     // double maxProfit = this->fillCapacity_Daniel(this->path, this->visited, this->cargo, this->profit, this->totalDist);
//     // double maxProfit =
//     fillCapacity_Insertion(*this);
//     // this->locSrcProfit = this->profit;
//     // double maxProfit = this->profit;

//     // matrixD maxVisited(visited);
//     // matrixI maxCargo(cargo);
//     // double maxDistance = this->totalDist;
//     Path maxPath(*this);

//     while (true)
//     {
//         pair<int, int> replacement = {-1, -1}; // {oldNode index, newNode}

//         for (int i = 1; i < path.size() - 1; i++)
//         {
//             // cout << "---Path: " << path[i] << "---" << endl;

//             int oldNode = path[i];
//             Path tempPath(*this); // path without node i

//             // Remove cargo related to node i
//             // cout << "Removed Cargo: ";
//             for (int pickup = 1; pickup < cargo.size(); pickup++)
//             {
//                 for (int dropoff = 1; dropoff < cargo[pickup].size(); dropoff++)
//                 {
//                     if ((pickup == oldNode || dropoff == oldNode) && cargo[pickup][dropoff] != 0)
//                     {
//                         tempPath.cargo[pickup][dropoff] = 0; // remove cargo from cargo matrix
//                         // cout << pickup << "->" << dropoff << endl;
//                         // only have to find cargo to remove once, dont do loop every time
//                         // update visited to reflect removal of cargo
//                         auto it_f = find(tempPath.path.begin(), tempPath.path.end(), dropoff);
//                         for (auto it_s = find(tempPath.path.begin(), tempPath.path.end(), pickup); it_s != it_f; it_s++)
//                         {
//                             tempPath.visited[*it_s][*(it_s + 1)] -= data.w[pickup][dropoff];
//                             // cout << tempPath.visited[*it_s][*(it_s + 1)] << endl;
//                         }
//                     }
//                 }
//             }
//             // cout << endl;

//             // go through all possible replacements for node i
//             for (int newNode = 1; newNode < data.numOfNode; newNode++)
//             {
//                 if (find(path.begin(), path.end(), newNode) != path.end())
//                     continue;

//                 double tempDist = this->totalDist - data.d[path[i - 1]][oldNode] - data.d[oldNode][path[i + 1]] + data.d[path[i - 1]][newNode] + data.d[newNode][path[i + 1]];
//                 if (tempDist < data.DIS)
//                 {
//                     // cout << "New Node: " << newNode << endl;
//                     // cout << "New Distance: " << tempDist << endl;

//                     Path replacePath(tempPath);

//                     // matrixI newCargo(tempPath.cargo);
//                     // matrixD newVisited(tempPath.visited);

//                     replacePath.visited[path[i - 1]][newNode] = replacePath.visited[path[i - 1]][oldNode];
//                     replacePath.visited[newNode][path[i + 1]] = replacePath.visited[oldNode][path[i + 1]];
//                     // newVisited[path[i - 1]][newNode] = newVisited[path[i - 1]][oldNode];
//                     // newVisited[newNode][path[i + 1]] = newVisited[oldNode][path[i + 1]];

//                     // newVisited[path[i - 1]][oldNode] = -1;
//                     // newVisited[oldNode][path[i + 1]] = -1;
//                     replacePath.visited[path[i - 1]][oldNode] = -1;
//                     replacePath.visited[oldNode][path[i + 1]] = -1;

//                     replacePath.path[i] = newNode; // replace old node with new node in path
//                     replacePath.calculateProfit();
//                     fillCapacity_Insertion(replacePath);
//                     // double newProfit = calculateProfit(newVisited, newCargo, tempDist);
//                     // cout << "New Profit: " << newProfit << endl;

//                     // newProfit = tempPath.fillCapacity_Daniel(tempPath.path, newVisited, newCargo, newProfit, tempDist);
//                     // cout << "   Cargo: ";
//                     // printCargo(cout);
//                     // cout << endl;
//                     // cout << "   Visited: ";
//                     // printVisited(cout);
//                     // cout << endl;
//                     // cout << "New Profit after local search: " << newProfit << endl;

//                     if (replacePath.profit > maxPath.profit)
//                     {
//                         // cout << "   **New Max: " << newNode << " ; new profit of " << newProfit << endl;
//                         replacement = make_pair(i, newNode);
//                         maxPath = replacePath;
//                         // maxVisited = newVisited;
//                         // maxCargo = newCargo;
//                         // maxProfit = newProfit;
//                         // maxDistance = tempDist;
//                     }

//                     // tempPath.path[i] = oldNode;
//                 }
//             }
//         }

//         if (replacement.first == -1)
//         {
//             // locSrcProfit = maxProfit;
//             // profit = maxProfit;
//             // textFile << "Final path after Replace Search:" << endl;
//             // textFile << "   Path:  " << *this << endl;
//             // textFile << "   Cargo: ";
//             // printCargo(textFile);
//             // textFile << "\n   Visited: ";
//             // printVisited(textFile);
//             // textFile << "\n   Profit: " << maxProfit << endl;
//             // textFile << "   Distance: " << totalDist << endl;
//             // return maxProfit;

//             return this->profit;
//         }

//         *this = maxPath;

//         // path[replacement.first] = replacement.second;
//         // visited = maxVisited;
//         // cargo = maxCargo;
//         // totalDist = maxDistance;
//         // // locSrcProfit = maxProfit;
//         // profit = maxProfit;
//     }
// }

double Path::fillCapacity_Daniel()
{
    return fillCapacity_Daniel(this->path, this->visited, this->cargo, this->profit, this->totalDist);
}

/**
 * @brief this function calculate profit of given visited, cargo and totaldistance.
 *
 * @param visitParam
 * @param cargoParam
 * @param distParam
 * @return double
 */
double Path::calculateProfit(const matrixD &visitParam, const matrixI &cargoParam, const double &distParam)
{
    // p<constant> * cargo weight * straight line distance, no detours
    // - cost<constant> * sum (distance of an arc * weight carried on the arc)
    // - c*v * total distance traveled
    double p1 = 0;
    double p2 = 0;

    for (int i = 1; i < visitParam.size(); i++)
    {
        for (int j = 1; j < visitParam[i].size(); j++)
        {
            // calculating price charge per cargo picked up.
            if (cargoParam[i][j] != 0 && cargoParam[i][j] != -2)
            {
                p1 += (data.w[i][j] * data.d[i][j]);
            }
            // calculating travel cost
            if (visitParam[i][j] != -1 && visitParam[i][j] != 0 && visitParam[i][j] != -2)
            {
                p2 += (data.d[i][j] * visitParam[i][j]);
            }
        }
    }

    double revenue = (data.priceCharged * p1);
    double cost = (data.travelCost * p2);

    double thisProfit = (data.priceCharged * p1) - (data.travelCost * p2) - (data.travelCost * data.vehicleWeight * distParam);

    return thisProfit;
}

// front number, last number, insert number, profit.
// This function will find the maximun node given the front number and back number.
pair<vector<int>, double> Path::findMax(const int &pickup, const int &dropoff)
{
    double maxProfit = profit;
    int insertNode = 0;
    vector<int> nodes(3);

    for (int k = 1; k < visited[pickup].size() - 1; ++k)
    {
        if (find(path.begin(), path.end(), k) != path.end())
        {
            continue; // Skip this iteration if j is found in path
        }

        double tempDist = (totalDist - data.d[pickup][dropoff]) + data.d[pickup][k] + data.d[k][dropoff];

        if (tempDist <= data.DIS && data.w[pickup][k] <= data.totalCapacity && data.w[k][dropoff] <= data.totalCapacity)
        {
            matrixI tempCargo(cargo);
            matrix tempVisited(visited);
            tempVisited[pickup][dropoff] = -1;          // Modify the edge to be unvisited since that path is change.
            tempVisited[pickup][k] = data.w[pickup][k]; // Update the edge
            tempVisited[k][dropoff] = data.w[k][dropoff];
            tempCargo[pickup][dropoff] = 0; // Modify to not pick up the cargo
            tempCargo[pickup][k] = 1;
            tempCargo[k][dropoff] = 1;

            double tempProfit = calculateProfit(tempVisited, tempCargo, tempDist);

            // If the new profit is larger than the current profit, then update the entire thing.
            if (maxProfit < tempProfit)
            {
                maxProfit = tempProfit;
                insertNode = k;
            }
        }
    }

    nodes[0] = pickup;
    nodes[1] = dropoff;
    nodes[2] = insertNode;

    return make_pair(nodes, maxProfit);
}

/**
 * @brief Find the maximun node can be inserted in the given pickup and dropoff
 *
 * @param pickup
 * @param dropoff
 * @return Path
 */
Path Path::findMax_2(const int &pickup, const int &dropoff)
{
    Path maxPath(*this);

    for (int k = 1; k < visited[pickup].size() - 1; ++k)
    {
        // Ij k is found in the path, then continue
        if (isFound(k))
        {
            continue;
        }

        double tempDist = (totalDist - data.d[pickup][dropoff]) + data.d[pickup][k] + data.d[k][dropoff];

        if (tempDist <= data.DIS && data.w[pickup][k] <= data.totalCapacity && data.w[k][dropoff] <= data.totalCapacity)
        {
            Path tempPath(*this);
            // find the position of dropoff in the tempPath
            auto dropoffPosition = find(tempPath.path.begin(), tempPath.path.end(), dropoff);
            // insert new node in tempPath
            tempPath.totalDist = tempDist;
            tempPath.path.insert(dropoffPosition, k);
            tempPath.visited[pickup][dropoff] = -1;

            if (tempPath.cargo[pickup][dropoff] != -2)
            {
                tempPath.cargo[pickup][dropoff] = 0;
            }
            // Modify the edge to be unvisited since that path is change.
            // Modify to not pick up the cargo

            // need to check if there are any cargo picked up from the node in front of @param pickup to @param dropoff.
            double overArcCargo = 0;
            // auto pickupPosition = find(tempPath.path.begin(), tempPath.path.end(), pickup);
            int dropoffPosition_it = distance(tempPath.path.begin(), find(tempPath.path.begin(), tempPath.path.end(), dropoff));
            int pickupPosition_it = distance(tempPath.path.begin(), find(tempPath.path.begin(), tempPath.path.end(), pickup));

            for (int it = 0; it <= pickupPosition_it; ++it)
            {
                for (int it2 = dropoffPosition_it; it2 < tempPath.size(); ++it2)
                {

                    if (tempPath.cargo[tempPath.path[it]][tempPath.path[it2]] == 1)
                    {
                        overArcCargo += data.w[tempPath.path[it]][tempPath.path[it2]];
                    }
                }
            }

            tempPath.visited[k][dropoff] = (overArcCargo == 0) ? -1 : overArcCargo;
            tempPath.visited[pickup][k] = (overArcCargo == 0) ? -1 : overArcCargo;
            // Update the edge
            tempPath.updateTotalDistance();
            tempPath.calculateProfit();
            fillCapacity_Insertion(tempPath);

            if (maxPath.profit < tempPath.profit)
            {
                maxPath = tempPath;
            }
        }
    }

    return maxPath;
}

/**
 * @brief Time comeplexity: O(n^3)
 *
 */
void Path::fillCapacity_Insertion(Path &tempPath)
{
    // double tempProfit = tempPath.calculateProfit();
    vector<pair<int, int>> additionalCargo; // debugging purpose

    bool loop = true;
    bool change = false;

    while (loop)
    {
        pair<int, int> maxPair(0, 0); // debugging purpose
        Path maxPath(tempPath);

        for (int i = 0; i < tempPath.size() - 1; i++)
        {
            for (int j = i + 1; j < tempPath.size(); j++)
            {
                int pickup = tempPath.path[i], dropoff = tempPath.path[j];
                if (tempPath.cargo[pickup][dropoff] == 0 && data.w[pickup][dropoff] > 0)
                {

                    Path thisTempPath = tempPath;

                    double max = 0;
                    for (int k = i; k < j; k++)
                    {
                        double visitedPickUpDropoff = thisTempPath.visited[thisTempPath.path[k]][thisTempPath.path[k + 1]];
                        max = (visitedPickUpDropoff > max) ? visitedPickUpDropoff : max;
                        if (visitedPickUpDropoff == -1 || visitedPickUpDropoff == 0 || visitedPickUpDropoff == -2)
                        {
                            thisTempPath.visited[thisTempPath.path[k]][thisTempPath.path[k + 1]] = data.w[pickup][dropoff];
                        }
                        else
                        {
                            thisTempPath.visited[thisTempPath.path[k]][thisTempPath.path[k + 1]] += data.w[pickup][dropoff];
                        }
                    }

                    if ((data.w[pickup][dropoff] + max) <= data.totalCapacity)
                    {
                        thisTempPath.cargo[pickup][dropoff] = 1;
                        double tempProfit = thisTempPath.calculateProfit();
                        if (maxPath.profit < tempProfit)
                        {
                            maxPath = thisTempPath;
                            change = true;
                            maxPair = make_pair(pickup, dropoff); // debugging purpose
                        }
                    }
                }
            }
        }

        if (change)
        {
            change = false;
            tempPath = maxPath;
            additionalCargo.push_back(maxPair); // debugging purpose
        }
        else if (!change)
        {
            loop = false;
        }
    }
}

double Path::fillCapacity_Daniel(vector<int> path, matrix &visited, vector<vector<int>> &cargo, double &originProfit, double totalDist)
{
    // find additional cargo loads between each of the current nodes that could be added (find w[i][j] and see if it is within capacity of each arc that the truck will carry it)
    // compare the profit for each, pick largest profit
    double maxProfit = originProfit; // save load with new max profit

    while (true)
    {
        pair<int, int> cargo_f = {-1, -1};
        double weight = 0;

        // iterate through each combination of possible cargo loads
        for (int i = 0; i < path.size() - 1; i++)
        {
            for (int j = i + 1; j < path.size(); j++)
            {

                int pickup = path[i], dropoff = path[j];
                if (cargo[pickup][dropoff] == 0 && data.w[pickup][dropoff] > 0)
                { // make sure cargo is available
                    // variables used to calculate profit later
                    vector<vector<int>> tempCargo = cargo;
                    matrix tempVisited = visited;

                    // max capacity truck is at possible cargo locations
                    double max = 0;
                    for (int k = i; k < j; k++)
                    {

                        max = (visited[path[k]][path[k + 1]] > max) ? visited[path[k]][path[k + 1]] : max;
                        tempVisited[path[k]][path[k + 1]] += data.w[pickup][dropoff]; // used for calculating new profit later
                    }
                    if ((data.w[pickup][dropoff] + max) <= data.totalCapacity)
                    { // if the new cargo can fit within capacity
                        tempCargo[pickup][dropoff] = 1;
                        double prof = calculateProfit(tempVisited, tempCargo, totalDist);
                        if (prof > maxProfit)
                        { // keep track of max profit gain
                            maxProfit = prof;
                            cargo_f = {i, j};
                            weight = data.w[pickup][dropoff];
                        }
                    }
                }
            }
        }

        if (cargo_f.first == -1)
        {
            // locSrcProfit = maxProfit;
            this->profit = maxProfit;
            return maxProfit;
        }
        for (int k = cargo_f.first; k < cargo_f.second; k++)
            visited[path[k]][path[k + 1]] += data.w[path[cargo_f.first]][path[cargo_f.second]];

        cargo[path[cargo_f.first]][path[cargo_f.second]] = 1;
    }
}

Path Path::deterministicAlgo(bool insert, bool replace)
{
    bool running = true;

    while (running)
    {
        double maxProfit = this->profit;
        Path tempPath(*this);
        // Local search phase

        if (replace)
        {
            tempPath.replaceSearch();
        }
        if (insert)
        {
            tempPath.insertSearch_Tran_2();
        }
        if (tempPath.profit > maxProfit)
        {
            // updateing path
            *this = tempPath;
        }
        else
        {
            running = false;
        }
    }

    return *this;
}

Path Path::removeNodeRandom(int numOfNodeToRemove)
{
    // remove randomly each node from the path
    random_device rd;
    mt19937 gen(42);
    // mt19937 gen(rd());
    // vector<int> remove = {2, 2, 4, 2};

    for (int i = 0; i < numOfNodeToRemove; ++i)
    {
        // if there is 2 nodes remain in the path, terminate
        // if (this->path.size() <= 2)
        // {
        //     break;
        // }
        if (this->path.size() <= 3)
        {
            break;
        }
        uniform_int_distribution<int> dist(1, this->path.size() - 2);
        int removeNodePosition = dist(gen); // find the node position generate radomly
        // int removeNodePosition = remove[i];
        this->removeNode(removeNodePosition, true);
        // this->locSrcProfit = 0;
    }

    cout << "Disturb: " << *this << " Profit: " << this->profit << endl;
    return *this;
}

void Path::removeNode(int removeNodePosition, bool calProfit)
{
    int nodeToRemove = this->path.at(removeNodePosition);      // find the node to remove
    int previousNode = this->path.at(removeNodePosition - 1);  // find the node before node to remove
    int followingNode = this->path.at(removeNodePosition + 1); // find the node after node to remove

    // this loop will remove all the cargo that is associated with the nodeToRemove throughout the path
    for (int node : path)
    {
        if (node == nodeToRemove)
        {
            continue;
        }

        int nodePosition = distance(this->path.begin(), find(this->path.begin(), this->path.end(), node));

        if (cargo[node][nodeToRemove] == 1)
        {
            for (int i = nodePosition; i < removeNodePosition; ++i)
            {
                // subtracting the weight of the cargo associated with the nodeToRemove from the visited matrix
                visited[this->path[i]][this->path[i + 1]] -= data.w[node][nodeToRemove];
                if (visited[this->path[i]][this->path[i + 1]] <= 0)
                {
                    visited[this->path[i]][this->path[i + 1]] = -1;
                }
            }

            cargo[node][nodeToRemove] = 0;
        }
        if (cargo[nodeToRemove][node] == 1)
        {
            for (int i = removeNodePosition; i < nodePosition; i++)
            {
                visited[this->path[i]][this->path[i + 1]] -= data.w[nodeToRemove][node];
                if (visited[this->path[i]][this->path[i + 1]] <= 0)
                {
                    visited[this->path[i]][this->path[i + 1]] = -1;
                }
            }
            cargo[nodeToRemove][node] = 0;
        }
    }

    if (visited[previousNode][nodeToRemove] > 0)
    {
        visited[previousNode][followingNode] = visited[previousNode][nodeToRemove];
    }
    else if (visited[nodeToRemove][followingNode] > 0)
    {
        visited[previousNode][followingNode] = visited[nodeToRemove][followingNode];
    }

    visited[previousNode][nodeToRemove] = -1;
    visited[nodeToRemove][followingNode] = -1;
    this->totalDist = totalDist - data.d[previousNode][nodeToRemove] - data.d[nodeToRemove][followingNode] + data.d[previousNode][followingNode];
    this->path.erase(this->path.begin() + removeNodePosition);

    if (calProfit)
        calculateProfit();
}

/**
 * @param maxIter : maximun of iteration the disturbance will loop
 * @param percentage : 0.1 -> 0.9 , Percentage of nodes in path to be removed
 * @param insert : boolean indicate if insert
 * @param replace : boolean indicate if replace
 */
StopFactor Path::disturbance(int maxIter, int timeLimit, double percentange, int &setSize, bool insert, bool replace)
{
    cout << "Greedy: " << *this
         << " Profit: " << this->profit << endl;
    auto startTime = chrono::high_resolution_clock::now();
    if (percentange >= 1)
    {
        cerr << "Percentage of remove node can not exceed or equal 1";
        return ERROR;
    }

    unordered_set<vector<int>, VectorHash> setPath; // the appeared setPath
    // random_device rd;
    mt19937 gen(42);
    Path maxPath(*this);
    cout << "Max Path: " << maxPath << " Profit: " << maxPath.profit << endl;

    Path tempPath(maxPath);
    bool stopLoop = false;
    bool timeExceed = false;
    int countTwice = 0;
    deque<Path> maxPathQueue;
    maxPathQueue.push_back(maxPath);

    size_t i = 0;
    int count = 1;

    for (; i < maxIter; ++i)
    {
        auto currentTime = chrono::high_resolution_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(currentTime - startTime).count();

        if (elapsedTime > timeLimit)
        {
            timeExceed = true;
            break;
        }

        tempPath.deterministicAlgo(insert, replace);
        cout << "Deterministic: " << tempPath << " Profit: " << tempPath.profit << endl; // debug

        if (tempPath.profit > maxPath.profit)
        {
            maxPath = tempPath;
            maxPathQueue.push_back(maxPath);
            cout << "Max Path: " << maxPath << " Profit: " << maxPath.profit << endl; // debug
        }

        if (tempPath.size() <= 3)
        {
            if (!maxPathQueue.empty() && maxPathQueue.back().size() > 3)
            {
                tempPath = maxPathQueue.back();
                maxPathQueue.pop_back();
                cout << "Return Max Path: " << tempPath << " Profit: " << tempPath.profit << endl; // debug purpose
            }
            else
                break;
        }

        int maxNodeToRemove = ((tempPath.size() - 2) * percentange);
        // int maxNodeToRemove = ((tempPath.size() - 1) * percentange);

        bool keepDisturbing = true;
        while (keepDisturbing)
        {
            Path pathRemoved(tempPath);
            // if (count == this->data.numOfNode + 1)
            // if (count == this->data.numOfNode / 2)
            if (count == 7)
            {
                countTwice++;
                count = 1;
                if (maxPathQueue.empty())
                {
                    stopLoop = true;
                    break;
                }
                // pathRemoved = maxPathQueue.front();
                // maxPathQueue.pop_front();
                pathRemoved = maxPathQueue.back();
                maxPathQueue.pop_back();
                cout << "Return Max Path: " << pathRemoved << " Profit: " << pathRemoved.profit << endl; // debug purpose
            }

            if (countTwice == 4)
            {
                stopLoop = true;
                break;
            }

            uniform_int_distribution<int> dist(1, maxNodeToRemove);
            int numOfNode = dist(gen);

            pathRemoved.removeNodeRandom(numOfNode);
            // pathRemoved.removeNode(3);

            // check if pathRemoved is found in setPath
            if (setPath.find(pathRemoved.path) != setPath.end())
            {
                count++;
            }
            // if not found then insert to the setPath
            else
            {
                keepDisturbing = false;
                setPath.insert(pathRemoved.path);
                tempPath = pathRemoved;
            }
        }

        if (stopLoop)
            break;
    }

    setSize = setPath.size();
    *this = maxPath;

    if (stopLoop)
    {
        return COUNT_OCCURURANCE_LIMIT;
    }
    else if (i == maxIter)
    {
        return ITERATION_LIMIT;
    }
    else if (timeExceed)
    {
        return TIME_LIMIT;
    }

    return NO_REMOVAL;
}

void Path::resetPathButRemainVistedCargo()
{
    this->path.clear();
    this->path.push_back(1);
    this->profit = 0;
    this->totalDist = 0;
    this->locSrcProfit = 0;

    // Iterate through the visited matrix and replace non -1 values with 0
    for (size_t i = 0; i < this->visited.size(); ++i)
    {
        for (size_t j = 0; j < this->visited[i].size(); ++j)
        {
            if (this->visited[i][j] != -1)
            {
                // this indicate that another vehicle has visited this path
                this->visited[i][j] = -2;
            }
        }
    }

    for (size_t i = 0; i < this->cargo.size(); ++i)
    {
        for (size_t j = 0; j < this->cargo[i].size(); ++j)
        {
            if (this->cargo[i][j] != 0)
            {
                // this indicate that another vehicle has picked up this cargo
                this->cargo[i][j] = -2;
            }
        }
    }
}