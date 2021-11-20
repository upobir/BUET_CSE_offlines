#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "enums.hpp"

namespace SymbolTable
{
    class SymbolInfo
    {
    private:
        std::string name, type;
        SymbolInfo* next;

    public:

        KIND idType;
        TYPE dataType;
        bool defined;
        std::vector<TYPE> parameterList;
        int offset = 0;

        SymbolInfo(std::string name, std::string type);
        ~SymbolInfo();

        std::string getName();
        std::string getType();
        SymbolInfo* getNext();
        void insertNext(SymbolInfo *info);
        SymbolInfo* removeNext();

        friend std::ostream& operator<<(std::ostream& os, SymbolInfo& info);
    };
} 
