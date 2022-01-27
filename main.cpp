#include <iostream>
#include <map>

#include "./include/graph.h"
#include "./include/parser.h"

using namespace std;

map<string, int> mapStopToInt();

int main() {
    Parser parser;
    Graph dayLinesWithDistances = parser.parseDayLinesWithDistances();
    Graph nightLinesWithDistances = parser.parseNightLinesWithDistances();
    Graph dayLines = parser.parseDayLines();
    map<string, int> stops = mapStopToInt();

    cout << dayLines.bfs_distance(stops["INF1"], stops["PASS"]) << endl;
    cout << "done\n";
    return 0;
}

/**
 * @briefs maps a stop code into an integer
 * @return a map in which the stop code is the key and the value is the corresponding integer
 */
map<string, int> mapStopToInt() {
    ifstream stopsFile("./dataset/stops.csv");

    string line, firstLine;
    map<string, int> stops;
    int pos = 1;

    getline(stopsFile, firstLine);

    while(getline(stopsFile, line)) {
        stringstream ss(line);
        while(getline(ss, line, ',')) {
            for(int i = 0; i < 4; i++) {
                if(i == 0) {
                    stops.insert(pair<string,int>(line, pos));
                    pos++;
                    break;
                }
            }
            break;
        }
    }
    return stops;
}

/**
 * @brief counts the number of lines available during the day and the number of lines available during the night
 * @return a pair containing the number of lines available, during the day and during the night
 */
pair<int, int> countDayAndNightLines() {
    ifstream file("./dataset/lines.csv");

    int day = 0, night = 0;
    string aux, code, name, firstLine;

    getline(file, firstLine); // skip the first line

    while(!file.eof()) {
        getline(file, aux);
        code = aux.substr(0, aux.find(','));

        if(code.find('M') == string::npos)
            day++;
        else
            night++;
    }
    return pair<int, int>(day, night);
}
