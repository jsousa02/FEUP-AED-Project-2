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

/**
 * @brief find code of Lines that work during daytime from "Lines" file
 * @return list with code of those Lines that work during daytime
 */
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

/**
 * @brief find Lines that work at nighttime from file
 * @return list with code of those Lines that work at nighttime
 */
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

/**
 * @brief find Stops that work during daytime from file
 * @param dayLinesList vector of code of Lines
 * @return list with codes of those stops that work during daytime
 */
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

/**
 * @brief find Stops that work at nighttime from file
 * @param dayLinesList vector of code of Lines
 * @return list with codes of those stops that work at nighttime
 */
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

/**
 * @brief maps Stops code to an unique integer value
 * @param stopsVec vector of stops
 * @return mapped Stops
 */
map<string, int> Parser::mapStopToInt(vector<string> stopsVec) {
    map<string, int> stops;
    int pos = 1;
    for (auto line: stopsVec){
        stops.insert(pair<string,int>(line, pos));
        pos++;
    }
    return stops;
}

/**
 * @brief parses the lines available during daytime and sets the weight of the edges as 1
 * @param closedStations vector that contains Code of closed stations
 * @return a graph in which each node is a stop
 */
Graph Parser::parseDayLines(vector<string> closedStations) {
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

    for (auto closedStation: closedStations) {
        dayLines.closeNode(stops[closedStation]);
    }

    return dayLines;
}

/**
 * @brief parses the lines available during the night and sets the weight of the edges as 1
 * @param closedStations vector that contains Code of closed stations
 * @return a graph in which each node is a stop
 */
Graph Parser::parseNightLines(vector<string> closedStations) {
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

    for (auto closedStation: closedStations) {
        nightLines.closeNode(stops[closedStation]);
    }
    return nightLines;
}

/**
 * @brief parses the lines available during daytime and sets the weight of the edges as distance between node and its destination
 * @param closedStations vector that contains Code of closed stations
 * @return a graph in which each node is a stop
 */
Graph Parser::parseDayLinesWithDistances(vector<string> closedStations) {
    vector<string> lines = readDayLines();
    vector<string> dayStops = readDayStops(lines);
    Graph dayLines(dayStops.size(),true);
    map<string, int> stops = mapStopToInt(dayStops);

    //adding edges
    addEdges(lines, stops, dayLines);
    for (auto closedStation: closedStations) {
        dayLines.closeNode(stops[closedStation]);
    }
    return dayLines;
}

/**
 * @brief parses the lines available during the night and sets the weight of the edges as distance between node and its destination
 * @param closedStations vector that contains Code of closed stations
 * @return a graph in which each node is a stop
 */
Graph Parser::parseNightLinesWithDistances(vector<string> closedStations) {
    vector<string> lines = readNightLines();
    vector<string> nightStops = readNightStops(lines);
    Graph nightLines(nightStops.size(),true);
    map<string, int> stops = mapStopToInt(nightStops);

    //adding edges
    addEdges(lines, stops, nightLines);
    for (auto closedStation: closedStations) {
        nightLines.closeNode(stops[closedStation]);
    }
    return nightLines;
}

/**
 * @brief adds edges to nodes of graph
 * @param lines vector that contains code of line
 * @param stops map that has unique integer value (node's number) for each stop
 * @param graph where edges will be added
 */
void Parser::addEdges(vector<string> lines, map<string, int> stops, Graph &graph) {
    for (auto line: lines) {
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, currentStop, previousStop;
            getline(linefile, firstline);
            getline(linefile, previousStop);        //first busstop

            pair<double, double> pos1 = pos(previousStop);
            double lat1 = pos1.first, lon1 = pos1.second;

            while(getline(linefile, currentStop)) { //next busstop
                pair<double, double> pos2 = pos(currentStop);
                double lat2 = pos2.first;
                double lon2 = pos2.second;
                graph.addEdge(stops[previousStop],stops[currentStop], line, haversine(lat1,lon1,lat2,lon2));
                previousStop = currentStop;
                lat1 = pos2.first;
                lon1 = pos2.second;
            }
        }
    }
}

/**
 * @brief reads coordinates from stops file
 * @param code code of stop to find coordinate
 * @return pair formed by: latitude, longitude
 */
pair<double, double> Parser::pos(string code) {
    pair<double, double> res;
    string line;
    ifstream stops("dataset/stops.csv"); // file containing info of all the stops
    while (getline(stops, line)) {
        stringstream stream(line);
        getline(stream, line, ',');

        if (line == code) { // only want the info related to the stops of the current line
            for (int j = 0; j < 4; j++) { // 4 loops to get different information in each
                getline(stream, line, ',');
                if (j == 2) // get the latitude of the stop
                    res.first = stod(line);
                else if (j == 3) // get the longitude of the stop
                    res.second = stod(line);
            }
            break; // after we find the info of a stop in the file, we can exit the loop
        }
    }
    return res;
}

/**
 * @brief maps Station Codes to corresponding pairs of latitude and longitude
 * @param stopsInLine vector that contains Station Codes
 * @return map with Station Codes as keys and coordinate pairs as values
 */
map<string, pair<double, double>> Parser::readStopsPos(vector<string> stopsInLine) {
    map<string, pair<double, double>> result;
    for (auto stop: stopsInLine) {
        pair<double,double> position = pos(stop);
        pair<string, pair <double, double>> toInsert (stop, position);
        result.insert(toInsert);
    }
    return result;
}
