#ifndef AED_PARSER_H
#define AED_PARSER_H

#include "graph.h"

class Parser {
private:
    double haversine(double lat1, double lon1, double lat2, double lon2);

public:
    Parser() {}
    map<string, int> mapStopToInt();
    Graph parseDayLines();
    Graph parseNightLines();
    void parseDayLinesWithDistances(vector<vector<string>> &stopCodesByLine0, vector<vector<string>> &stopCodesByLine1);
    void addDistances(vector<vector<string>> &v1, Graph &graph);
    void parseNightLinesWithDistances(vector<vector<string>> &stopCodesByLine0, vector<vector<string>> &stopCodesByLine1);
};



#endif //AED_PARSER_H
