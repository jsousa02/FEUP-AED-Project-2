#include <iostream>
#include <map>

#include "./include/graph.h"
#include "./include/parser.h"

using namespace std;

int main() {
    Parser parser;
    Graph nightLinesWithDist(2500, true);
    Graph dayLinesWithDist(2500, true);
    map<string, int> stops = parser.mapStopToInt();
    vector<vector<string>> night1;
    vector<vector<string>> night0;
    vector<vector<string>> day1;
    vector<vector<string>> day0;

    Graph dayLines = parser.parseDayLines();
    Graph nightLines = parser.parseNightLines();
    parser.parseNightLinesWithDistances(night0, night1);
    parser.parseDayLinesWithDistances(day0, day1);
    parser.addDistances(night0, nightLinesWithDist);
    parser.addDistances(night1, nightLinesWithDist);
    parser.addDistances(day0, dayLinesWithDist);
    parser.addDistances(day1, dayLinesWithDist);

    //cout << dayLines.bfs_distance(stops["INF1"], stops["PASS"]) << endl;
    cout << "done\n";
    return 0;
}
