//
// Created by df001 on 2022/1/30.
//

#ifndef AED_MENU_H
#define AED_MENU_H

#include <map>
#include "parser.h"

using namespace std;

class Menu {
    Parser parser;
    map<string, int> stops;
    map<int, string> stopsName;

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

    vector <string> priorities = {"Lesser stops", "Lesser route distance"};
    vector <string> stationOrPlace = {"Station", "Place"};
    vector <string> dayOrNight = {"Day", "Night"};

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

    string query(string text, vector<string> options);
    map<int, string> getStopName(map<string, int> m);
    void printPath(list<int> path);
public:
    Menu();
    int intInput(int min, int max);
    int stationInput();
    double coordinatesInput();
    void run();
};


#endif //AED_MENU_H
