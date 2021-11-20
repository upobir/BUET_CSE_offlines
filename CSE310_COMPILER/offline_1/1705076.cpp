#include <iostream>
#include <fstream>
#include <string>
#include "SymbolTable.h"

int main(){

    std::ifstream fileIn("input.txt", std::ios::in);
    std::ofstream out("output.txt", std::ios::out);
    // std::istream& fileIn = std::cin;
    // std::ostream& out = std::cout;

    int totalBuckets;
    fileIn>>totalBuckets;

    SymbolTable::SymbolTable* table = new SymbolTable::SymbolTable(totalBuckets, out);

    std::string cmd;
    while(fileIn>>cmd){
        switch (cmd[0]){
        case 'I':{
                std::string name, type;
                fileIn>>name>>type;

                cmd += " " + name + " " + type;
                out<<cmd<<std::endl;
                out<<std::endl;

                table->insert(name, type);
                break;
            }

        case 'L':{
                std::string name;
                fileIn>>name;

                cmd += " " + name;
                out<<cmd<<std::endl;
                out<<std::endl;

                table->lookUp(name);
                break;
            }

        case 'D':{
                std::string name;
                fileIn>>name;

                cmd += " " + name;
                out<<cmd<<std::endl;
                out<<std::endl;

                table->remove(name);
                break;
            }
            
        case 'P':{
                std::string option;
                fileIn>>option;

                cmd += " " + option;
                out<<cmd<<std::endl;
                out<<std::endl;

                if(option == "C"){
                    table->printCurrentScope();
                }
                else if(option == "A"){
                    table->printAllScopes();
                }
                else{
                    out<<"Invalid command"<<std::endl;
                }
                break;
            }

        case 'S':{
                out<<cmd<<std::endl;
                out<<std::endl;

                table->enterScope();
                break;
            }

        case 'E':{
                out<<cmd<<std::endl;
                out<<std::endl;
                
                table->exitScope();
                break;
            }
        
        default:{
                out<<cmd<<std::endl;
                out<<std::endl;

                out<<"Invalid Command"<<std::endl;
                break;
            }
        }
        out<<std::endl;
    }

    delete table;

    fileIn.close();
    out.close();

    return 0;
}