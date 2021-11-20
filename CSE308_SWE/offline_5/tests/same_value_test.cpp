#include "doctest.h"
#include "parametrized_macro.h"
#include "data_generator.h"
#include "../src/sort.hpp"

using std::vector;


void checkDataSame(vector<int> data){
    auto datacopy = data;

    sort(data);

    CHECK(data.size() == datacopy.size());

    for(auto x : datacopy){
        int expectedcount = 0;
        int actualcount = 0;

        for(int y : data)
            actualcount += (x == y);

        for(int y : datacopy)
            expectedcount += (x == y);

        CHECK(expectedcount == actualcount);
    }
}


TEST_CASE("Numbers not altered by function"){
    auto testdata = generateData();

    vector<int> data;

    DOCTEST_VALUE_PARAMETERIZED_DATA(data, testdata);

    checkDataSame(data);
}