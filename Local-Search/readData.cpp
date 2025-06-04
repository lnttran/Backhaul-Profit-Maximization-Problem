#include "readData.h"
#include <optional>

/**
 * This function read txt file
 * @param filename
 */
void readData::readSingleFile(const string &filename, optional<unordered_map<string, double>> optimalProfits)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error in opening file: "
             << filename << endl;
        return;
    }

    w.clear(); // clear the matrix if the stores incorrect values
    d.clear(); // clear the matrix if the stores incorrect values

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string param, value, assignSymbol, n, p, c, Q, v, dis;
        string variable = "";
        int pos = 0;
        int check = 0;
        bool isWeight, isDemand;

        if (line.empty() || line[0] == '#')
        {
            // Skip empty lines or lines starting with '#'
            continue;
        }

        if (iss >> param >> value >> assignSymbol && assignSymbol == ":=")
        {
            if (param == "param" && value == "n")
            {
                iss >> n;
                numOfNode = stoi(n);
            }
            else if (param == "param" && value == "p")
            {
                iss >> p;
                priceCharged = stod(p);
            }
            else if (param == "param" && value == "c")
            {
                iss >> c;
                travelCost = stoi(c);
            }
            else if (param == "param" && value == "Q")
            {
                iss >> Q;
                totalCapacity = stoi(Q);
            }
            else if (param == "param" && value == "v")
            {
                iss >> v;
                vehicleWeight = stod(v);
            }
            else if (param == "param" && value == "DIS")
            {
                iss >> dis;
                DIS = stoi(dis);
            }
            else if (param == "param" && value == "w")
            {
                isWeight = true;
                isDemand = false;
            }
            else if (param == "param" && value == "d")
            {
                isWeight = false;
                isDemand = true;
            }
        }
        else if ((pos = line.find(":")))
        {
            if (pos != string::npos)
            {
                param = line.substr(0, pos);
                variable = line.substr(pos + 2);
                variable.erase(0, value.find_first_not_of(";"));

                // Process the parameter and value as needed
                if (param == "param n")
                {
                    numOfNode = stoi(variable);
                }
                else if (param == "param p")
                {
                    priceCharged = stod(variable);
                }
                else if (param == "param c")
                {
                    travelCost = stoi(variable);
                }
                else if (param == "param Q")
                {
                    totalCapacity = stoi(variable);
                }
                else if (param == "param v")
                {
                    vehicleWeight = stod(variable);
                }
                else if (param == "param DIS")
                {
                    DIS = stoi(variable);
                }
                else if (param == "param w")
                {
                    isWeight = true;
                    isDemand = false;
                    check = 1;
                }
                else if (param == "param d")
                {
                    isWeight = false;
                    isDemand = true;
                    check = 1;
                }
            }

            vector<string> tokens = splitString(line, '\t');

            if (tokens.size() == 3 && isWeight && !isDemand)
            {
                int intI = stoi(tokens[0]);
                int intJ = stoi(tokens[1]);
                double doubleWeight = stod(tokens[2]);

                if (intI >= w.size())
                    w.resize(intI + 1);
                if (intJ >= w[intI].size())
                    w[intI].resize(intJ + 1);

                w[intI][intJ] = doubleWeight;
            }
            else if (tokens.size() > 3 && check != 1 && isWeight && !isDemand)
            {
                vector<double> j;

                if (w.size() == 0)
                {
                    j.push_back(0);
                    w.push_back(j);
                }
                else
                    j.push_back(0);

                for (auto it = tokens.begin() + 1; it != tokens.end(); ++it)
                {
                    if (*it == "")
                    {
                        continue;
                    }
                    else
                        j.push_back(std::stod(*it));
                }

                w.push_back(j);
            }
            else if (tokens.size() == 3 && isDemand && !isWeight)
            {
                int intI = stoi(tokens[0]);
                int intJ = stoi(tokens[1]);
                double doubleDemand = stod(tokens[2]);

                if (intI >= d.size())
                    d.resize(intI + 1);
                if (intJ >= d[intI].size())
                    d[intI].resize(intJ + 1);

                d[intI][intJ] = doubleDemand;
            }
            else if (tokens.size() > 3 && check != 1 && !isWeight && isDemand)
            {
                vector<double> j;
                if (d.size() == 0)
                {
                    j.push_back(0);
                    d.push_back(j);
                }
                else
                    j.push_back(0);

                for (auto it = tokens.begin() + 1; it != tokens.end(); ++it)
                {
                    if (*it == "")
                    {
                        continue;
                    }
                    else
                        j.push_back(std::stod(*it));
                }
                d.push_back(j);
            }
        }
    }

    file.close();
    if (optimalProfits)
    {
        getOptimalProfit(filename, *optimalProfits);
    }
}

