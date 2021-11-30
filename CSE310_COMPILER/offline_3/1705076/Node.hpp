#pragma once

#include <string>
#include <vector>

#include "enums.hpp"

struct Node{
    std::string text;
    TYPE type;
    std::vector<Node*> collection;

    ~Node(){
        for(auto n : collection)
            delete n;
    }
};