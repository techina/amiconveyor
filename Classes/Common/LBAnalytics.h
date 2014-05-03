#ifndef leadblow_LBAnalytics_h
#define leadblow_LBAnalytics_h

#include <string>
#include <map>

using namespace std;

class LBAnalytics
{
public:
    static void logEvent(const char* event, map<string, string> param);
    static void startEvent(const char* event, map<string, string> param);
    static void endEvent(const char* event, map<string, string> param);
};

#endif
