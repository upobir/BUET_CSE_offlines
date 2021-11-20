#pragma once

#include <iostream>
#include <vector>

#include "Node.hpp"

#ifndef YYSTYPE
#define YYSTYPE Node*
#endif

#include "parser.hpp"

#include "SymbolTable.h"

class CodeGenerator{
private:
public:
    int labelCount = 0;
    std::string data;
    std::string code;
    SymbolTable::SymbolTable* table;
    std::vector<int> stackSizes;
    std::vector<std::vector<int>> freeStacks;
    std::string returnLabel;

    CodeGenerator(SymbolTable::SymbolTable* table);

    std::string newLabel();
    int newTemp();
    void release(Node* node);
    std::string getAssembly(Node* expression);
    std::string declareVariable(TYPE type, std::string name, int dimension);
    std::string return_expression(Node* expression);
    void parameters(Node* parameters);
    void optimize();
    
    std::string func_defintion(Node* id, Node* parameter_list, Node* body);
    std::string compound_statement(Node* statements);
    void access(Node* self, Node* id, Node* expression);
    void var_id(Node* id);
    void factor_var(Node* variable);
    void assignment(Node* self, Node* variable, Node* expression);
    void addop(Node* self, Node* addop, Node* expression);
    void not_op(Node* self, Node* expression);
    std::string println(Node* id);
    void decop(Node* self, Node* variable);
    void incop(Node* self, Node* variable);
    void mulop(Node* self, Node* term, Node* op, Node* expression);
    void addop(Node* self, Node* term, Node* op, Node* expression);
    void rel_expression(Node* self, Node* expr1, Node* op, Node* expr2);
    void logic_expression(Node* self, Node* expr1, Node* op, Node* expr2);
    void function_call(Node* self, Node* id, Node* args);
    std::string if_stmt(Node* expression, Node* statement);
    std::string if_else_stmt(Node* expression, Node* stmt1, Node* stmt2);
    std::string while_stmt(Node* expression, Node* statement);
    std::string for_loop(Node* expr1, Node* expr2, Node* expr3, Node* stmt);

    void printCode(std::ostream& os);
};