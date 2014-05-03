#import "LBAnalytics.h"
#import "Flurry.h"

NSDictionary* convert(map<string, string> val)
{
    NSMutableDictionary* dic = [@{} mutableCopy];
    for (auto& e : val) {
        dic[[NSString stringWithFormat:@"%s", e.first.c_str()]] = [NSString stringWithFormat:@"%s", e.second.c_str()];
    }
    return dic;
}

void LBAnalytics::logEvent(const char *event, map<string, string> param)
{
    if (param.empty()) {
        [Flurry logEvent:[NSString stringWithFormat:@"%s", event]];
    } else {
        [Flurry logEvent:[NSString stringWithFormat:@"%s", event] withParameters:convert(param)];
    }
}

void LBAnalytics::startEvent(const char *event, map<string, string> param)
{
    if (param.empty()) {
        [Flurry logEvent:[NSString stringWithFormat:@"%s", event] timed:YES];
    } else {
        [Flurry logEvent:[NSString stringWithFormat:@"%s", event] withParameters:convert(param) timed:YES];
    }
}

void LBAnalytics::endEvent(const char *event, map<string, string> param)
{
    if (param.empty()) {
        [Flurry endTimedEvent:[NSString stringWithFormat:@"%s", event] withParameters:nil];
    } else {
        [Flurry endTimedEvent:[NSString stringWithFormat:@"%s", event] withParameters:convert(param)];
    }
}
