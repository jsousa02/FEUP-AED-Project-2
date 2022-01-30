#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "minHeap.h"
#include <vector>
#include <list>
#include <iostream>
#include <climits>
#include <queue>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Graph {
    struct Edge {
        int dest;
        double weight;
        string line;
    };

    struct Node {
        vector<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        int pred;
        bool visited;
        double dist;
        bool isOpen = true;
    };

    int n;
    bool hasDir;
    vector<Node> nodes;
    void dijkstra(int s);
    void bfs(int v);
    void mst(int s);
public:
    Graph();
    Graph(int nodes, bool dir = false);
    void addEdge(int src, int dest, string line, double weight = 1.0);
    double dijkstra_distance(int a, int b);
    int bfs_distance(int a, int b);
    list<int> dijkstra_path(int a, int b);
    list<int> bfs_path(int a, int b);
    double mst_distance(int a);
    void closeNode(int a);
};

#endif
