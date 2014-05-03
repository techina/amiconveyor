#ifndef leadblow_Purchase_h
#define leadblow_Purchase_h

#include <string>
#include <functional>

using namespace std;

class Purchase {
public:
    static void purchase(const char* productID, function<void(string)> callback);
};

#endif
