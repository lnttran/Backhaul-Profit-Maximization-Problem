#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <unordered_map>
#include "readData.h"

// https://cplusplus.com/reference/random/mt19937/

using namespace std;
using matrix = vector<vector<double>>;

int main(int argc, char **argv) // int argc, char **argv
{

    cout << argc << endl;
    for (int i = 1; i < argc; ++i)
        cout << argv[i] << endl;

    // Set a sparsity level (e.g., 0.3 means 30% of the values will be set to 0.0)
    double sparsity = 0.8;

    readData data;

    unordered_map<string, double> optimalProfits = data.getOptimalProfitMap("BPMP_Data_Files/optimalProfit.csv");
    // unordered_map<string, double> optimalProfits = data.getOptimalProfitMap("/users7/cse/tnlam/SummerProject2023/Local-Search/BPMP_Data_Files/optimalProfit.csv");

    data.readSingleFile(argv[1], optimalProfits);
    // data.readSingleFile("/users7/cse/tnlam/SummerProject2023/Local-Search/BPMP_Data_Files/10_nodes/t10_01_data.txt", optimalProfits);

    // Seed for the random number generator
    random_device rd;

    // PRNG algorithm that produces high-quality random numbers
    // Seeding ensures that the random numbers generated will be different each time the program runs, providing true randomness.
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    // Initialize non-zero element counters for rows and columns
    vector<int> nonZeroRowCount(data.w.size(), data.w.size() - 1);
    vector<int> nonZeroColCount(data.w.size(), data.w.size() - 1);

    // Iterate through the matrix and randomly set values to 0.0 based on sparsity
    // for (auto &row : data.w)
    // {
    //     for (auto &element : row)
    //     {
    //         if (dis(gen) < sparsity)
    //         {
    //             element = 0.0;
    //         }
    //     }
    // }

    for (size_t i = 1; i < data.w.size(); i++)
    {
        for (size_t j = 1; j < data.w[i].size(); j++)
        {
            if (dis(gen) < sparsity)
            {
                // Only set to zero if it's not the last non-zero element in its row and column
                if (nonZeroRowCount[i] > 1 && nonZeroColCount[j] > 1)
                {
                    data.w[i][j] = 0.0;
                    nonZeroRowCount[i]--;
                    nonZeroColCount[j]--;
                }
            }
        }
    }

    // Update the text file with the new sparse matrix
    ofstream outputFile(argv[2]);

    // ofstream outputFile("test.txt");
    if (!outputFile)
    {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    // read w matrix from the file
    // Write the parameter values to the file
    outputFile << "param n:=" << data.numOfNode << ";" << endl;
    outputFile << "param p:=" << data.priceCharged << ";" << endl;
    outputFile << "param c:=" << data.travelCost << ";" << endl;
    outputFile << "param Q:=" << data.totalCapacity << ";" << endl;
    outputFile << "param v:=" << data.vehicleWeight << ";" << endl;
    outputFile << "param DIS:=" << data.DIS << ";" << endl;

    if (data.numOfNode > 30)
    {
        // Write matrix w to the file
        outputFile << "param w :=" << endl;

        for (int i = 1; i <= data.numOfNode; i++)
        {
            for (int j = 1; j <= data.numOfNode; j++)
            {
                outputFile << i << "\t" << j << "\t" << data.w[i][j] << endl;
            }
        }

        outputFile << ";" << endl;

        // Write matrix d to the file
        outputFile << "param d :=" << endl;

        for (int i = 1; i <= data.numOfNode; i++)
        {
            for (int j = 1; j <= data.numOfNode; j++)
            {
                outputFile << i << "\t" << j << "\t" << data.d[i][j] << endl;
            }
        }

        outputFile << ";" << endl;
    }
    else
    {
        // Write matrix w to the file
        outputFile << "param w:";

        for (int i = 1; i <= data.numOfNode; i++)
        {
            outputFile << " " << i;
        }

        outputFile << " :=" << endl;

        for (int i = 1; i <= data.numOfNode; i++)
        {
            outputFile << i << "\t";

            for (int j = 1; j <= data.numOfNode; j++)
            {
                outputFile << data.w[i][j] << "\t";
            }

            outputFile << endl;
        }

        outputFile << ";" << endl;

        // Write matrix d to the file
        outputFile << "param d:";

        for (int i = 1; i <= data.numOfNode; i++)
        {
            outputFile << " " << i;
        }

        outputFile << " :=" << endl;

        for (int i = 1; i <= data.numOfNode; i++)
        {
            outputFile << i << "\t";

            for (int j = 1; j <= data.numOfNode; j++)
            {
                outputFile << data.d[i][j] << "\t";
            }

            outputFile << endl;
        }

        outputFile << ";" << endl;
    }

    // Close the file
    outputFile.close();

    cout << "Data has been written to the " << argv[2] << " file." << endl;

    return 0;
}