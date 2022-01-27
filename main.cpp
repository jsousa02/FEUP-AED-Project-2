#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <iomanip>
#include "./include/graph.h"

using namespace std;

pair<int, int> countDayAndNightLines();
pair<int, int> countDayAndNightStops();

vector<string> readDayLines();
vector<string> readNightLines();

vector<string> readDayStops(vector<string> dayLinesList);
vector<string> readNightStops(vector<string> nightLinesList);

Graph parseDayLines(Graph &graph);
Graph parseNightLines(Graph &graph);

int getIndex(vector<string> basicString, string K);

int main() {
    //pair<int, int> n = countDayAndNightLines();

    pair<int, int> n = countDayAndNightStops();
    //cout << n.first << endl << n.second << endl; //day 2454 and night 747

    Graph dayLines(n.first, true);
    Graph nightLines(n.second, true);

    vector<string> dayLinesList = readDayLines();
    vector<string> nightLinesList = readNightLines();
    //cout << dayLinesList.size() << endl << nightLinesList.size() << endl;

    vector<string> dayStops = readDayStops(dayLinesList);
    vector<string> nightStops = readNightStops(nightLinesList);
    //cout << dayStops.size() << endl << nightStops.size() << endl;

    dayLines = parseDayLines(dayLines);
    nightLines = parseNightLines(nightLines);

/*
    cout << "During day" << endl;
    cout << dayLines;
    cout << "During night" << endl;
    cout << nightLines;
*/
    cout << fixed << setprecision(2) << "Distancia desde " << dayLines.nodes.at(5).getName() << " ate " << dayLines.nodes.at(2454).getName()
    << " e igual a " <<  dayLines.dijkstra_distance(5, 2454) << "km?" << endl;
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

pair<int, int> countDayAndNightStops() {
    ifstream file("dataset/lines.csv");

    string aux, code, name, firstLine;
    list<string> days, nights;

    getline(file, firstLine); // skip the first line
    while(getline(file, aux)) {
        code = aux.substr(0, aux.find(','));

        if(code.find('M') == string::npos)
            days.push_back(code);
        else
            nights.push_back(code);
    }

    set<string> dayStops;
    set<string> nightStops;
    for (int i = 0; i < 2; i++) {
        for (auto code : days){
            string filename = "dataset/line_" + code + "_" + to_string(i) + ".csv";
            ifstream linefile(filename);
            string stop;

            getline(linefile, firstLine);
            while(getline(linefile, stop)){
                dayStops.insert(stop);
            }
            linefile.close();
        }

        for (auto code : nights){
            string filename = "dataset/line_" + code + "_" + to_string(i) + ".csv";
            ifstream linefile(filename);
            string stop;

            getline(linefile, firstLine);
            while(getline(linefile, stop)){
                nightStops.insert(stop);
            }
            linefile.close();
        }
    }

    return pair<int, int>(dayStops.size(), nightStops.size());
}

Graph parseDayLines(Graph &graph) {
    vector<string> dayStops = readDayStops(readDayLines());
    for (int i = 0; i < dayStops.size(); i++) {
        string stopLine;
        ifstream stops("dataset/stops.csv"); // file containing info of all the stops
        while (getline(stops, stopLine)) {
            stringstream stream(stopLine);
            getline(stream, stopLine, ',');

            if (stopLine == dayStops.at(i)) { // only want the info related to the stops of the current line
                graph.nodes.at(i+1).setCode(stopLine);

                for (int j = 0; j < 4; j++) { // 4 loops to get different information in each
                    getline(stream, stopLine, ',');
                    if (j == 0) // get the name of the stop
                        graph.nodes.at(i+1).setName(stopLine);
                    else if (j == 1) // get the zone of the stop
                        graph.nodes.at(i+1).setZone(stopLine);
                    else if (j == 2) // get the latitude of the stop
                        graph.nodes.at(i+1).setLatitude(stod(stopLine));
                    else if (j == 3) // get the longitude of the stop
                        graph.nodes.at(i+1).setLongitude(stod(stopLine));
                }
                break; // after we find the info of a stop in the file, we can exit the loop
            }
        }
        stops.close();
    }
    //addedges
    vector<string> dayLines = readDayLines();
    for (auto line : dayLines) {
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, stop, previousStop;
            getline(linefile, firstline);
            getline(linefile, previousStop);        //first busstop
            while(getline(linefile, stop)) {        //second stop
                graph.addEdge(getIndex(dayStops, previousStop), getIndex(dayStops, stop));
                previousStop = stop;
            }
        }
    }
    return graph;
}

