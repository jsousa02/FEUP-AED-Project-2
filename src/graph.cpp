// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#include "graph.h"
#include <climits>

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num+1) {}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, int weight) {
    if (src<0 || src>n || dest<0 || dest>n) return;
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

double Graph::Node::getLatitude() {
    return this->latitude;
}

double Graph::Node::getLongitude() {
    return this->longitude;
}

double Graph::Node::getDist() {
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
    for(int i = 0; i < graph.nodes.size(); i++) {
        cout << "Name = " << graph.nodes.at(i).getName() << endl;
        cout << "Code = " << graph.nodes.at(i).getCode() << endl;
        cout << "Zone = " << graph.nodes.at(i).getZone() << endl;
        cout << "Latitude = " << graph.nodes.at(i).getLatitude() << endl;
        cout << "Longitude = " << graph.nodes.at(i).getLongitude() << endl;
        cout << "-----------------------------------------------" << endl;
    }
}

void Graph::dijkstra(int s) {
    MinHeap<int, int>q(n, -1);

    for(int v = 1; v <= n; v++) {
        nodes.at(v).dist = INT_MAX;
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
            int w = e.weight;

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
int Graph::dijkstra_distance(int a, int b) {
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
