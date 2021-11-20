#pragma once
#include "ScopeTable.h"
#include "SymbolInfo.h"
#include <string>
#include <sstream>


namespace SymbolTable{
    class SymbolTable
    {
    private:
        int totalBuckets;
        ScopeTable* currentScope;
        //std::ostream& os;
    public:
        SymbolTable(int totalBuckets);//, std::ostream& os);
        ~SymbolTable();

        void enterScope();
        void exitScope();
        bool insert(std::string name, std::string type);
        bool remove(std::string name);
        SymbolInfo* lookUp(std::string name);
        // void printCurrentScope();
        void printAllScopes(std::ostream &os);
        std::string getCurrentID();
    };    
}