#include <map>
#include <cmath>

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
 * @brief parses the lines available during the night and sets the weight of the edges as 1
 * @return a graph in which each node is a stop
 */
Graph Parser::parseNightLines() {
    map<string, int> stops = mapStopToInt();
    Graph nightLines(415, true);
    vector<string> stopCodes;
    ifstream linesFile("./dataset/lines.csv");
    string line, firstLine, lineCode, numOfStops, stopCode, stop;

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

/**
 * @brief parses the lines available during the day and sets the weight of the edges as 1
 * @return a graph in which node is a stop
 */
Graph Parser::parseDayLines() {
    map<string, int> stops = mapStopToInt();
    Graph dayLines(2500, true);
    vector<string> stopCodes;
    ifstream linesFile("./dataset/lines.csv");
    string line, firstLine, lineCode, numOfStops, stopCode, stop;
    double lat1, lat2, lon1, lon2;

    getline(linesFile, firstLine); // skip first line

    while(getline(linesFile, line)) {
        stringstream ss(line);
        while(getline(ss, line, ',')) {
            for(int i = 0; i < 2; i++) {
                if(i == 0) // only get the line code
                    lineCode = line;
            }
            if(lineCode.find("M") == string::npos) { // M is not found in the line code
                ifstream dayFile("./dataset/line_" + lineCode + "_0.csv");
                getline(dayFile, numOfStops); // get number of stops

                for(int j = 0; j < stoi(numOfStops); j++) {
                    getline(dayFile, stopCode);
                    stopCodes.push_back(stopCode);
                }
                for(int j = 0; j < stopCodes.size() - 1; j++) {
                    dayLines.addEdge(stops[stopCodes.at(j)], stops[stopCodes.at(j + 1)], lineCode, haversine(lat1, lon1, lat2, lon2));
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
                    dayLines.addEdge(stops[stopCodes.at(k)], stops[stopCodes.at(k + 1)], lineCode, haversine(lat1, lon1, lat2, lon2));
                }
            }
        }
    }
    return dayLines;
}

/**
 * @brief separates the stops of each line in vectors
 */
void Parser::parseDayLinesWithDistances(vector<vector<string>> &stopCodesByLine0, vector<vector<string>> &stopCodesByLine1) {
    map<string, int> stops = mapStopToInt();
    vector<string> stopCodes;
    ifstream linesFile("./dataset/lines.csv");
    string line, firstLine, lineCode, numOfStops, stopCode, stop;

    getline(linesFile, firstLine); // skip first line

    while(getline(linesFile, line)) {
        stringstream ss(line);
        while(getline(ss, line, ',')) {
            for(int i = 0; i < 2; i++) {
                if(i == 0) // only get the line code
                    lineCode = line;
            }
            if(lineCode.find("M") == string::npos) { // M is not found in the line code
                ifstream dayFile("./dataset/line_" + lineCode + "_0.csv");
                getline(dayFile, numOfStops); // get number of stops

                stopCodes.push_back(lineCode);  // add line code to the beginning of the vector

                for(int j = 0; j < stoi(numOfStops); j++) {
                    getline(dayFile, stopCode);
                    stopCodes.push_back(stopCode);
                }
                stopCodesByLine0.push_back(stopCodes);

                ifstream dayFile1("./dataset/line_" + lineCode + "_1.csv");
                getline(dayFile1, numOfStops); // get number of stops

                if(numOfStops == "0")
                    break;
                stopCodes.clear();
                stopCodes.push_back(lineCode);

                for(int k = 0; k < stoi(numOfStops); k++) {
                    getline(dayFile1, stopCode);
                    stopCodes.push_back(stopCode);
                }
                stopCodesByLine1.push_back(stopCodes);
                stopCodes.clear();
            }
            break;
        }
    }
}

/**
 * @brief separates the stops of each line in vectors
 */
void Parser::parseNightLinesWithDistances(vector<vector<string>> &stopCodesByLine0, vector<vector<string>> &stopCodesByLine1) {
    map<string, int> stops = mapStopToInt();
    Graph nightLines(415, true);
    vector<string> stopCodes;
    string line, firstLine, lineCode, numOfStops, stopCode, stop;

    ifstream linesFile("./dataset/lines.csv");

    getline(linesFile, firstLine); // skip first line

    while(getline(linesFile, line)) {
        stringstream ss(line);
        while(getline(ss, line, ',')) {
            for(int i = 0; i < 2; i++) {
                if(i == 0) {
                    lineCode = line;
                    break;
                }
            }
            if(lineCode.find("M") != string::npos) {

                ifstream dayFile("./dataset/line_" + lineCode + "_0.csv");
                getline(dayFile, numOfStops); // get number of stops

                stopCodes.push_back(lineCode);

                for(int j = 0; j < stoi(numOfStops); j++) {
                    getline(dayFile, stopCode);
                    stopCodes.push_back(stopCode);
                }
                stopCodesByLine0.push_back(stopCodes);

                ifstream dayFile1("./dataset/line_" + lineCode + "_1.csv");
                getline(dayFile1, numOfStops); // get number of stops

                if(numOfStops == "0")
                    break;
                stopCodes.clear();
                stopCodes.push_back(lineCode);

                for(int k = 0; k < stoi(numOfStops); k++) {
                    getline(dayFile1, stopCode);
                    stopCodes.push_back(stopCode);
                }
                stopCodesByLine1.push_back(stopCodes);
                stopCodes.clear();
            }
            break;
        }
    }
}

/**
 * @brief calculates the distance between 2 edges and adds them to the graph
 * @param v1 the vector containing the stops of each line separated in vectors
 * @param graph the graph to add the nodes to
 */
void Parser::addDistances(vector<vector<string>> &v1, Graph &graph) {
    map<string, int> stops = mapStopToInt();
    string firstLine, stopInfo, lineCode, zone;
    double lat1 = 0, lat2 = 0, lon1, lon2;

    for(auto v: v1) {
        for(int i = 1; i < v.size() - 1; i++) {
            lineCode = v.at(0);

            ifstream stopsFile("./dataset/stops.csv");
            getline(stopsFile, firstLine); // skip first line

            while(getline(stopsFile, stopInfo)) {
                stringstream ss(stopInfo);
                while(getline(ss, stopInfo, ',') && (lat1 == 0 || lat2 == 0)) {
                    if(stopInfo != v.at(i) && stopInfo != v.at(i + 1))
                        break;
                    if(stopInfo == v.at(i)) {
                        for(int j = 0; j < 4; j++) {
                            getline(ss, stopInfo, ',');
                            if(j == 2)
                                lat1 = stod(stopInfo);
                            else if (j == 3)
                                lon1 = stod(stopInfo);
                        }
                    }
                    else if(stopInfo == v.at(i + 1)) {
                        for(int k = 0; k < 4; k++) {
                            getline(ss, stopInfo, ',');
                            if(k == 2)
                                lat2 = stod(stopInfo);
                            else if(k == 3)
                                lon2 = stod(stopInfo);
                        }
                    }

                }
            }
            graph.addEdge(stops[v.at(i)], stops[v.at(i + 1)], lineCode, haversine(lat1, lon1, lat2, lon2));
        }
    }
}

/**
 * @brief maps a stop code into an integer
 * @return a map in which the key is the stop code and the value is the corresponding integer
 */
map<string, int> Parser::mapStopToInt() {
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
