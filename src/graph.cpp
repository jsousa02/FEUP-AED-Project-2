// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#include "graph.h"
#define INF (INT_MAX/2)

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num + 1) {}

// Constructor: default
Graph::Graph() = default;

/**
 * @brief adds an edge connecting the src node to the dest node with a given weight
 * @param src the source node
 * @param dest the destination node
 * @param line the line the edge belongs
 * @param weight the weight given to the edge
 */
void Graph::addEdge(int src, int dest, string line, double weight) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    nodes[src].adj.push_back({dest, weight, line});
    if (!hasDir) nodes[dest].adj.push_back({src, weight, line});
}

/**
 * @brief calculates the shortest distance to a given node
 * @brief time complexity O(E log(V)), E represents the edge relaxations and V the number of nodes
 * @param s the node to calculate the distance to
 */
void Graph::dijkstra(int s) {
    MinHeap<int, int> q(n, -1);
    for (int v=1; v<=n; v++) {
        nodes.at(v).dist = INF;
        q.insert(v, INF);
        nodes.at(v).visited = false;
    }
    nodes.at(s).dist = 0;
    q.decreaseKey(s, 0);
    nodes.at(s).pred = s;
    while (q.getSize()>0) {
        int u = q.removeMin();
        // cout << "Node " << u << " with dist = " << nodes[u].dist << endl;
        nodes.at(u).visited = true;
        for (auto e : nodes.at(u).adj) {
            if (nodes.at(u).isOpen){
                double v = e.dest;
                double w = e.weight;

                if (!nodes.at(v).visited && nodes.at(u).dist + w < nodes.at(v).dist) {
                    nodes.at(v).dist = nodes.at(u).dist + w;
                    q.decreaseKey(v, nodes.at(v).dist);
                    nodes.at(v).pred = u;
                }
            }
        }
    }
}

/**
 * @brief calculates the minimum distance between 2 nodes
 * @param a the first node
 * @param b the second node
 * @return the minimum distance between the nodes
 */
double Graph::dijkstra_distance(int a, int b) {
    dijkstra(a);
    if(nodes.at(b).dist == INF) return -1.0;
    return nodes.at(b).dist;
}

/**
 * @brief calculates the path from a starting node to a destination node
 * @param a the starting node
 * @param b the destination node
 * @return a list containing the nodes that are part of the path
 */
list<int> Graph::dijkstra_path(int a, int b) {
    dijkstra(a);
    list<int> path;
    if (dijkstra_distance(a,b) == -1.0) return path;
    path.push_front(b);
    int i = b;
    while( i != a) {
        i = nodes[i].pred;
        path.push_front(i);
    }
    return path;
}

/**
 * @brief finds the path that passes through the fewer number of stops
 * @brief time complexity: O(V + E) in which V is the number of nodes and E the number of edges
 * @param v the starting node
 */
void Graph::bfs(int v) {
    for (int v=1; v<=n; v++) nodes.at(v).visited = false;
    nodes.at(v).dist = 0;
    queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes.at(v). visited = true;
    nodes.at(v).pred = v;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();
        //cout << u << " "; // show node order
        for (auto e : nodes.at(u).adj) {
            int w = e.dest;
            if (!nodes.at(w).visited) {
                q.push(w);
                nodes.at(w).visited = true;
                nodes.at(w).dist = nodes.at(u).dist +1;
                nodes.at(w).pred = u;
            }
        }
    }
}

/**
 * @brief gets the amount of stops between 2 nodes
 * @param a the starting node
 * @param b the ending node
 * @return the amount of stops between the 2 nodes
 */
int Graph::bfs_distance(int a, int b) {
    if (a == b) return 0;
    for (int v = 1; v < n; v++) nodes[v].dist = -1;
    bfs(a);
    return nodes[b].dist;
}

/**
 * @brief calculates the path from a starting node to a destination node
 * @param a the starting node
 * @param b the ending node
 * @return a list containing the nodes that are part of the path
 */
list<int> Graph::bfs_path(int a, int b) {
    list<int> path;
    if (bfs_distance(a,b) == -1) return path;
    bfs(a);
    path.push_front(b);
    int i = b;
    while( i != a) {
        i = nodes.at(i).pred;
        path.push_front(i);
    }
    return path;
}

void Graph::closeNode(int a) {
    nodes.at(a).isOpen = false;
}
