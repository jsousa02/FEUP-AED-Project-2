#include <sstream>
#include <fstream>
#include <map>
#include <cmath>
#include <algorithm>

#include "../include/parser.h"

/**
 * @brief calculates the distance in kilometers between 2 points given their coordinates
 * @param lat1 the latitude of the first point
 * @param lon1 the longitude of the first point
 * @param lat2 the latitude of the second point
 * @param lon2 the longitude of the second point
 * @return the distance between the 2 points
 */
double Parser::haversine(double lat1, double lon1, double lat2, double lon2) {
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

vector<string> Parser::readDayLines() {
    ifstream file("dataset/lines.csv");

    string aux, code, name, firstLine;
    vector<string> dayLines;

    getline(file, firstLine); // skip the first line
    while(getline(file, aux)) {
        code = aux.substr(0, aux.find(','));
        if(code.find('M') == string::npos) dayLines.push_back(code);
    }
    return dayLines;
}

vector<string> Parser::readNightLines() {
    ifstream file("dataset/lines.csv");

    string aux, code, name, firstLine;
    vector<string> nightLines;

    getline(file, firstLine); // skip the first line
    while(getline(file, aux)) {
        code = aux.substr(0, aux.find(','));
        if(code.find('M') != string::npos) nightLines.push_back(code);
    }
    return nightLines;
}

vector<string> Parser::readDayStops(vector<string> dayLinesList) {
    vector<string> dayStops;
    for (auto line: dayLinesList){
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, stop;
            getline(linefile, firstline);
            while(getline(linefile, stop)) {
                if (find(dayStops.begin(),dayStops.end(),stop) != dayStops.end()) {
                    continue;
                }
                else
                    dayStops.push_back(stop);
            }
        }
    }
    return dayStops;
}

vector<string> Parser::readNightStops(vector<string> nightLinesList) {
    vector<string> nightStops;
    for (auto line: nightLinesList){
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, stop;
            getline(linefile, firstline);
            while(getline(linefile, stop)) {
                if (find(nightStops.begin(),nightStops.end(),stop) != nightStops.end()) {
                    continue;
                }
                else
                    nightStops.push_back(stop);
            }
        }
    }
    return nightStops;
}

map<string, int> Parser::mapStopToInt(vector<string> stopsVec) {
    map<string, int> stops;
    int pos = 1;
    for (auto line: stopsVec){
        stops.insert(pair<string,int>(line, pos));
        pos++;
    }
    return stops;
}

Graph Parser::parseDayLines() {
    vector<string> lines = readDayLines();
    vector<string> dayStops = readDayStops(lines);
    Graph dayLines(dayStops.size(),true);
    map<string, int> stops = mapStopToInt(dayStops);

    //adding edges
    for (auto line: lines) {
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, currentStop, previousStop;
            getline(linefile, firstline);
            getline(linefile, previousStop);        //first busstop
            while(getline(linefile, currentStop)) { //next busstop
                dayLines.addEdge(stops[previousStop],stops[currentStop], line);
                previousStop = currentStop;
            }
        }
    }
    return dayLines;
}

Graph Parser::parseNightLines() {
    vector<string> lines = readNightLines();
    vector<string> nightStops = readNightStops(lines);
    Graph nightLines(nightStops.size(),true);
    map<string, int> stops = mapStopToInt(nightStops);

    //adding edges
    for (auto line: lines) {
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, currentStop, previousStop;
            getline(linefile, firstline);
            getline(linefile, previousStop);        //first busstop
            while(getline(linefile, currentStop)) {        //second stop
                nightLines.addEdge(stops[previousStop],stops[currentStop], line);
                previousStop = currentStop;
            }
        }
    }
    return nightLines;
}

Graph Parser::parseDayLinesWithDistances() {
    vector<string> lines = readDayLines();
    vector<string> dayStops = readDayStops(lines);
    Graph dayLines(dayStops.size(),true);
    map<string, int> stops = mapStopToInt(dayStops);

    //adding edges
    addEdges(lines, stops, dayLines);
    return dayLines;
}

double Parser::lat(string code) {
    string line;
    ifstream stops("dataset/stops.csv"); // file containing info of all the stops
    while (getline(stops, line)) {
        stringstream stream(line);
        getline(stream, line, ',');

        if (line == code) { // only want the info related to the stops of the current line
            for (int j = 0; j < 4; j++) { // 4 loops to get different information in each
                getline(stream, line, ',');
                if (j == 2) // get the latitude of the stop
                    return stod(line);
            }
            break; // after we find the info of a stop in the file, we can exit the loop
        }
    }
    stops.close();
    return -1;
}

double Parser::lon(string code) {
    string line;
    ifstream stops("dataset/stops.csv"); // file containing info of all the stops
    while (getline(stops, line)) {
        stringstream stream(line);
        getline(stream, line, ',');

        if (line == code) { // only want the info related to the stops of the current line
            for (int j = 0; j < 4; j++) { // 4 loops to get different information in each
                getline(stream, line, ',');
                if (j == 3) // get the longitude of the stop
                    return stod(line);
            }
            break; // after we find the info of a stop in the file, we can exit the loop
        }
    }
    stops.close();

    return -1;
}

Graph Parser::parseNightLinesWithDistances() {
    vector<string> lines = readNightLines();
    vector<string> nightStops = readNightStops(lines);
    Graph nightLines(nightStops.size(),true);
    map<string, int> stops = mapStopToInt(nightStops);

    //adding edges
    addEdges(lines, stops, nightLines);
    return nightLines;
}

void Parser::addEdges(vector<string> lines, map<string, int> stops, Graph &graph) {
    for (auto line: lines) {
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, currentStop, previousStop;
            getline(linefile, firstline);
            getline(linefile, previousStop);        //first busstop

            double lat1, lon1;
            lat1 = lat(previousStop);
            lon1 = lon(previousStop);

            while(getline(linefile, currentStop)) { //next busstop
                double lat2 = lat(currentStop);
                double lon2 = lon(currentStop);
                graph.addEdge(stops[previousStop],stops[currentStop], line, haversine(lat1,lon1,lat2,lon2));
                previousStop = currentStop;
                lat1 = lat2;
                lon1 = lon2;
            }
        }
    }
}