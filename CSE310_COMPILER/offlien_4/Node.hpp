#pragma once

#include <string>
#include <vector>

#include "enums.hpp"

struct Node{
    std::string text;
    TYPE type;
    int dimension = 0;
    std::vector<Node*> collection;
    std::string code;
    int offset = 0;
    std::string address;

    ~Node(){
        for(auto n : collection)
            delete n;
    }
};