void readData::getOptimalProfit(const string &path, unordered_map<string, double> optimalProfits)
{
    auto it = optimalProfits.find(trimFileName(path));
    if (it != optimalProfits.end())
    {
        this->optimalProfit = it->second;
    }
}

unordered_map<string, double> readData::getOptimalProfitMap(const string &filename)
{
    unordered_map<string, double> optimalProfits;
    ifstream csvfile(filename);
    if (!csvfile.is_open())
    {
        cerr << "Error: Could not open the optimal profit file." << endl;
        return optimalProfits;
    }

    string line;
    getline(csvfile, line); // Read and ignore the header line
    while (getline(csvfile, line))
    {
        stringstream ss(line);
        string dataFile;
        double optProfit;

        if (getline(ss, dataFile, ',') && ss >> optProfit)
        {
            optimalProfits[dataFile] = optProfit;
        }
        else
        {
            cerr << "Error: Invalid line in the CSV file." << endl;
        }
    }

    return optimalProfits;
}

string readData::trimFileName(const string &filename)
{
    size_t pos = filename.find("_data.txt");
    size_t pos2 = filename.find(".txt");
    string substring;
    if (pos != string::npos)
    {
        substring = filename.substr(0, pos);
    }
    else if (pos2 != string::npos)
    {
        substring = filename.substr(0, pos2);
    }

    pos = substring.find_last_of("/");
    if (pos != string::npos)
    {
        substring = substring.substr(pos + 1);
    }

    return substring;
}

/**
 * This function print out the stats if the file read in
 */
void readData::printStats()
{
    cout << "===============Stats===============" << endl;
    cout << "Number of Node: " << numOfNode << endl;
    cout << "Price charged: " << priceCharged << endl;
    cout << "Travel cost: " << travelCost << endl;
    cout << "Total Capacity: " << totalCapacity << endl;
    cout << "Vehicle Weight: " << vehicleWeight << endl;
    cout << "DIS Maximum distance that Vehicle travel: " << DIS << endl;
    cout << "Total W: " << (w.size() - 1) << endl;
    cout << "Total D: " << (d.size() - 1) << endl;
    cout << "===================================" << endl
         << endl;
}

void readData::printData()
{

    cout << "Weight: " << endl;
    for (auto i : w)
    {
        for (auto j : i)
        {
            cout << j << " ";
        }
        cout << endl;
    }

    cout << "Distance: " << endl;
    for (auto i : d)
    {
        for (auto j : i)
        {
            cout << j << " ";
        }
        cout << endl;
    }
}

/**
 * This function split the string and return it as a vector of string seperate by delimiter
 * @param input string to split
 * @param delimiter which character to split at
 */
vector<string> readData::splitString(const string &input, char delimiter)
{
    vector<string> tokens;
    istringstream iss(input);
    string token;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

void readData::printPathInfo(double &totalWeight, double &currDist, bool distRatio, vector<int> path, int greedyNum)
{
    cout << "Weight: " << totalWeight << endl;
    cout << "Distance: " << currDist << endl;
    cout << "Ratio: " << totalWeight / currDist << endl;
    cout << "Path for Greedy #" << greedyNum << " (distRatio = " << distRatio << "): ";
    for (auto &c : path)
        cout << c << " ";
    cout << endl
         << endl;
}