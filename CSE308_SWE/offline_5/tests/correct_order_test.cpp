#include "doctest.h"
#include "parametrized_macro.h"
#include "data_generator.h"
#include "../src/sort.hpp"

using std::vector;


void checkDataCorrectOrdered(vector<int> data){
    sort(data);

    for(int i = 1; i<data.size(); i++){
        CHECK(data[i-1] <= data[i]);
    }
}


TEST_CASE("Numbers are ordered correctly"){
    auto testdata = generateData();

    vector<int> data;

    DOCTEST_VALUE_PARAMETERIZED_DATA(data, testdata);

    checkDataCorrectOrdered(data);
}