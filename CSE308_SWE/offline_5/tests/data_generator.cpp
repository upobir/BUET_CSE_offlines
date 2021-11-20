#include "data_generator.h"
#include "randgen.h"

vector<pair<string, vector<int>>> getBlankList(){
    auto message = "blank list";
    return {
        {message, {}}
    };
}

vector<pair<string, vector<int>>> getOneElementList(){
    auto message = "single elements";
    return {
        {message, {10}}
    };
}

vector<pair<string, vector<int>>> getTwoElementList(){
    auto message = "two elements";
    return {
        {message, {10, 20}},
        {message, {20, 10}},
        {message, {10, 10}}
    };
}

vector<pair<string, vector<int>>> getRandomList(){
    vector<pair<string, vector<int>>> result;
    auto message = "random list";

    for(int i = 0; i<10; i++){
        auto size = RandGen::getNumber(5, 30);
        vector<int> data(size);

        for(int j = 0; j<size; j++)
            data[j] = RandGen::getNumber(-100, 100);

        result.emplace_back(message, data);
    }

    return result;
}

vector<pair<string, vector<int>>> getAscendingList(){
    auto message = "ascending list";

    vector<int> data(10);

    int current = -50;
    for(int i = 0; i<data.size(); i++){
        current += RandGen::getNumber(1, 10);
        data[i] = current;
    }

    return {
        {message, data}
    };
}

vector<pair<string, vector<int>>> getDescendingList(){
    auto message = "descending list";

    vector<int> data(10);

    int current = 50;
    for(int i = 0; i<data.size(); i++){
        current -= RandGen::getNumber(1, 10);
        data[i] = current;
    }

    return {
        {message, data}
    };
}

vector<pair<string, vector<int>>> getEqualElementList(){
    auto message = "equal elements";

    vector<int> data(10, 11);

    return {
        {message, data}
    };
}

vector<pair<string, vector<int>>> generateData() {
    RandGen::seed(100);

    vector<pair<string, vector<int>>> result;

    for(auto generator: {getBlankList, getOneElementList, getTwoElementList, getRandomList, getAscendingList, getDescendingList, getEqualElementList}){
        auto temp = generator();

        result.insert(result.end(), temp.begin(), temp.end());
    }

    return result;
}

