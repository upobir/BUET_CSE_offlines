#pragma once

#include <vector>
#include <numeric>

template<typename T>
void differentsort(std::vector<T>& container){
    std::iota(container.begin(), container.end(), 0);
}