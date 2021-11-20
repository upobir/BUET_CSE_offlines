#include "SymbolTable.h"

namespace SymbolTable{
    SymbolTable::SymbolTable(int totalBuckets){ //, std::ostream& os) : os(os){
        this->totalBuckets = totalBuckets;
        this->currentScope = new ScopeTable(totalBuckets, nullptr);//, os);
    }
    
    SymbolTable::~SymbolTable(){
        while(currentScope != nullptr){
            ScopeTable* previousScope = currentScope->getParentScope();
            delete currentScope;
            currentScope = previousScope;
        }
    }

    void SymbolTable::enterScope(){
        ScopeTable* newScope = new ScopeTable(totalBuckets, currentScope);//, os);
        currentScope = newScope;
        // os<<"New ScopeTable with id "<<currentScope->getId()<<" created"<<std::endl;
    }

    void SymbolTable::exitScope(){
        ScopeTable* previousScope = currentScope->getParentScope();
        // os<<"ScopeTable with id "<<currentScope->getId()<<" removed"<<std::endl;
        delete currentScope;
        currentScope = previousScope;
    }

    bool SymbolTable::insert(std::string name, std::string type){
        bool success = currentScope->insert(name, type);
        return success;
    }

    bool SymbolTable::remove(std::string name){
        bool success = currentScope->remove(name);
        return success;
    }

    SymbolInfo* SymbolTable::lookUp(std::string name){
        ScopeTable* scope = currentScope;
        while(scope != nullptr){
            SymbolInfo* info = scope->lookUp(name);
            if(info != nullptr)
                return info;
            scope = scope->getParentScope();
        }

        // os<<"Not found"<<std::endl;
        return nullptr;
    }

    // void SymbolTable::printCurrentScope(){
    //     currentScope->print();
    // }

    void SymbolTable::printAllScopes(std::ostream &os){
        ScopeTable *scope = currentScope;
        while(scope != nullptr){
            scope->print(os);
            scope = scope->getParentScope();
            if(scope != nullptr)
                os<<std::endl;
        }
    }
    
    std::string SymbolTable::getCurrentID() {
        return currentScope->getId();
    }
}