#include "Shortest_path.h"

void Shortest_path::dijkstra(int start, string filename)
{
    int n = this->d.size() - 1; // Skip row 0 and column 0 logically
    int destination = n;
    // Distance vector initialized to infinity
    vector<double> dist(n + 1, INF);

    // Priority queue to store (distance, node) pairs
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

    // Vector to track visited nodes
    vector<bool> visited(n + 1, false);

    // Vector to store the previous node (for path reconstruction)
    vector<int> prev(n + 1, -1);

    // Start node has a distance of 0
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty())
    {
        int currNode = pq.top().second;
        pq.pop();

        // If the current node is already visited, skip it
        if (visited[currNode])
            continue;
        visited[currNode] = true;

        // If we reached the destination, stop the algorithm
        if (currNode == destination)
            break;

        // Relax edges (skip index 0)
        for (int neighbor = 1; neighbor <= n; ++neighbor)
        {
            if (this->d[currNode][neighbor] > 0)
            { // Valid edge
                double newDist = dist[currNode] + this->d[currNode][neighbor];

                if (newDist < dist[neighbor])
                {
                    dist[neighbor] = newDist;
                    prev[neighbor] = currNode;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }

    // Output the shortest distance
    // if (dist[destination] == INF)
    // {
    //     cout << "No path from node " << start << " to node " << destination << ".\n";
    //     return;
    // }

    // cout << "Shortest distance from node " << start << " to node " << destination << ": " << dist[destination] << "\n";

    // // Reconstruct the shortest path
    // vector<int> path;
    // for (int at = destination; at != -1; at = prev[at])
    // {
    //     path.push_back(at);
    // }
    // reverse(path.begin(), path.end());

    // cout << "Path: ";
    // for (size_t i = 0; i < path.size(); ++i)
    // {
    //     cout << path[i]; // Use 1-based indexing
    //     if (i < path.size() - 1)
    //         cout << " -> ";
    // }
    // cout << "\n";
    // for (size_t i = 0; i < path.size() - 1; ++i)
    // {
    //     int from = path[i];
    //     int to = path[i + 1];
    //     cout << "From node " << from << " to node " << to << ": " << this->d[from][to] << "\n";
    // }
    // cout << "\n";

    ofstream outFile(filename);

    if (!outFile)
    {
        std::cerr << "Error: Unable to open " << filename << " file !\n ";
        return;
    }

    // cout << "Shortest distances from node " << start << ":\n";
    // for (int i = 1; i <= n; ++i)
    // {
    //     if (dist[i] == INF)
    //     {
    //         cout << "Node " << i << ": No path\n";
    //     }
    //     else
    //     {
    //         cout << "Node " << i << ": " << dist[i] << "\n";
    //     }
    // }

    outFile << dist[n];

    // Output paths to all reachable nodes
    // cout << "\nShortest paths from node " << start << ":\n";
    for (int destination = 1; destination <= n; ++destination)
    {
        if (destination == start || dist[destination] == INF)
            continue; // Skip the start node and unreachable nodes

        vector<int> path;
        for (int at = destination; at != -1; at = prev[at])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        // cout << "Path to node " << destination << ": ";
        outFile << "\n";
        outFile << "[";
        for (size_t i = 0; i < path.size(); ++i)
        {
            outFile << path[i]; // Use 1-based indexing
            if (i < path.size() - 1)
                // cout << " -> ";
                outFile << ",";
        }

        outFile << "]";
    }
    outFile.close();
}
