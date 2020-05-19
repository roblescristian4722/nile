#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <map>
#include <stdexcept>

using namespace std;

template<typename T>
class Graph
{
public:
    Graph(bool d = false)
    {
        directed = d;
    }

    bool isEdge(const T& origin, const T& destiny);
    void createEdge(const T& origin, const T& destiny, int cost);
    void printData();
    int getCost(const T& origin, const T& destiny);
    const map<T, int>& getNeighbors(const T& origin);
    bool contains(const T& origin);

private:
    map<T, map<T, int>> graph;
    bool directed;
};
#endif // GRAPH_H

template<typename T>
bool Graph<T>::isEdge(const T &origin, const T &destiny)
{
    if (graph.find(origin) != graph.end())
    {
        if (graph[origin].find(destiny) != graph[origin].end())
        {
            return true;
        }
        return false;
    }
    return false;
}

template<typename T>
void Graph<T>::createEdge(const T& origin, const T& destiny, int cost)
{
    map<T, int> destinies;

    if (directed)
    {
        if (graph.find(origin) != graph.end())
        {
            destinies = graph[origin];
        }
        destinies[destiny] = cost;

        graph[origin] = destinies;
    }
    else
    {
        map<T, int> o_d;
        map<T, int> d_o;
        if (graph.find(origin) != graph.end())
        {
            o_d = graph[origin];
        }
        o_d[destiny] = cost;
        if (graph.find(destiny) != graph.end())
        {
            d_o = graph[destiny];
        }
        d_o[origin] = cost;

        graph[origin] = o_d;
        graph[destiny] = d_o;
    }
}

template<typename T>
void Graph<T>::printData()
{
    typename map<T, map<T, int>>::iterator it;

    it = graph.begin();

    while(it != graph.end())
    {
        typename map<T, int>::iterator itDest;
        itDest = graph[it->first].begin();
        while(itDest != graph[it->first].end())
        {
            cout <<"(" <<it->first <<"," <<itDest->first <<"," <<itDest->second <<")"
                <<endl;
            ++itDest;
        }
        ++it;
    }
}

template<typename T>
int Graph<T>::getCost(const T& origin, const T& destiny)
{
    if (!isEdge(origin, destiny))
    {
        throw invalid_argument("The edge does not exist");
    }
    return graph[origin][destiny];
}

template<typename T>
const map<T, int> &Graph<T>::getNeighbors(const T &origin)
{
    if (graph.find(origin) == graph.end())
        throw runtime_error("The origin doesn't exist");
    return graph[origin];
}

template<typename T>
bool Graph<T>::contains(const T &origin)
{
    return graph.find(origin) != graph.end();
}
