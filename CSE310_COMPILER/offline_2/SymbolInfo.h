#pragma once
#include <string>
#include <sstream>

namespace SymbolTable
{
    class SymbolInfo
    {
    private:
        std::string name, type;
        SymbolInfo* next;

    public:
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
