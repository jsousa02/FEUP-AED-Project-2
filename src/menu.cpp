#include "menu.h"

Menu::Menu() = default;

/**
 * @brief asks for an integer input between the given options and validates it. repeats if invalid
 * @param min lower limit of interval
 * @param max upper limit of interval
 * @return return the validated int
 */
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
 * @brief ask for station and checks if input string is a station code that exist
 * @return the validated station's node number
 */
pair<int, string> Menu::stationInput() {
    string input;
    int inputCode; // 0 means input ok, 1 means input not in station code format, 2 means station not found;
    pair<int, string> output;
    do {
        inputCode = 0;
        cin >> input;
        for (auto c : input) {
            if (!isdigit(c) && !isupper(c)) inputCode = 1;
        }
        if (inputCode == 0 && stops.find(input) == stops.end()) inputCode = 2;
        if (inputCode == 1) cout << "That's not the station code format of input. Please try again." << endl;
        if (inputCode == 2) cout << "Station not found. Please try again." << endl;
        if (inputCode == 0) {
            output.second = input;
            output.first = stops[input];
        }
    } while (inputCode != 0);
    return output;
}

/**
 * @brief ask for coordinate and validate that it's a double
 * @return the validated double
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

/**
 * @brief queries the user for an option between the given ones
 * @param text the query text
 * @param options a vector containing each option
 * @return the string corresponding to the selected option
 */
string Menu::query(string text, vector<string> options) {
    cout << text << endl;
    for (int i = 1; i <= options.size(); i++) {
        cout << i << " - " << options[i - 1] << endl;
    }
    return options[intInput(0, options.size())-1];
}

/**
 * @brief asks the user, until he says he is finished, whether he wants to close another station or finish
 */
void Menu::closeStationQuery() {
    int option;
    closedStations.clear();

    cout << closeStationOrEnd;
    option = intInput(1, 2);
    if (option == 1) {
        do {
            cout << whichStation << endl;
            closedStations.push_back(stationInput().second);
            cout << "Register another as closed?\n"
                    "1 - Yes\n"
                    "2 - Finish\n";
            option = intInput(1, 2);
        } while (option == 1);
    }
}

/**
 * @brief run the menu, collecting information needed for the user to perform the desired action
 * which can be the calculation of a mst distance or a desired search route
 */
void Menu::run() {
    fromStation = -1;
    toStation = -1;
    int option;

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
        fromStation = stationInput().first;
    } else {
        cout << coordinatesQuery << endl;
        cout << latitudeQuery << endl;
        fromLatitude = coordinatesInput();
        cout << longitudeQuery << endl;
        fromLongitude = coordinatesInput();
    }
    cout << "Select an option\n"
            "1 - Search route\n"
            "2 - See the shortest distance to go through all stations from a starting point\n";
    option = intInput(1, 2);

    if (option == 1) {
        to = query(stationQuery2, stationOrPlace);
        if (to == "Station") {
            cout << whichStation << endl;
            toStation = stationInput().first;
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
        closeStationQuery();
        callResults();
    } else {
        closedStations.clear();
        string station;
        if(from == "Place") {
            pair <string, double> closestStation = findClosestStation(fromLatitude, fromLongitude);
            fromStation = stops[closestStation.first];
            station = closestStation.first;
            cout << "You have to walk " << closestStation.second <<
                 "km to " << closestStation.first << "." << endl;
        } else {
            station = stopsName[fromStation];
        }
        if (time == "Day") {
            graph = parser.parseDayLinesWithDistances(closedStations);
        } else {
            graph = parser.parseNightLinesWithDistances(closedStations);
        }
        cout << "Starting at station " <<
        station << " you would go through " << graph.mst_distance(fromStation) <<
        "km to go across all stations." << endl;
    }
}

/**
 * @brief maps the m map backwards
 * @param m map to be mapped backwards
 * @return the resulting map (backwards m)
 */
map<int, string> Menu::getStopName(map<string, int> m) {
    map<int, string> res;
    for(auto x: m) {
        res[x.second] = x.first;
    }
    return res;
}

/**
 * @brief prints a pathway from where you are through the given path
 * @param path given path to be printed
 */
void Menu::printPath(list<int> path) {
    cout << "You're here -> ";
    for (auto i: path) {
        cout << stopsName[i] << " -> ";
    }
    cout << "Destination." << endl;
}

/**
 * @brief do the calculations corresponding to the user's options and shows the resulting searched route
 */
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

/**
 * @brief finds the closest station to a given position
 * @param lat latitude of the position
 * @param lon longitude of the position
 * @return a pair containing the station's code and the distance to the position respectively
 */
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
