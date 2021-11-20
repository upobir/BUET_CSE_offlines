#pragma once

#include <random>

class RandGen{
public:
    static void seed(size_t x);
    static int getNumber(int lo, int hi);

private:
    RandGen() = default;
    static std::mt19937 engine;
};