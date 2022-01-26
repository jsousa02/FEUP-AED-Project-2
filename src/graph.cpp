// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#include "graph.h"
#include <climits>
#include <queue>

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num + 1) {}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, string line, double weight) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    nodes[src].adj.push_back({dest, weight, line});
    if (!hasDir) nodes[dest].adj.push_back({src, weight, line});
}

void Graph::dijkstra(int s) {
    MinHeap<int, int>q(n, -1);

    for(int v = 1; v <= n; v++) {
        nodes.at(v).dist = INT_MAX / 2;
        q.insert(v, nodes.at(v).dist);
        nodes.at(v).visited = false;
    }

    nodes.at(s).dist = 0;
    q.decreaseKey(s, nodes.at(s).dist);

    while(q.getSize() != 0) {
        int u = q.removeMin();
        nodes.at(u).visited = true;

        for(auto e : nodes.at(u).adj) {
            int v = e.dest;
            double w = e.weight;

            if(!nodes.at(v).visited && nodes.at(u).dist + w < nodes.at(v).dist) {
                nodes.at(v).dist = nodes.at(u).dist + w;
                q.decreaseKey(v, nodes.at(v).dist);
            }
        }
    }

}

// ----------------------------------------------------------
// 1) Algoritmo de Dijkstra e caminhos mais curtos
// ----------------------------------------------------------
// ..............................
// a) Distância entre dois nós
// TODO
double Graph::dijkstra_distance(int a, int b) {
    dijkstra(a);
    if(nodes.at(b).dist == INT_MAX) return -1;
    return nodes.at(b).dist;
}

// ..............................
// b) Caminho mais curto entre dois nós
// TODO
list<int> Graph::dijkstra_path(int a, int b) {
    list<int> path;
    return path;
}

void Graph::bfs(int v) {
    queue<int> q;
    q.push(v);
    nodes.at(v).dist = 0;
    nodes.at(v).visited = true;

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(auto e: nodes.at(u).adj) {
            int w = e.dest;
            if(!nodes.at(w).visited) {
                q.push(w);
                nodes.at(w).visited = true;
                nodes.at(w).dist = nodes.at(u).dist + 1;
            }
        }
    }
}


int Graph::bfs_distance(int a, int b) {
    bfs(a);
    return nodes.at(b).dist;
}