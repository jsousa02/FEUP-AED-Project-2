// AED 2021/2022 - Aula Pratica 11
// Pedro Ribeiro (DCC/FCUP) [17/01/2022]

#include "graph.h"


// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num + 1) {}

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

/**
 * @brief calculates the minimum distance between 2 nodes
 * @param a the first node
 * @param b the second node
 * @return the minimum distance between the nodes
 */
double Graph::dijkstra_distance(int a, int b) {
    dijkstra(a);
    if(nodes.at(b).dist == INT_MAX) return -1;
    return nodes.at(b).dist;
}

/**
 * @brief calculates the path from a starting node to a destination node
 * @param a the starting node
 * @param b the destination node
 * @return a list containing the nodes that are part of the path
 */
list<int> Graph::dijkstra_path(int a, int b) {
    list<int> path;
    return path;
}

/**
 * @brief finds the path that passes through the fewer number of stops
 * @brief time complexity: O(V + E) in which V is the number of nodes and E the number of edges
 * @param v the starting node
 */
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

/**
 * @brief gets the amount of stops between 2 nodes
 * @param a the starting node
 * @param b the ending node
 * @return the amount of stops between the 2 nodes
 */
int Graph::bfs_distance(int a, int b) {
    bfs(a);
    return nodes.at(b).dist;
}

/**
 * @brief finds closest stops to a point given a maximum distance
 * @param lat the latitude of the point
 * @param lon the longitude of the point
 * @param dist the maximum distance to the point
 * @return a vector with all the stops that are closer to the point
 */
vector<string> Graph::findClosestStop(double lat, double lon, double dist) {
    ifstream stopsFile("./dataset/stops.csv");

    string firstLine, line, stopCode, stopName;
    int count = 0;
    double lat2, lon2, distance;
    vector<string> closestStops;

    getline(stopsFile, firstLine);

    while(getline(stopsFile, line)) {
        stringstream ss(line);
        while(getline(ss, line, ',')) {
            if(count == 0)
                stopCode = line;
            else if(count == 1)
                stopName = line;
            else if(count == 3)
                lat2 = stod(line);
            else if(count == 4)
                lon2 = stod(line);
            count++;
        }
        count = 0;
        distance = haversine(lat, lon, lat2, lon2);
        if(distance <= dist)
            closestStops.push_back(stopCode);
    }
    return closestStops;
}

/**
 * @brief calculates the distance in kilometers between 2 points given their coordinates
 * @param lat1 the latitude of the first point
 * @param lon1 the longitude of the first point
 * @param lat2 the latitude of the second point
 * @param lon2 the longitude of the second point
 * @return the distance between the 2 points
 */
double Graph::haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formula
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}
