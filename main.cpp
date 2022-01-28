#include <iostream>
#include <map>

#include "./include/graph.h"
#include "./include/parser.h"

using namespace std;

map<int, string> getStopName(map<string, int> m);

int main() {
    Parser parser;
    Graph nightLinesWithDist(2500, true);
    Graph dayLinesWithDist(2500, true);
    map<string, int> stops = parser.mapStopToInt();
    map<int, string> stopName = getStopName(stops);
    vector<vector<string>> night1;
    vector<vector<string>> night0;
    vector<vector<string>> day1;
    vector<vector<string>> day0;

    Graph dayLines = parser.parseDayLines();
    Graph nightLines = parser.parseNightLines();
    //parser.parseNightLinesWithDistances(night0, night1);
    //parser.parseDayLinesWithDistances(day0, day1);
    //parser.addDistances(night0, nightLinesWithDist);
    //parser.addDistances(night1, nightLinesWithDist);
    //parser.addDistances(day0, dayLinesWithDist);
    //parser.addDistances(day1, dayLinesWithDist);
    list<int> path = nightLines.dijkstra_path(stops["AL3"], stops["JM1"]);

    for(auto i: path)
         cout << stopName[i] << endl;

    //cout << dayLines.bfs_distance(stops["INF1"], stops["PASS"]) << endl;
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