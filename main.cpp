#include <iostream>
#include<unordered_map>
#include<vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

double dijkstra(
    unordered_map<string, vector<pair<string, double>>> &graph,
    string source,
    string destination
);

double calculateCost(vector<string> route)
{
ifstream file("data.csv");

string line;
double total_cost = 0;

getline(file, line); // header skip

for(int i = 0; i < route.size() - 1; i++)
{
string from = route[i];
string to = route[i + 1];

file.clear();
file.seekg(0);

getline(file, line); // header skip

while(getline(file, line))
{
string station1, station2;
double distance, cost;

stringstream ss(line);

getline(ss, station1, ',');
getline(ss, station2, ',');
ss >> distance;
ss.ignore();
ss >> cost;

if((station1 == from && station2 == to) ||
(station1 == to && station2 == from))
{
total_cost += cost;
break;
}
}
}

file.close();

return total_cost;
}


int main()
{
unordered_map<string, vector<pair<string,double>>> graph;
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

graph[station1].push_back({station2, distance});
graph[station2].push_back({station1, distance});
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