#ifndef AED_MENU_H
#define AED_MENU_H

#include <map>
#include "parser.h"
#include "float.h"

using namespace std;

class Menu {
    Parser parser;
    map<string, int> stops;
    map<int, string> stopsName;
    map<string, pair<double, double>> stopsPositions;

    string welcome = "Welcome to AEDA Transport assisstant!";
    string stationQuery = "First, where are you? A station or a place?";
    string stationQuery2 = "And you want to go where? A station or a place?";
    string whichStation = "Which station? Input the code, please. (It should look"
                          " something like this: AL3, written in capital letters and numbers only)";
    string coordinatesQuery = "Then I'll need the coordinates. ";
    string latitudeQuery = "Please input the coordinate's latitude.";
    string longitudeQuery = "Please input the coordinate's longitude.";
    string options = "Now about your travel options. ";
    string dayNightQuery = "Are you traveling at day? Or night?";
    string priorityQuery = "And what is your priority?";
    string startingMenuString = "What do you want to do?\n"
                          "1 - Close stations\n"
                          "2 - Search route\n";
    string closeStationOrEnd = "Select one below\n"
                               "1 - Close a station\n"
                               "2 - End closing stations\n";

    vector <string> priorities = {"Lesser stops", "Lesser route distance"};
    vector <string> stationOrPlace = {"Station", "Place"};
    vector <string> dayOrNight = {"Day", "Night"};
    vector <string> closedStations;

    Graph graph;

    double fromLatitude{};
    double fromLongitude{};
    double toLatitude{};
    double toLongitude{};
    string from;
    string to;
    string time;
    string priority;
    int fromStation{};
    int toStation{};
    int caseCode{};

    string query(string text, vector<string> options);
    map<int, string> getStopName(map<string, int> m);
    void printPath(list<int> path);
    pair<string, double> findClosestStation(double lat, double lon);
    double haversine(double lat1, double lon1, double lat2, double lon2);
public:
    Menu();
    void startingMenu();
    void closeStationQuery();
    int intInput(int min, int max);
    pair<int, string> stationInput();
    double coordinatesInput();
    void runSearchRoute();
    void callResults();
};


#endif //AED_MENU_H
