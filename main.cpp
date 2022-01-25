#include <iostream>
#include <fstream>
#include <sstream>

#include "./include/graph.h"

using namespace std;

pair<int, int> countDayAndNightLines();
Graph parseDayLines(Graph &graph);
Graph parseNightLines(Graph &graph);

int main() {
    pair<int, int> n = countDayAndNightLines();
    Graph dayLines(2500, false);
    Graph nightLines(415, false);
    dayLines = parseDayLines(dayLines);
    nightLines = parseNightLines(nightLines);

    cout << nightLines << endl;

    return 0;
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

Graph parseDayLines(Graph &graph) {
    ifstream linesFile("./dataset/lines.csv"); // info of the lines
    string line, firstLine, aux, numOfStops, stopCode, stopLine;
    int pos = 0, numOfLines = 0;

    getline(linesFile, firstLine); // skip the first line

    while(getline(linesFile, line)) {
        stringstream ss(line);

        for(int i = 0; i < 2; i++) { // first loop to get the code, second to get the line's name
            getline(ss, line, ',');
        
            if(i == 0 && line.find('M') == string::npos) { // only lines that are available during the day
                string filename = "./dataset/line_" + line + "_0.csv";
                ifstream lineFile(filename); // file containing the stops of a line
                getline(lineFile, numOfStops);

                for(int k = 0; k < stoi(numOfStops); k++) { // for all the stops of a line

                    getline(lineFile, stopCode);
                    ifstream stops("./dataset/stops.csv"); // file containing info of all the stops

                    while(getline(stops, stopLine)) {
                        stringstream stream(stopLine);
                        getline(stream, stopLine, ',');

                        if(stopLine == stopCode) { // only want the info related to the stops of the current line

                            if(pos != stoi(numOfStops) - 1) // -1 to not make a connection between 2 different lines
                                graph.addEdge(k, k + 1); // create connections between stops in the same line

                            graph.nodes.at(numOfLines).setCode(stopLine);

                            for(int j = 0; j < 4; j++) { // 4 loops to get different information in each
                                getline(stream, stopLine, ',');
                                if(j == 0) // get the name of the stop
                                    graph.nodes.at(numOfLines).setName(stopLine);
                                else if(j == 1) // get the zone of the stop
                                    graph.nodes.at(numOfLines).setZone(stopLine);
                                else if(j == 2) // get the latitude of the stop
                                    graph.nodes.at(numOfLines).setLatitude(stod(stopLine));
                                else if(j == 3) // get the longitude of the stop
                                    graph.nodes.at(numOfLines).setLongitude(stod(stopLine));
                            }
                            numOfLines++; // increase the number of lines checked
                            break; // after we find the info of a stop in the file, we can exit the loop
                        }
                    }
                }
                pos = 0; // auxiliary variable to stop adding an edge between the last stop of a line
                        // and the first stop of the next line, so needs to be reset after checking the stops of a line
            }
        }
    }
    return graph;
}

Graph parseNightLines(Graph &graph) {
    ifstream linesFile("./dataset/lines.csv"); // info of the lines
    string line, firstLine, aux, numOfStops, stopCode, stopLine;
    int pos = 0, numOfLines = 0;

    getline(linesFile, firstLine); // skip the first line

    while(getline(linesFile, line)) {
        stringstream ss(line);

        for(int i = 0; i < 2; i++) { // first loop to get the code, second to get the line's name
            getline(ss, line, ',');

            if(i == 0 && line.find('M') != string::npos) { // only lines that are available during the day
                string filename = "./dataset/line_" + line + "_0.csv";
                ifstream lineFile(filename); // file containing the stops of a line
                getline(lineFile, numOfStops);

                for(int k = 0; k < stoi(numOfStops); k++) { // for all the stops of a line

                    getline(lineFile, stopCode);
                    ifstream stops("./dataset/stops.csv"); // file containing info of all the stops

                    while(getline(stops, stopLine)) {
                        stringstream stream(stopLine);
                        getline(stream, stopLine, ',');

                        if(stopLine == stopCode) { // only want the info related to the stops of the current line

                            if(pos != stoi(numOfStops) - 1) // -1 to not make a connection between 2 different lines
                                graph.addEdge(k, k + 1); // create connections between stops in the same line

                            graph.nodes.at(numOfLines).setCode(stopLine);

                            for(int j = 0; j < 4; j++) { // 4 loops to get different information in each
                                getline(stream, stopLine, ',');
                                if(j == 0) // get the name of the stop
                                    graph.nodes.at(numOfLines).setName(stopLine);
                                else if(j == 1) // get the zone of the stop
                                    graph.nodes.at(numOfLines).setZone(stopLine);
                                else if(j == 2) // get the latitude of the stop
                                    graph.nodes.at(numOfLines).setLatitude(stod(stopLine));
                                else if(j == 3) // get the longitude of the stop
                                    graph.nodes.at(numOfLines).setLongitude(stod(stopLine));
                            }
                            numOfLines++; // increase the number of lines checked
                            break; // after we find the info of a stop in the file, we can exit the loop
                        }
                    }
                }
                pos = 0; // auxiliary variable to stop adding an edge between the last stop of a line
                // and the first stop of the next line, so needs to be reset after checking the stops of a line
            }
        }
    }
    return graph;
}