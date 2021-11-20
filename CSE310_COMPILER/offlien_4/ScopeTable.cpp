#include "ScopeTable.h"

namespace SymbolTable{
    ScopeTable::ScopeTable(int totalBuckets, ScopeTable* parentScope){ //, std::ostream& os): os(os){
        this->totalBuckets = totalBuckets;
        this->buckets = new SymbolInfo*[totalBuckets];
        for(int bucket = 0; bucket < totalBuckets; bucket++)
            buckets[bucket] = nullptr;

        this->parentScope = parentScope;
        this->childCount = 0;

        int currentId = 1;
        this->id = "";
        if(this->parentScope != nullptr){
            currentId = ++this->parentScope->childCount;
            this->id = this->parentScope->id + ".";
        }
        this->id += std::to_string(currentId);
    }

    ScopeTable::~ScopeTable(){
        for(int bucket = 0; bucket < totalBuckets; bucket++){
            SymbolInfo* current = buckets[bucket];
            while(current != nullptr){
                SymbolInfo* next = current->getNext();
                delete current;
                current = next;
            }
        }
        delete[] buckets;
    }
    
    int ScopeTable::hash(std::string& str){
        int hashResult = 0;
        for(char c : str){
            hashResult += (int)c;
        }
        hashResult %= totalBuckets;
        return hashResult;
    }
    
    int ScopeTable::getPrevious(int bucket, std::string& name, SymbolInfo*& info){
        info = buckets[bucket];

        int position = 0;

        if(info == nullptr || info->getName() == name){
            info = nullptr;
            return position;
        }

        position++;
        while(info->getNext() != nullptr){
            if(info->getNext()->getName() == name)
                break;
            info = info->getNext();
            position++;
        }

        return position;
    }

    bool ScopeTable::insert(std::string name, std::string type){
        int bucket = hash(name);
        
        SymbolInfo* previous;
        int position = getPrevious(bucket, name, previous);

        if(buckets[bucket] == nullptr || (previous != nullptr && previous->getNext() == nullptr)){
            SymbolInfo* info = new SymbolInfo(name, type);

            if(previous == nullptr){
                buckets[bucket] = info;
            }
            else{
                previous->insertNext(info);
            }

            //os<<"Inserted in ScopeTable # "<<id<<" at position "<<bucket<<", "<<position<<std::endl;

            return true;
        }
        else{

            SymbolInfo* info = (previous == nullptr? buckets[bucket] : previous->getNext());

            //os<<(*info) <<" already exists in current ScopeTable"<<std::endl;

            return false;
        }
    }

    SymbolInfo* ScopeTable::lookUp(std::string name){
        int bucket = hash(name);

        SymbolInfo* previous;
        int position = getPrevious(bucket, name, previous);

        if(buckets[bucket] == nullptr || (previous != nullptr && previous->getNext() == nullptr)){
            return nullptr;
        }
        else{
            SymbolInfo* info;
            if(previous == nullptr){
                info = buckets[bucket];
            }
            else{
                info = previous->getNext();
            }

            //os<<"Found in ScopeTable # "<<id<<" at position "<<bucket<<", "<<position<<std::endl;
            return info;
        }
    }

    bool ScopeTable::remove(std::string name){
        int bucket = hash(name);

        SymbolInfo* previous;
        int position = getPrevious(bucket, name, previous);

        if(buckets[bucket] == nullptr || (previous != nullptr && previous->getNext() == nullptr)){
            //os<<"Not found"<<std::endl;
            return false;
        }
        else{
            SymbolInfo* toBeDeleted;
            if(previous == nullptr){
                toBeDeleted = buckets[bucket];
                buckets[bucket] = toBeDeleted->getNext();
            }
            else{
                toBeDeleted = previous->removeNext();
            }

            delete toBeDeleted;

            //os<<"Deleted entry "<<bucket<<", "<<position<<" from current ScopeTable"<<std::endl;

            return true;
        }
    }

    void ScopeTable::print(std::ostream &os){
        os<<"ScopeTable # "<<id<<std::endl;

        for(int bucket = 0; bucket < totalBuckets; bucket++){
            SymbolInfo* current = buckets[bucket];
            if(current == nullptr)
                continue;

            os<<" "<<bucket<<" --> ";
            while(current != nullptr){
                os<<(*current)<<" ";
                current = current->getNext();
            }

            os<<std::endl;
        }
    }

    ScopeTable* ScopeTable::getParentScope(){
        return parentScope;
    }

    std::string ScopeTable::getId(){
        return id;
    }

}