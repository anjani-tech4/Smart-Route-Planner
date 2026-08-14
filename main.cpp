#include <iostream>
#include<unordered_map>
#include<vector>
#include <fstream>
#include <sstream>
#include <string>
#include "dijkstra.h"
using namespace std;

int main()
{
unordered_map<string, vector<Edge>> graph;
ifstream file("data.csv");

if (!file)
{
cout << "File not found!" << endl;
return 0;
}

string line;

getline(file, line); // header skip

while (getline(file, line))
{
string station1, station2;
double distance;
double cost;

stringstream ss(line);

getline(ss, station1, ',');
getline(ss, station2, ',');
ss >> distance;
ss.ignore();
ss >> cost;

graph[station1].push_back({station2, distance, cost});
graph[station2].push_back({station1, distance, cost});

}

file.close();

// SOURCE AND DESTINATION
string source, destination;

cout << "Enter source station: ";
getline(cin, source);
if(graph.find(source)==graph.end())
{
cout<<"Source station not found!"<<endl;
return 0;
}

cout << "Enter destination station: ";
getline(cin, destination);
if(graph.find(destination)==graph.end())
{
cout<<"Destination station not found!"<<endl;
return 0;
}

 // CALL DIJKSTRA
double result = dijkstra(graph, source, destination);

cout << "Shortest distance: " << result << " km" << endl;
return 0;
}