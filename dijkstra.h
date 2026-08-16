#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct Edge
{
    string station;
    double distance;
    double cost;
};

double dijkstra(
    unordered_map<string, vector<Edge>> &graph,
    string source,
    string destination,
    vector<string> *pathOut = nullptr
);