//
//  RandomMock.h
//  rgb
//
//  Created by ntotani on 2014/03/06.
//
//

#ifndef __rgb__RandomMock__
#define __rgb__RandomMock__

#include <vector>
#include "IRandom.h"

class RandomMock : public IRandom {
    std::vector<int> arr;
    int idx;
public:
    RandomMock();
    RandomMock(std::vector<int> arr);
    int next();
};

#endif /* defined(__rgb__RandomMock__) */
