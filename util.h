#ifndef DEMO001_UTIL_H
#define DEMO001_UTIL_H

#include <random>
#include "typedef.h"

namespace Util {
    namespace {
        std::minstd_rand randomGen;
    }

    static int64 rand() {
        return randomGen();
    }

    static int32 randRange(int32 min, int32 max) {
        return randomGen() % (max - min) + min;
    }

    static int32 randRange(int32 max) {
        return randRange(0, max);
    }
}

#endif //DEMO001_UTIL_H
