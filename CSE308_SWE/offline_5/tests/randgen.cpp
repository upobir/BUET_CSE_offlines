#include "randgen.h"

std::mt19937 RandGen::engine(0);

void RandGen::seed(size_t x) {
    engine.seed(x);
}

int RandGen::getNumber(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(engine);
}
