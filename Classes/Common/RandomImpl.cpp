//
//  RandomImpl.cpp
//  rgb
//
//  Created by ntotani on 2014/03/06.
//
//

#include <iostream>
#include <stdio.h>
#include <time.h>
#include "RandomImpl.h"

RandomImpl::RandomImpl() {
    srand((unsigned int)time(NULL));
}

int RandomImpl::next() {
    return rand();
}
