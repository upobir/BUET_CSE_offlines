#pragma once
#include "SymbolInfo.h"
#include <string>
#include <sstream>


namespace SymbolTable{
    class ScopeTable
    {
    private:
        int totalBuckets;
        SymbolInfo** buckets;
        ScopeTable* parentScope;
        int childCount;
        std::string id;
        //std::ostream& os;

        int hash(std::string& str);
        int getPrevious(int bucket, std::string& name, SymbolInfo*& info);
    public:
        ScopeTable(int totalBuckets, ScopeTable* parentScope);//, std::ostream& os);
        ~ScopeTable();

        bool insert(std::string name, std::string type);
        SymbolInfo* lookUp(std::string name);
        bool remove(std::string name);
        void print(std::ostream &os);
        ScopeTable* getParentScope();
        std::string getId();
    };   
}