#include <iostream>
#include <map>

#include "./include/graph.h"
#include "./include/parser.h"

using namespace std;

map<int, string> getStopName(map<string, int> m);


int main() {
    Parser parser;
    /*
    Graph dayLines = parser.parseDayLines();
    map<string, int> stops = parser.mapStopToInt(parser.readDayStops(parser.readDayLines()));
    map<int, string> stopsName = getStopName(stops);

    cout << dayLines.bfs_distance(stops["INF1"], stops["PASS"]) << endl;
    list<int> path = dayLines.bfs_path(stops["INF1"], stops["PASS"]);
    cout << path.size() << endl;
    for (auto index: path) {
        cout << stopsName[index] << endl;
    }
    */

    Graph dayLinesWithDistances = parser.parseDayLinesWithDistances();
    map<string, int> stops = parser.mapStopToInt(parser.readDayStops(parser.readDayLines()));
    map<int, string> stopsName = getStopName(stops);

    list<int> path = dayLinesWithDistances.dijkstra_path(stops["INF1"], stops["PASS"]);
    for (auto i : path) {
        cout << stopsName[i] << endl;
    }

    cout << dayLinesWithDistances.dijkstra_distance(stops["INF1"], stops["PASS"]) << endl;//4.5612
    cout << dayLinesWithDistances.dijkstra_distance(stops["PASS2"], stops["INF1"]) << endl;//4.5683


    //Graph nightLinesWithDistances = parser.parseNightLinesWithDistances();
    //cout << dayLinesWithDistances.dijkstra_distance(stops["CQ10"], stops["BLRB1"]) << endl;
    //cout << dayLinesWithDistances.dijkstra_distance(stops["BLRB1"], stops["CQ10"]) << endl;

    cout << "done\n";
    return 0;
}


map<int, string> getStopName(map<string, int> m) {
    map<int, string> res;
    for(auto x: m) {
        res[x.second] = x.first;
    }
    return res;
}