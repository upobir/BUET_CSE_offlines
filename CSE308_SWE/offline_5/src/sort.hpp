#pragma once

#include <vector>

#include "mergesort.hpp"
#include "differentsort.hpp"
#include "identity.hpp"

template<typename T>
void sort(std::vector<T>& input){
    mergeSort(input);
    //differentsort(input);
    //identity(input);
}