Graph parseNightLines(Graph &graph) {
    vector<string> nightStops = readNightStops(readNightLines());
    for (int i = 0; i < nightStops.size(); i++) {
        string stopLine;
        ifstream stops("dataset/stops.csv"); // file containing info of all the stops
        while (getline(stops, stopLine)) {
            stringstream stream(stopLine);
            getline(stream, stopLine, ',');

            if (stopLine == nightStops.at(i)) { // only want the info related to the stops of the current line
                graph.nodes.at(i+1).setCode(stopLine);

                for (int j = 0; j < 4; j++) { // 4 loops to get different information in each
                    getline(stream, stopLine, ',');
                    if (j == 0) // get the name of the stop
                        graph.nodes.at(i+1).setName(stopLine);
                    else if (j == 1) // get the zone of the stop
                        graph.nodes.at(i+1).setZone(stopLine);
                    else if (j == 2) // get the latitude of the stop
                        graph.nodes.at(i+1).setLatitude(stod(stopLine));
                    else if (j == 3) // get the longitude of the stop
                        graph.nodes.at(i+1).setLongitude(stod(stopLine));
                }
                break; // after we find the info of a stop in the file, we can exit the loop
            }
        }
        stops.close();
    }
    //addedges
    vector<string> nightLines = readNightLines();
    for (auto line : nightLines) {
        for (int i = 0; i < 2; i++) {
            string filename = "dataset/line_"+line+"_"+ to_string(i)+".csv";
            ifstream linefile(filename);

            string firstline, stop, previousStop;
            getline(linefile, firstline);
            getline(linefile, previousStop);        //first busstop
            while(getline(linefile, stop)) {        //second stop
                graph.addEdge(getIndex(nightStops, previousStop), getIndex(nightStops, stop));
                previousStop = stop;
            }
        }
    }
    return graph;
}

int getIndex(vector<string> v, string K)
{
    auto it = find(v.begin(), v.end(), K);
    if (it != v.end()){
        int index = it - v.begin();
        return index + 1;
    }
    else{
        return -1;
    }
}

/*
Graph parseDayLines(Graph &graph) {
    ifstream linesFile("dataset/lines.csv"); // info of the lines
    string line, firstLine, aux, numOfStops, stopCode, stopLine;
    int pos = 0, numOfLines = 0;

    getline(linesFile, firstLine); // skip the first line

    while(getline(linesFile, line)) {
        stringstream ss(line);

        for(int i = 0; i < 2; i++) { // first loop to get the code, second to get the line's name
            getline(ss, line, ',');
        
            if(i == 0 && line.find('M') == string::npos) { // only lines that are available during the day
                string filename = "dataset/line_" + line + "_0.csv";
                ifstream lineFile(filename); // file containing the stops of a line
                getline(lineFile, numOfStops);

                for(int k = 0; k < stoi(numOfStops); k++) { // for all the stops of a line

                    getline(lineFile, stopCode);
                    ifstream stops("dataset/stops.csv"); // file containing info of all the stops

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
}*/

/*Graph parseNightLines(Graph &graph) {
    ifstream linesFile("dataset/lines.csv"); // info of the lines
    string line, firstLine, aux, numOfStops, stopCode, stopLine;
    int pos = 0, numOfLines = 0;

    getline(linesFile, firstLine); // skip the first line

    while(getline(linesFile, line)) {
        stringstream ss(line);

        for(int i = 0; i < 2; i++) { // first loop to get the code, second to get the line's name
            getline(ss, line, ',');

            if(i == 0 && line.find('M') != string::npos) { // only lines that are available during the day
                string filename = "dataset/line_" + line + "_0.csv";
                ifstream lineFile(filename); // file containing the stops of a line
                getline(lineFile, numOfStops);

                for(int k = 0; k < stoi(numOfStops); k++) { // for all the stops of a line

                    getline(lineFile, stopCode);
                    ifstream stops("dataset/stops.csv"); // file containing info of all the stops

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
}*/

vector<string> readDayLines() {
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

vector<string> readNightLines() {
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

vector<string> readDayStops(vector<string> dayLinesList) {
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

vector<string> readNightStops(vector<string> nightLinesList) {
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
