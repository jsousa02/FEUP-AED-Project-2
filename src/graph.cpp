// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#include "graph.h"
#include <climits>
#include <cmath>
#define INF (INT_MAX/2)


// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num+1) {}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, float weight) {
    if (src<0 || src>n || dest<0 || dest>n) return;
    weight = harversine(src, dest);
    nodes[src].adj.push_back({dest, weight});
    if (!hasDir) nodes[dest].adj.push_back({src, weight});
}

void Graph::Node::setName(string name) {
    this->name = name;
}

void Graph::Node::setCode(string code) {
    this->code = code;
}

void Graph::Node::setDist(double dist) {
    this->dist = dist;
}

void Graph::Node::setVisited(bool visited) {
    this->visited = visited;
}

void Graph::Node::setLatitude(double latitude) {
    this->latitude = latitude;
}

void Graph::Node::setLongitude(double longitude) {
    this->longitude = longitude;
}

void Graph::Node::setZone(string zone) {
    this->zone = zone;
}

float Graph::Node::getLatitude() {
    return this->latitude;
}

float Graph::Node::getLongitude() {
    return this->longitude;
}

float Graph::Node::getDist() {
    return this->dist;
}

bool Graph::Node::isVisited() {
    return this->visited;
}

string Graph::Node::getCode() {
    return this->code;
}

string Graph::Node::getName() {
    return this->name;
}

string Graph::Node::getZone() {
    return this->zone;
}

ostream& operator<<(ostream &out, Graph graph) {
    for(int i = 1; i < graph.nodes.size(); i++) {
        cout << "Name = " << graph.nodes.at(i).getName() << endl;
        cout << "Code = " << graph.nodes.at(i).getCode() << endl;
        cout << "Zone = " << graph.nodes.at(i).getZone() << endl;
        cout << "Latitude = " << graph.nodes.at(i).getLatitude() << endl;
        cout << "Longitude = " << graph.nodes.at(i).getLongitude() << endl;
        cout << "-----------------------------------------------" << endl;
    }
}

void Graph::dijkstra(int s) {
    MinHeap<int, int> q(n, -1);
    for (int v=1; v<=n; v++) {
        nodes[v].dist = INF;
        q.insert(v, INF);
        nodes[v].visited = false;
    }
    nodes[s].dist = 0;
    q.decreaseKey(s, 0);
    nodes[s].pred = s;
    while (q.getSize()>0) {
        int u = q.removeMin();
        // cout << "Node " << u << " with dist = " << nodes[u].dist << endl;
        nodes[u].visited = true;
        for (auto e : nodes[u].adj) {
            double v = e.dest;
            double w = e.weight;
            if (!nodes[v].visited && nodes[u].dist + w < nodes[v].dist) {
                nodes[v].dist = nodes[u].dist + w;
                q.decreaseKey(v, nodes[v].dist);
                nodes[v].pred = u;
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
float Graph::dijkstra_distance(int a, int b) {
    dijkstra(a);
    if (nodes[b].dist == INF) return -1;
    return nodes[b].dist;
}

// ..............................
// b) Caminho mais curto entre dois nós
// TODO
list<int> Graph::dijkstra_path(int a, int b) {
    dijkstra(a);
    list<int> path;
    if (nodes[b].dist == INF) return path;
    path.push_back(b);
    int v = b;
    while (v != a) {
        v = nodes[v].pred;
        path.push_front(v);
    }
    return path;
}

float Graph::harversine(int a, int b) {
    float lat1, lon1, lat2, lon2;
    lat1 = nodes.at(a).getLatitude();
    lon1 = nodes.at(a).getLongitude();
    lat2 = nodes.at(b).getLatitude();
    lon2 = nodes.at(b).getLongitude();

    float dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    float dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formula
    float c = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    float rad = 6371;
    float r = 2 * asin(sqrt(c));
    return rad * r;
}
