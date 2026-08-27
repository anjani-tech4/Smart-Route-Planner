#include<iostream>
#include<pq.h>
#include<vector>
#include<unordered_map>
#include<string>
#include<stack>
#include<unordered_set>
#include<algorithm>
#include "dijkstra.h"

using namespace std;

int my_comparator(pair<string,double> left, pair<string,double> right)
{
    return left.second > right.second;
}

int my_search_comparator(pair<string,double> left, pair<string,double> right)
{
    return left.first == right.first;
}

double dijkstra(
    unordered_map<string, vector<Edge>> &graph,
    string source,
    string destination,
    vector<string> *pathOut,
    double *costOut = nullptr
)
{
    unordered_map<string,pair<string,double>> path_map;
    unordered_map<string,double> cost_map;

    // Initialize path_map
    for(auto &entry : graph)
    {
        path_map[entry.first] = {"", 0};
    }

    cost_map[source] = 0;

    unordered_set<string> visited;
    PQ<pair<string,double>> pq(my_comparator,my_search_comparator);

    pq.push({source,0});

    pair<string,double> element;
    bool route_found = false;

    while(!pq.empty())
    {
        element = pq.top();
        pq.pop();

        //visited.erase(element.first);

        if(element.first == destination)
        {
            route_found = true;
            break;
        }

        //code to traverse adjacent vertices of the element extracted from pq
        auto iter = graph.find(element.first);
        vector<Edge> vector_of_pairs = (*iter).second;

        for(auto j : vector_of_pairs)
        {
            auto adv = j.station;
            auto total_distance = j.distance + element.second;
            auto total_cost = cost_map[element.first] + j.cost;

            if(visited.find(adv) != visited.end())
            {
                auto idx = pq.find({adv,0});

                if(idx == -1)
                {
                    continue;
                }

                auto visited_entry = pq.at(idx);

                if(total_distance >= visited_entry.second)
                    continue;

                visited_entry.second = total_distance;
                pq.update(visited_entry,idx);

                auto path_iter = path_map.find(adv);
                (*path_iter).second = {element.first,j.distance};

                cost_map[adv] = total_cost;
            }

            else
            {
                pair<string,double> entry(adv,total_distance);
                pq.push(entry);
                visited.insert(adv);

                auto path_iter = path_map.find(adv);
                (*path_iter).second = {element.first,j.distance};

                cost_map[adv] = total_cost;
            }
        }
    }

    if(route_found == true)
    {
        cout << "Route exists and the distance is : "
             << element.second << "km" << endl;

        cout << "Total cost is : "
             << cost_map[destination] << endl;

        if(costOut != nullptr)
        {
            *costOut = cost_map[destination];
        }

        auto look_for = destination;
        stack<pair<string,double>> stk;

        while(1)
        {
            if(look_for == source)
                break;

            auto path_iter = path_map.find(look_for);
            auto k = (*path_iter).first;
            auto val_pair = (*path_iter).second;

            stk.push({k,val_pair.second});
            look_for = val_pair.first;
        }

        // ---- additive only: capture the same path into pathOut if requested ----
        if(pathOut != nullptr)
        {
            pathOut->clear();
            pathOut->push_back(source);
        }

        cout << source;

        while(!stk.empty())
        {
            auto pr = stk.top();
            stk.pop();

            cout << "------" << pr.second << "km ------>" << pr.first;

            if(pathOut != nullptr)
            {
                pathOut->push_back(pr.first);
            }
        }

        cout << endl;
    }

    else
    {
        cout << "Route doesnot exists" << endl;
        return -1;
    }

    return element.second;
}