// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "minHeap.h"
#include <vector>
#include <list>
#include <iostream>

using namespace std;

class Graph {
    struct Edge {
        int dest;   // Destination node
        int weight; // An integer weight
        string line; //
    };

    struct Node {
        list<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        int pred;
        bool visited;
        string name, code, zone;
        double latitude, longitude, dist;
        void setName(string name);
        void setLatitude(double latitude);
        void setLongitude(double longitude);
        void setCode(string code);
        void setZone(string zone);
        void setVisited(bool visited);
        void setDist(double dist);
        list<Edge> getEdges() {return adj;};
        bool isVisited();
        string getName();
        string getCode();
        string getZone();
        double getLatitude();
        double getLongitude();
        double getDist();
    };

    int n;              // Graph size (vertices are numbered from 1 to n)
    bool hasDir;        // false: undirect; true: directed
     // The list of nodes being represented

    void dijkstra(int s);

public:
    // Constructor: nr nodes and direction (default: undirected)
    Graph(int nodes, bool dir = false);
    // Add edge from source to destination with a certain weight
    void addEdge(int src, int dest, int weight = 1);
    vector<Node> nodes;
    int dijkstra_distance(int a, int b);
    list<int> dijkstra_path(int a, int b);
    friend ostream& operator<<(ostream& out, Graph graph);
};

#endif
