#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>

#include "./include/graph.h"
#include "./include/parser.h"

using namespace std;

map<string, int> mapStopToInt();
double haversine1(double lat1, double lon1, double lat2, double lon2);
vector<string> findClosestStop(double lat, double lon, double dist);

int main() {
    Parser parser;
    //Graph dayLines = parser.parseDayLinesWithDistances();
    //Graph nightLines = parser.parseNightLinesWithDistances();
    Graph dayLines = parser.parseDayLines();
    map<string, int> stops = mapStopToInt();

    //vector<string> nearbyStops = findClosestStop(41.14969203, -8.611312751, 0.1);
    //for(auto i: nearbyStops)
    //    cout << i << endl;

    cout << dayLines.bfs_distance(stops["INF1"], stops["PASS"]) << endl;

    cout << "done\n";
    return 0;
}

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

vector<string> findClosestStop(double lat, double lon, double dist) {
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
        distance = haversine1(lat, lon, lat2, lon2);
        if(distance <= dist)
            closestStops.push_back(stopCode);
    }
    return closestStops;
}


double haversine1(double lat1, double lon1, double lat2, double lon2) {
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