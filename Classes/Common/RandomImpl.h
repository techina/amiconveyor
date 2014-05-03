//
//  RandomImpl.h
//  rgb
//
//  Created by ntotani on 2014/03/06.
//
//

#ifndef __rgb__RandomImpl__
#define __rgb__RandomImpl__

#include "IRandom.h"

class RandomImpl : public IRandom {
public:
    RandomImpl();
    int next();
};

#endif /* defined(__rgb__RandomImpl__) */
