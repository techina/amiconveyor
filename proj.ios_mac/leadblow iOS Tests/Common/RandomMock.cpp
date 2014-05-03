//
//  RandomMock.cpp
//  rgb
//
//  Created by ntotani on 2014/03/06.
//
//

#include "RandomMock.h"

RandomMock::RandomMock():idx(0) {
    arr = {0};
}

RandomMock::RandomMock(std::vector<int> arr):arr(arr),idx(0) {
}

int RandomMock::next() {
    int ret = arr[idx];
    idx = (idx + 1) % arr.size();
    return ret;
}
