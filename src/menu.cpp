//
// Created by df001 on 2022/1/30.
//

#include "menu.h"
#include <map>

Menu::Menu() {}

int Menu::intInput(int min, int max) {
    string input;
    bool validInput;
    int output;
    do {
        validInput = true;
        cin >> input;
        try {
            output = stoi(input);
        } catch (invalid_argument &e) {
            validInput = false;
        }
        if (output < min || output > max) {
            validInput = false;
        }
        if (!validInput) cout << "Invalid option. Please try again." << endl;
    } while (!validInput);
    return output;
}
/**
 * @brief ask for station and checks if input station exists
 * @return station's node number
 */
int Menu::stationInput() {
    string input;
    int inputCode; // 0 means input ok, 1 means input not in station code format, 2 means station not found;
    int output;
    do {
        inputCode = 0;
        cin >> input;
        for (auto c : input) {
            if (!isdigit(c) && !isupper(c)) inputCode = 1;
        }
        if (inputCode == 0 && stops.find(input) == stops.end()) inputCode = 2;
        if (inputCode == 1) cout << "That's not the station code format of input. Please try again." << endl;
        if (inputCode == 2) cout << "Station not found. Please try again." << endl;
        if (inputCode == 0) output = stops[input];
    } while (inputCode != 0);
    return output;
}
/**
 * @brief ask for coordinate
 * @return
 */
double Menu::coordinatesInput() {
    string input;
    double output;
    bool validInput;
    do {
        validInput = true;
        cin >> input;
        try {
            output = stod(input);
        } catch (invalid_argument& e) {
            validInput = false;
        }
        if (!validInput) cout << "That is not a coordinate value. Please try again.";
    } while (!validInput);
    return output;
}

string Menu::query(string text, vector<string> options) {
    cout << text << endl;
    for (int i = 1; i <= options.size(); i++) {
        cout << i << " - " << options[i - 1] << endl;
    }
    return options[intInput(0, options.size()) - 1];
}

void Menu::run() {
    fromStation = -1;
    toStation = -1;

    cout << welcome << endl;

    time = query(dayNightQuery, dayOrNight);
    if (time == "Day"){
        this->stops = parser.mapStopToInt(parser.readDayStops(parser.readDayLines()));
        this->stopsName = getStopName(stops);
    } else {
        this->stops = parser.mapStopToInt(parser.readNightStops(parser.readNightLines()));
        this->stopsName = getStopName(stops);
    }

    from = query(stationQuery, stationOrPlace);
    if (from == "Station") {
        cout << whichStation << endl;
        fromStation = stationInput();
    } else {
        cout << coordinatesQuery << endl;
        cout << latitudeQuery << endl;
        fromLatitude = coordinatesInput();
        cout << longitudeQuery << endl;
        fromLongitude = coordinatesInput();
    }

    to = query(stationQuery2, stationOrPlace);
    if (to == "Station") {
        cout << whichStation << endl;
        toStation = stationInput();
    } else {
        cout << coordinatesQuery << endl;
        cout << latitudeQuery << endl;
        toLatitude = coordinatesInput();
        cout << longitudeQuery << endl;
        toLongitude = coordinatesInput();
    }

    cout << options << endl;
    priority = query(priorityQuery, priorities);

    //results
    if (priority == "Lesser stops" && time == "Day"){
        Graph graph = parser.parseDayLines();
        //distance
        int distance = graph.bfs_distance(fromStation, toStation);
        if (distance < 0) {
            cout << "No path found between given stations." << endl;
        } else {
            cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation] << " will pass " << distance
                 << " stations.\n";
        }
        //path
        list<int> path = graph.bfs_path(fromStation, toStation);
        if (!path.empty()) printPath (path);
    } else if (priority == "Lesser route distance" && time == "Day") {
        Graph graph = parser.parseDayLinesWithDistances();
        //distance
        double distance = graph.dijkstra_distance(fromStation, toStation);
        if (distance < 0) {
            cout << "No path found between given stations." << endl;
        } else {
            cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation]
            << " will take " << distance << " km.\n";
        }
        //path
        list<int> path = graph.dijkstra_path(fromStation, toStation);
        if (!path.empty()) printPath (path);
    } else if (priority == "Lesser stops" && time == "Night") {
        Graph graph = parser.parseNightLines();
        //distance
        int distance = graph.bfs_distance(fromStation, toStation);
        if (distance < 0) {
            cout << "No path found between given stations." << endl;
        } else {
            cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation] << " will pass " << distance
                 << " stations.\n";
        }
        //path
        list<int> path = graph.bfs_path(fromStation, toStation);
        if (!path.empty()) printPath (path);
    } else if (priority == "Lesser route distance" && time == "Night") {
        Graph graph = parser.parseDayLinesWithDistances();
        //distance
        double distance = graph.dijkstra_distance(fromStation, toStation);
        if (distance < 0) {
            cout << "No path found between given stations." << endl;
        } else {
            cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation]
            << " will take " << distance << " km.\n";
        }
        //path
        list<int> path = graph.dijkstra_path(fromStation, toStation);
        if (!path.empty()) printPath (path);
    }
}

map<int, string> Menu::getStopName(map<string, int> m) {
    map<int, string> res;
    for(auto x: m) {
        res[x.second] = x.first;
    }
    return res;
}

void Menu::printPath(list<int> path) {
    for (auto i: path) {
        cout << stopsName[i] << endl;
    }
}