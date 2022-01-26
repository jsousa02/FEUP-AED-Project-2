#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>

#include "./include/graph.h"

using namespace std;

Graph parseDayLines();
Graph parseNightLines();
map<string, int> mapStopToInt();

int main() {
    Graph dayLines = parseDayLines();
    Graph nightLines = parseNightLines();
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

Graph parseDayLines() {
    map<string, int> stops = mapStopToInt();
    Graph dayLines(2500, true);
    vector<string> stopCodes;
    ifstream linesFile("./dataset/lines.csv");
    string line, firstLine, lineCode, numOfStops, stopCode;

    getline(linesFile, firstLine); // skip first line

    while(getline(linesFile, line)) {
        stringstream ss(line);
        while(getline(ss, line, ',')) {
            for(int i = 0; i < 2; i++) {
                if(i == 0)
                    lineCode = line;
            }
            if(lineCode.find("M") == string::npos) {
                ifstream dayFile("./dataset/line_" + lineCode + "_0.csv");
                getline(dayFile, numOfStops); // get number of stops

                for(int j = 0; j < stoi(numOfStops); j++) {
                    getline(dayFile, stopCode);
                    stopCodes.push_back(stopCode);
                }
                for(int j = 0; j < stopCodes.size() - 1; j++) {
                    dayLines.addEdge(stops[stopCodes.at(j)], stops[stopCodes.at(j + 1)], lineCode);
                }

                ifstream dayFile1("./dataset/line_" + lineCode + "_1.csv");
                getline(dayFile1, numOfStops); // get number of stops
                if(numOfStops == "0")
                    break;
                stopCodes.clear();

                for(int k = 0; k < stoi(numOfStops); k++) {
                    getline(dayFile1, stopCode);
                    stopCodes.push_back(stopCode);
                }
                for(int k = 0; k < stopCodes.size() - 1; k++) {
                    dayLines.addEdge(stops[stopCodes.at(k)], stops[stopCodes.at(k + 1)], lineCode);
                }
            }
        }
    }
    return dayLines;
}

Graph parseNightLines() {
    map<string, int> stops = mapStopToInt();
    Graph nightLines(415, true);
    vector<string> stopCodes;
    ifstream linesFile("./dataset/lines.csv");
    string line, firstLine, lineCode, numOfStops, stopCode;

    getline(linesFile, firstLine); // skip first line

    while(getline(linesFile, line)) {
        stringstream ss(line);
        while(getline(ss, line, ',')) {
            for(int i = 0; i < 2; i++) {
                if(i == 0)
                    lineCode = line;
            }

            if(lineCode.find("M") != string::npos) {

                ifstream dayFile("./dataset/line_" + lineCode + "_0.csv");
                getline(dayFile, numOfStops); // get number of stops

                for(int j = 0; j < stoi(numOfStops); j++) {
                    getline(dayFile, stopCode);
                    stopCodes.push_back(stopCode);
                }
                for(int j = 0; j < stopCodes.size() - 1; j++) {
                    nightLines.addEdge(stops[stopCodes.at(j)], stops[stopCodes.at(j + 1)], lineCode);
                }

                ifstream dayFile1("./dataset/line_" + lineCode + "_1.csv");
                getline(dayFile1, numOfStops); // get number of stops
                if(numOfStops == "0")
                    break;
                stopCodes.clear();

                for(int k = 0; k < stoi(numOfStops); k++) {
                    getline(dayFile1, stopCode);
                    stopCodes.push_back(stopCode);
                }
                for(int k = 0; k < stopCodes.size() - 1; k++) {
                    nightLines.addEdge(stops[stopCodes.at(k)], stops[stopCodes.at(k + 1)], lineCode);
                }
            }
        }
    }
    return nightLines;
}

double haversine(double lat1, double long1, double lat2, double long2) {

    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (long2 - long1) * M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // formula
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
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
