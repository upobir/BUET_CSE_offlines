#include "SymbolInfo.h"

namespace SymbolTable{
    SymbolInfo::SymbolInfo(std::string name, std::string type){
        this->name = name;
        this->type = type;
        this->next = nullptr;
    }
    
    SymbolInfo::~SymbolInfo(){
    }

    std::string SymbolInfo::getName(){
        return this->name;
    }

    std::string SymbolInfo::getType(){
        return this->type;
    }

    SymbolInfo* SymbolInfo::getNext(){
        return this->next;
    }

    void SymbolInfo::insertNext(SymbolInfo *info){
        if(info != nullptr) {
            info->next = this->next;
        }
        this->next = info;
    }

    SymbolInfo* SymbolInfo::removeNext(){
        SymbolInfo* info = this->next;
        if(info != nullptr){
            this->next = info->next;
        }
        return info;
    }

    std::ostream& operator<<(std::ostream& os, SymbolInfo& info){
        return os<<"< "<<info.getName()<<" , "<<info.getType()<<" >";
    }

}