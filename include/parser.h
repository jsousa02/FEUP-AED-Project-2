#ifndef AED_PARSER_H
#define AED_PARSER_H

#include "graph.h"

class Parser {
private:
    double haversine(double lat1, double lon1, double lat2, double lon2);
    double lat(string code);
    double lon(string code);
    void addEdges(vector<string> lines, map<string, int> stops, Graph &graph);
public:
    Parser() = default;
    map<string, int> mapStopToInt(vector<string> stopsVec);
    vector<string> readDayLines();
    vector<string> readNightLines();
    vector<string> readDayStops(vector<string> dayLinesList);
    vector<string> readNightStops(vector<string> nightLinesList);
    Graph parseDayLines();
    Graph parseNightLines();
    Graph parseDayLinesWithDistances();
    Graph parseNightLinesWithDistances();

};



#endif //AED_PARSER_H
