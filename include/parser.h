#ifndef AED_PARSER_H
#define AED_PARSER_H

#include "graph.h"

class Parser {
private:
    double haversine(double lat1, double lon1, double lat2, double lon2);
    map<string, int> mapStopToInt();

public:
    Parser() {}
    Graph parseDayLines();
    Graph parseNightLines();
    Graph parseDayLinesWithDistances();
    Graph parseNightLinesWithDistances();
};



#endif //AED_PARSER_H
