#include "menu.h"

Menu::Menu() = default;

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
    return options[intInput(0, options.size())-1];
}

void Menu::run() {
    fromStation = -1;
    toStation = -1;

    cout << welcome << endl;

    time = query(dayNightQuery, dayOrNight);
    if (time == "Day"){
        vector<string> dayStops = parser.readDayStops(parser.readDayLines());
        this->stops = parser.mapStopToInt(dayStops);
        this->stopsPositions = parser.readStopsPos(dayStops);
        this->stopsName = getStopName(stops);
    } else {
        vector<string> nightStops = parser.readNightStops(parser.readNightLines());
        this->stops = parser.mapStopToInt(nightStops);
        this->stopsPositions = parser.readStopsPos(nightStops);
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

    if (priority == "Lesser stops" && time == "Day"){
        caseCode = 0;
    } else if (priority == "Lesser route distance" && time == "Day") {
        caseCode = 1;
    } else if (priority == "Lesser stops" && time == "Night") {
        caseCode = 2;
    } else if (priority == "Lesser route distance" && time == "Night") {
        caseCode = 3;
    }
    //Results
    callResults();
}

map<int, string> Menu::getStopName(map<string, int> m) {
    map<int, string> res;
    for(auto x: m) {
        res[x.second] = x.first;
    }
    return res;
}

void Menu::printPath(list<int> path) {
    cout << "You're here -> ";
    for (auto i: path) {
        cout << stopsName[i] << " -> ";
    }
    cout << "Destination." << endl;
}

void Menu::callResults() {
    list<int> path = {};
    int distInt = -2;
    double distDouble = -2;

    if(from == "Place") {
        pair <string, double> closestStation = findClosestStation(fromLatitude, fromLongitude);
        fromStation = stops[closestStation.first];
        cout << "You have to walk " << closestStation.second <<
        "km to " << closestStation.first << "." << endl;
    }
    if(to == "Place") {
        pair <string, double> closestStation = findClosestStation(toLatitude, toLongitude);
        toStation = stops[closestStation.first];
        cout << "After exiting at " << closestStation.first << " you have to walk " <<
        closestStation.second << "km to the desired location." << endl;
    }

    switch (caseCode) {
        case 0:
            graph = parser.parseDayLines(closedStations);
            distInt = graph.bfs_distance(fromStation, toStation);
            if (distInt > 0) {
                cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation]
                     << " will pass " << distInt + 1 << " stations.\n";
            }
            path = graph.bfs_path(fromStation, toStation);
            break;
        case 1:
            graph = parser.parseDayLinesWithDistances(closedStations);
            distDouble = graph.dijkstra_distance(fromStation, toStation);
            if (distDouble > 0) {
                cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation]
                     << " will take " << distDouble << " km.\n";
            }
            path = graph.dijkstra_path(fromStation, toStation);
            break;
        case 2:
            graph = parser.parseNightLines(closedStations);
            distInt = graph.bfs_distance(fromStation, toStation);
            if (distInt > 0) {
                cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation]
                     << " will pass " << distInt + 1 << " stations.\n";
            }
            path = graph.bfs_path(fromStation, toStation);
            break;
        case 3:
            graph = parser.parseNightLinesWithDistances(closedStations);
            distDouble = graph.dijkstra_distance(fromStation, toStation);
            if (distDouble > 0) {
                cout << "From " << stopsName[fromStation] << " to " << stopsName[toStation]
                     << " will take " << distDouble << " km.\n";
            }
            path = graph.dijkstra_path(fromStation, toStation);
            break;
    }
    if (distInt == -1 || distDouble == -1.0 ) {
        cout << "No path found between given stations." << endl;
    }
    if (!path.empty()) printPath (path);
}

pair<string, double> Menu::findClosestStation(double lat, double lon) {
    pair<string, double> closestStation;
    closestStation.second = DBL_MAX;
    for (auto element: stopsPositions) {
        double lat2 = element.second.first;
        double lon2 = element.second.second;
        double distance = haversine(lat,lon,lat2,lon2);
        if (distance < closestStation.second) {
            closestStation.first = element.first;
            closestStation.second = distance;
        }
    }
    return closestStation;
}

/**
 * @brief calculates the distance in kilometers between 2 points given their coordinates
 * @param lat1 the latitude of the first point
 * @param lon1 the longitude of the first point
 * @param lat2 the latitude of the second point
 * @param lon2 the longitude of the second point
 * @return the distance between the 2 points
 */
double Menu::haversine(double lat1, double lon1, double lat2, double lon2) {
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
