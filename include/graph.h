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
        double weight; // An integer weight
        string line; //
    };

    struct Node {
        vector<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        int pred;
        bool visited;
        double dist;
    };

    int n;              // Graph size (vertices are numbered from 1 to n)
    bool hasDir;        // false: undirect; true: directed
    // The list of nodes being represented
    vector<Node> nodes;
    void dijkstra(int s);
    void bfs(int v);

public:
    // Constructor: nr nodes and direction (default: undirected)
    Graph(int nodes, bool dir = false);
    // Add edge from source to destination with a certain weight
    void addEdge(int src, int dest, string line, double weight = 1.0);
    double dijkstra_distance(int a, int b);
    int bfs_distance(int a, int b);
    list<int> dijkstra_path(int a, int b);
    friend ostream& operator<<(ostream& out, Graph graph);
};

#endif
