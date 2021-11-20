%{
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#include "SymbolTable.h"
#include "ScopeTable.h"
#include "SymbolInfo.h"

void printError(std::string message);
#include "Node.hpp"
using namespace SymbolTable;

#define YYSTYPE Node*

#include "CodeGenerator.h"

extern int lineCount;
int errorCount;

int yyparse(void);
extern int yylex(void);

extern FILE *yyin;
std::ofstream codeFile;
std::ofstream errorFile;

// void yyerror(char *s);
void printRule(std::string variable, std::string rule);

void beginScope();
void endScope();
void declareFunction(Node* type, Node* name, Node* parameters, bool errorCheck = true);
void defineFunction(Node* type, Node* name, Node* parameters);
bool declareVariable(TYPE type, std::string name, int dimension = 0);
SymbolInfo* expectSymbol(std::string name, KIND kind);
TYPE typeFromString(std::string type);
TYPE cast(TYPE expected, TYPE type, std::string msg = "");
TYPE operandTypeCheck(TYPE type1, TYPE type2);
void exitScope();
void yyerror(char *s);

const int bucketCount = 30;
SymbolTable::SymbolTable *symbolTable;

CodeGenerator* generator;

%}

%token ID
%token INT FLOAT VOID FOR WHILE IF ELSE RETURN PRINTLN
%token COMMA SEMICOLON
%token LPAREN RPAREN
%token LCURL RCURL
%token LTHIRD RTHIRD
%token CONST_INT CONST_FLOAT
%token ASSIGNOP
%token LOGICOP
%token RELOP
%token ADDOP MULOP
%token NOT
%token INCOP DECOP

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%start Start

%destructor {delete $$;} ID CONST_INT CONST_FLOAT LOGICOP RELOP ADDOP MULOP INCOP DECOP Start Program Unit Func_declaration Func_definition Parameter_list Compound_statement Var_declaration Type_specifier Declaration_list Statements Statement Expression_statement Variable Expression Logic_expression Rel_expression Simple_expression Term Unary_expression Factor Argument_list Arguments

%%

Start:
      Program 
        {
            // printRule("start", "program");
            $$ = $1;

            generator->code = $$->code;
        }
    ;

Program:
      Program Unit
        {
            // printRule("program", "program unit");

            std::string text = $1->text + "\n" + $2->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};

            if($2->code.substr(0, 4) == "main")
                $$->code = $2->code + $1->code;
            else
                $$->code = $1->code + $2->code;

            delete $1;
            delete $2;
        }
    | Unit
        {
            // printRule("program", "unit");
            $$ = $1;
        }
    ;

Unit: 
      Var_declaration
        {
            // printRule("unit", "var_declaration");

            $$ = $1;
        }
    | Func_declaration
        {
            // printRule("unit", "func_declaration");

            $$ = $1;
            ;
        }
    | Func_definition
        {
            // printRule("unit", "func_definition");

            $$ = $1;
            ;
        }
    ;

Func_declaration:
      Type_specifier ID LPAREN Parameter_list RPAREN { declareFunction($1, $2, $4); } SEMICOLON
        {
            // printRule("func_declaration", "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");

            std::string text = $1->text + " " + $2->text + "(" + $4->text + ");";
            $$ = new Node{
                .text = text, .type = TYPE::VOID_T};
            ;

            symbolTable->exitScope();

            delete $1;
            delete $2;
            delete $4;
        }
    | Type_specifier ID LPAREN RPAREN { declareFunction($1, $2, nullptr); } SEMICOLON
        {
            // printRule("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
            std::string text = $1->text + " " + $2->text + "();";
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            symbolTable->exitScope();


            delete $1;
            delete $2;
            
        }
    ;

Func_definition:
      Type_specifier ID LPAREN Parameter_list RPAREN { defineFunction($1, $2, $4); } Compound_statement
        {
            // printRule("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");
            std::string text = $1->text + " " + $2->text + "(" + $4->text + ")" + $7->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $$->code = generator->func_defintion($2, $4, $7);

            delete $1;
            delete $2;
            delete $4;
            delete $7;
        }
    | Type_specifier ID LPAREN RPAREN { defineFunction($1, $2, nullptr); } Compound_statement
        {
            // printRule("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");
            std::string text = $1->text + " " + $2->text + "()" + $6->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $$->code = generator->func_defintion($2, nullptr, $6);

            delete $1;
            delete $2;
            delete $6;
        }
    ;

Parameter_list:
      Parameter_list COMMA Type_specifier ID
        {
            // printRule("parameter_list", "parameter_list COMMA type_specifier ID");
            $1->text = $1->text + "," +  $3->text + " " + $4->text;
            $$ = $1;

            for(auto n : $$->collection){
                if(n->text == $4->text){
                    printError("Multiple declaration of " + $4->text + " in parameter");
                    $4->text = "$";
                }
            }

            ;

            $4->type = typeFromString($3->text);
            $$->collection.push_back($4);


            delete $3;
        }
    | Parameter_list COMMA Type_specifier
        {
            // printRule("parameter_list", "parameter_list COMMA type_specifier");
            $1->text = $1->text + "," +  $3->text;
            $$ = $1;
            ;

            $3->type = typeFromString($3->text);
            $3->text = "";

            $1->collection.push_back($3);

        }
    | Type_specifier ID
        {
            // printRule("parameter_list", "type_specifier ID");
            std::string text = $1->text + " " + $2->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};

            $2->type = typeFromString($1->text);
            $$->collection.push_back($2);


            delete $1;
        }
    | Type_specifier
        {
            // printRule("parameter_list", "type_specifier");
            std::string text = $1->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $1->type = typeFromString($1->text);
            $1->text = "";
            $$->collection.push_back($1);

        }
    ;

Compound_statement:
      LCURL Statements RCURL
        {
            // printRule("compound_statement", "LCURL statements RCURL");
            std::string text = "{\n" + $2->text + "\n}";
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $$->code = generator->compound_statement($2);
            exitScope();

            delete $2;
        }
    | LCURL Opt_error RCURL
        {
            // printRule("compound_statement", "LCURL RCURL");
            $$ = new Node{.text = "{}", .type = TYPE::VOID_T};
            ;
            exitScope();
        }
    ;

Var_declaration:
      Type_specifier Declaration_list SEMICOLON
        {
            // printRule("var_declaration","type_specifier declaration_list SEMICOLON");
            std::string text = $1->text + " " + $2->text + ";";
            $$ = new Node{.text = text, .type = TYPE::VOID_T};

            for(auto node : $2->collection){
                auto type = typeFromString($1->text);
                if(declareVariable(type, node->text, node->dimension))
                    $$->code += generator->declareVariable(type, node->text, node->dimension);
            }

            delete $1;
            delete $2;
            ;
        }
    ;

Type_specifier:
      INT
        {
            // printRule("type_specifier", "INT");
            $$ = new Node{.text = "int", .type = TYPE::VOID_T};
            ;
        }
    | FLOAT
        {
            // printRule("type_specifier", "FLOAT");
            $$ = new Node{.text = "float", .type = TYPE::VOID_T};
            ;
        }
    | VOID
        {
            // printRule("type_specifier", "VOID");
            $$ = new Node{.text = "void", .type = TYPE::VOID_T};
            ;
        }
    ;

Declaration_list:
      Declaration_list COMMA ID
        {
            // printRule("declaration_list", "declaration_list COMMA ID");
            $1->text = $1->text + "," + $3->text;
            $$ = $1;
            ;

            $3->type = TYPE::VOID_T;
            $$->collection.push_back($3);
        }
    | Declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
        {
            // printRule("declaration_list", "declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
            // $1->text = $1->text + "," + $3->text + "[" + $5->text +"]";
            $$ = $1;
            ;

            $3->type = TYPE::SPECIAL;
            $3->dimension = std::stoi($5->text);
            $1->collection.push_back($3);

            delete $5;
        }
    | ID
        {
            // printRule("declaration_list", "ID");
            std::string text = $1->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $1->type = TYPE::VOID_T;
            $$->collection.push_back($1);
        }
    | ID LTHIRD CONST_INT RTHIRD
        {
            // printRule("declaration_list", "ID LTHIRD CONST_INT RTHIRD");
            std::string text = $1->text + "[" + $3->text + "]";
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $1->type = TYPE::SPECIAL;
            $1->dimension = std::stoi($3->text);
            $$->collection.push_back($1);

            delete $3;
        }
    ;

Statements:
      Statements Statement
        {
            // printRule("statements", "statements statement");
            std::string text = $1->text + "\n" + $2->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $$->code = $1->code + $2->code;

            delete $1;
            delete $2;
        }
    | Statement
        {
            // printRule("statements", "statement");
            $$ = $1;
            ;
        }
    ;

Statement:
      Var_declaration
        {
            // printRule("statement", "var_declaration");
            $$ = $1;
            ;
        }
    | Expression_statement
        {
            // printRule("statement", "expression_statement");
            $$ = $1;
            generator->release($1);
        }
    | { beginScope(); } Compound_statement
        {
            // printRule("statement", "compound_statement");
            $$ = $2;
            ;
        }
    | FOR LPAREN Expression_statement Expression_statement Expression RPAREN Statement
        {
            // printRule("statement", "FOR LPAREN expression_statement expression_statement expression RPAREN statement");
            std::string text = "for(" + $3->text + $4->text + $5->text + ")" + $7->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            
            $$->code = generator->for_loop($3, $4, $5, $7);

            delete $3;
            delete $4;
            delete $5;
            delete $7;
        }
    | IF LPAREN Expression RPAREN Statement %prec LOWER_THAN_ELSE
        {
            // printRule("statement", "IF LPAREN expression RPAREN statement");
            std::string text = "if (" + $3->text + ")" + $5->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            
            $$->code = generator->if_stmt($3, $5);

            delete $3;
            delete $5;
        }
    | IF LPAREN Expression RPAREN Statement ELSE Statement
        {
            // printRule("statement", "IF LPAREN expression RPAREN statement ELSE statement");
            std::string text = "if (" + $3->text + ")" + $5->text + "\nelse " + $7->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            
            $$->code = generator->if_else_stmt($3, $5, $7);

            delete $3;
            delete $5;
            delete $7;
        }
    | WHILE LPAREN Expression RPAREN Statement
        {
            // printRule("statement", "WHILE LPAREN expression RPAREN statement");
            std::string text = "while ("+$3->text+")"+$5->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            
            $$->code = generator->while_stmt($3, $5);

            delete $3;
            delete $5;
        }
    | PRINTLN LPAREN ID RPAREN SEMICOLON
        {
            // printRule("statement", "PRINTLN LPAREN ID RPAREN SEMICOLON");
            std::string text = "printf("+$3->text+");";
            $$ = new Node{.text = text, .type = TYPE::VOID_T};

            expectSymbol($3->text, KIND::VARIABLE);

            $$->code = generator->println($3);

            delete $3;
        }
    | RETURN Expression SEMICOLON
        {
            // printRule("statement", "RETURN expression SEMICOLON");
            std::string text = "return " + $2->text +";";
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $$->code = generator->return_expression($2);

            delete $2;
        }
    ;

Expression_statement:
      Opt_error SEMICOLON
        {
            // printRule("expression_statement", "SEMICOLON");
            $$ = new Node{.text = ";", .type = TYPE::VOID_T};
            ;
        }
    | Opt_error Expression SEMICOLON
        {
            // printRule("expression_statement", "expression SEMICOLON");
            std::string text = $2->text + ";";
            $$ = new Node{.text = text, .type = TYPE::VOID_T};
            ;

            $$->code = $2->code;
            $$->offset = $2->offset;
            //std::cerr<<$$->code<<std::endl;

            delete $2;
        }
    ;


Variable:
      ID
        {
            // printRule("variable", "ID");

            std::string text = $1->text;
            $$ = new Node{.text = text};

            SymbolInfo *info = expectSymbol($$->text, KIND::VARIABLE);

            if(info == nullptr){
                $$->type = TYPE::NONE;
            }
            else{
                $$->type = info->dataType;
                generator->var_id($$);
            }

            delete $1;
        }
    | ID LTHIRD Expression RTHIRD
        {
            // printRule("variable", "ID LTHIRD expression RTHIRD");

            std::string text = $1->text + "[" + $3->text + "]";
            $$ = new Node{.text = text};

            SymbolInfo *info = expectSymbol($1->text, KIND::ARRAY);

            if(info == nullptr){
                $$->type = TYPE::NONE;
            }
            else{
                if($3->type != TYPE::VOID_T && $3->type != TYPE::INT_T){
                    printError("Expression inside third brackets not an integer");
                }
                $$->type = info->dataType;
                generator->access($$, $1, $3);
            }
            
            ;

            delete $1;
            delete $3;
        }
    ;

Expression:
      Logic_expression
        {
            // printRule("expression", "logic_expression");
            $$ = $1;
            ;
        }
    | Variable ASSIGNOP Logic_expression
        {
            // printRule("expression", "variable ASSIGNOP logic_expression");
            std::string text = $1->text + "=" + $3->text;
            $$ = new Node{.text = text};

            $$->type = cast($1->type, $3->type);

            ;

            generator->assignment($$, $1, $3);

            delete $1;
            delete $3;
        }
    ;

Logic_expression:
      Rel_expression
        {
            // printRule("logic_expression", "rel_expression");
            $$ = $1;
            ;
        }
    | Rel_expression LOGICOP Rel_expression
        {
            // printRule("logic_expression", "rel_expression LOGICOP rel_expression");
            std::string text = $1->text + $2->text + $3->text;
            $$ = new Node{.text = text};

            if($1->type == TYPE::NONE || $2->type == TYPE::NONE){
                $$->type = TYPE::NONE;
            }
            else{
                $$->type = TYPE::INT_T;
            }

            generator->logic_expression($$, $1, $2, $3);

            delete $1;
            delete $2;
            delete $3;
        }
    ;

Rel_expression:
      Simple_expression
        {
            // printRule("rel_expression", "simple_expression");
            $$ = $1;
            ;
        }
    | Simple_expression RELOP Simple_expression
        {
            // printRule("rel_expression", "simple_expression RELOP simple_expression");
            std::string text = $1->text + $2->text + $3->text;
            $$ = new Node{.text = text};

            if($1->type == TYPE::NONE || $2->type == TYPE::NONE){
                $$->type = TYPE::NONE;
            }
            else{
                $$->type = TYPE::INT_T;
            }

            generator->rel_expression($$, $1, $2, $3);

            delete $1;
            delete $2;
            delete $3;
        }
    ;

Simple_expression:
      Term
        {
            // printRule("simple_expression", "term");
            $$ = $1;
            ;
        }
    | Simple_expression ADDOP Term
        {
            // printRule("simple_expression", "simple_expression ADDOP term");
            std::string text = $1->text + $2->text + $3->text;
            $$ = new Node{.text = text};

            $$->type = operandTypeCheck($1->type, $3->type);

            generator->addop($$, $1, $2, $3);

            delete $1;
            delete $2;
            delete $3;
        }
    ;

Term:
      Unary_expression
        {
            // printRule("term", "unary_expression");
            $$ = $1;
            ;
        }
    | Term MULOP Unary_expression
        {
            // printRule("term", "term MULOP unary_expression");
            std::string text = $1->text + $2->text + $3->text;
            $$ = new Node{.text = text};

            if($2->text == "%"){
                if($1->type == TYPE::NONE || $3->type == TYPE::NONE){
                    $$->type = TYPE::NONE;
                }
                else if($1->type != TYPE::INT_T || $3->type != TYPE::INT_T){
                    printError("Non-Integer operand on modulus operator");
                    $$->type = TYPE::NONE;
                }
                else if($3->text == "0"){
                    printError("Modulus by Zero");
                    $$->type = TYPE::NONE;
                }
                else{
                    $$->type = TYPE::INT_T;
                }
            }
            else{
                $$->type = operandTypeCheck($1->type, $3->type);
            }

            generator->mulop($$, $1, $2, $3);

            delete $1;
            delete $2;
            delete $3;
        }
    ;

Unary_expression:
      ADDOP Unary_expression
        {
            // printRule("unary_expression","ADDOP unary_expression");
            std::string text = $1->text + $2->text;
            $$ = new Node{.text = text};

            if($1->type == TYPE::VOID_T){
                printError("Void Operand");
                $$->type = TYPE::NONE;
            }
            else{
                $$->type = $1->type;
            }

            ;

            generator->addop($$, $1, $2);

            delete $1;
            delete $2;
        }
    | NOT Unary_expression
        {
            // printRule("unary_expression","NOT unary_expression");
            std::string text = "!" + $2->text;
            $$ = new Node{.text = text};

            if($1->type == TYPE::VOID_T){
                printError("Void Operand");
                $$->type = TYPE::NONE;
            }
            else{
                $$->type = TYPE::INT_T;
            }

            ;

            generator->not_op($$, $2);

            delete $2;
        }
    | Factor
        {
            // printRule("unary_expression","factor");
            $$ = $1;
            ;
            // std::cerr<<$$->code<<std::endl;
            // std::cerr<<$$->text<<std::endl;
        }
    ;

Factor:
      Variable
        {
            // printRule("factor", "variable");
            $$ = $1;
            generator->factor_var($$);
        }
    | ID LPAREN Argument_list RPAREN
        {
            // printRule("factor", "ID LPAREN argument_list RPAREN");
            std::string text = $1->text + "(" + $3->text + ")";
            $$ = new Node{.text = text};

            SymbolInfo* info = expectSymbol($1->text, KIND::FUNCTION);

            if(info == nullptr){
                $$->type = TYPE::NONE;
            }
            else if($3->collection.size() != info->parameterList.size()){
                printError("Total number of arguments mismatch in function " + $1->text);
                $$->type = TYPE::NONE;
            }
            else{
                for(int i = 0; i<info->parameterList.size(); i++){
                    if($3->collection[i]->type == TYPE::NONE)
                        continue;
                    std::string msg = std::to_string(i+1) + "th argument mismatch in function " + $1->text;
                    if(cast(info->parameterList[i], $3->collection[i]->type, msg) == TYPE::NONE){
                        break;
                    }
                }
                $$->type = info->dataType;
                generator->function_call($$, $1, $3);
            }

            ;

            delete $1;
            delete $3;
        }
    | LPAREN Expression RPAREN
        {
            // printRule("factor", "LPAREN expression RPAREN");
            std::string text = "(" + $2->text + ")";
            $$ = new Node{.text = text};

            $$->type = $2->type;

            ;

            $$->code = $2->code;
            $$->offset = $2->offset;

            delete $2;
        }
    | CONST_INT
        {
            // printRule("factor", "CONST_INT");
            std::string text = $1->text;
            $$ = new Node{.text = text};

            $$->type = TYPE::INT_T;

            ;

            $$->offset = 1;

            delete $1;
        }
    | CONST_FLOAT
        {
            // printRule("factor", "CONST_FLOAT");
            std::string text = $1->text;
            $$ = new Node{.text = text};

            $$->type = TYPE::FLOAT_T;

            ;
            $$->offset = 1;

            delete $1;
        }
    | Variable INCOP
        {
            // printRule("factor", "variable INCOP");
            std::string text = $1->text + $2->text;
            $$ = new Node{.text = text};

            $$->type = $1->type;

            generator->incop($$, $1);

            delete $1;
            delete $2;
        }
    | Variable DECOP
        {
            // printRule("factor", "variable DECOP");
            std::string text = $1->text + $2->text;
            $$ = new Node{.text = text};

            $$->type = $1->type;

            generator->decop($$, $1);

            delete $1;
            delete $2;
        }
    ;

Argument_list:
      Arguments
        {
            // printRule("argument_list", "arguments");
            $$ = $1;
            ;
        }
    |
        {
            // printRule("argument_list", "");
            $$ = new Node{.text = "", .type = TYPE::VOID_T};
            ;
        }
    ;

Arguments:
      Arguments COMMA Logic_expression
        {
            // printRule("arguments", "arguments COMMA logic_expression");
            $1->text = $1->text + "," + $3->text;
            $$ = $1;

            $$->collection.push_back($3);

            ;
        }
    | Logic_expression
        {
            // printRule("arguments", "logic_expression");
            std::string text = $1->text;
            $$ = new Node{.text = text, .type = TYPE::VOID_T};

            $$->collection.push_back($1);

            ;
        }
    ;

Opt_error :
      error 
    |
    ;

%%

int main(int argc,char **argv){
    if(argc < 2){
        printf("please enter the input file name\n");
        return 0;
    }

    FILE *fileIn = fopen(argv[1],"r");

    //std::string logFileName(argc >= 3? argv[2] : "log.txt");
    //logFile.open(logFileName, std::ios::out);

    errorFile.open("error.txt", std::ios::out);

    symbolTable = new SymbolTable::SymbolTable(bucketCount);
    generator = new CodeGenerator(symbolTable);

    if(fileIn==NULL){
        std::cout<<"Cannot open specified file"<<std::endl;
        return 0;
    }
    else{
        std::cout<<"Lexing and Parsing "<<argv[1]<<std::endl;
    }

    yyin = fileIn;

    yyparse();

    //symbolTable->printAllScopes(logFile);
    //logFile<<std::endl;

    // logFile<<"Total lines: "<<lineCount<<std::endl;
    // logFile<<"Total errors: "<<errorCount<<std::endl;

    fclose(fileIn);

    errorFile.close();

    codeFile.open("code.asm", std::ios::out);
    if(errorCount == 0){
        generator->printCode(codeFile);
    }
    else{
        codeFile<<"";
    }
    codeFile.close();
    
    
    codeFile.open("optimized_code.asm", std::ios::out);
    if(errorCount == 0){
        generator->optimize();
        generator->printCode(codeFile);
    }
    else{
        codeFile<<"";
    }
    codeFile.close();

    delete symbolTable;
    delete generator;

    return 0;
}

void beginScope() {
    symbolTable->enterScope();
    generator->stackSizes.push_back(generator->stackSizes.back());
    generator->freeStacks.emplace_back();
    // std::cerr<<">"<<generator->stackSizes.size()<<std::endl;
}

void printRule(std::string variable, std::string rule){
    // logFile<<"Line "<<lineCount<<": "<<variable<<" : "<<rule<<std::endl;
    // logFile<<std::endl;
}

void printError(std::string message){

    yyerror((char *)message.c_str());
}

void declareFunction(Node* type, Node* name, Node* parameters, bool errorCheck){

    bool valid = symbolTable->insert(name->text, "ID");
    SymbolInfo* info = symbolTable->lookUp(name->text);

    if(valid){
        info->idType = KIND::FUNCTION;
        info->dataType = typeFromString(type->text);
        info->defined = false;
    }

    beginScope();

    if(parameters){
        for(auto node : parameters->collection){
            if(!node->text.empty() && node->text != "$"){
                declareVariable(node->type, node->text);
            }
            if(valid)
                info->parameterList.push_back(node->type);
        }
        generator->parameters(parameters);
    }
    
    if(!valid && errorCheck){
        printError("Multiple declaration of " + name->text);
    }
}

void defineFunction(Node* type, Node* name, Node* parameters){

    declareFunction(type, name, parameters, false);
    SymbolInfo* info = symbolTable->lookUp(name->text);

    if(info->idType != KIND::FUNCTION){
        printError("Multiple declaration of " + name->text);
    }
    else{
        if(info->defined){
            printError("Multiple defintion of " + name->text);
        }
        else{
            
            int paramCount = (parameters? parameters->collection.size() : 0);
            if(info->dataType != typeFromString(type->text)){
                printError("Return type mismatch with function declaration in function " + name->text);
            }
            else if(paramCount != info->parameterList.size()){
                printError("Total number of arguments mismatch with declaration in function " + name->text);
            }
            else{
                bool matched = true;
                for(int i = 0; i<paramCount; i++){
                    if(info->parameterList[i] != parameters->collection[i]->type){
                        printError(std::to_string(i+1) + "th paramter type does not match");
                        break;
                    }
                    else if(parameters->collection[i]->text.empty()){
                        printError(std::to_string(i+1) + "th parameter's name not given in function definition of " + name->text);
                        break;
                    }
                }
            }

            info->defined = true;
        }
    }
}

bool declareVariable(TYPE type, std::string name, int diemnsion){
    if(type == TYPE::VOID_T){
        printError("Variable type cannot be void");
        return false;
    }
    else if(symbolTable->insert(name, "ID")){
        SymbolInfo* info = symbolTable->lookUp(name);
        info->idType = (diemnsion ? KIND::ARRAY : KIND::VARIABLE);
        info->dataType = type;
        return true;
    }
    else{
        printError("Multiple declaration of " + name);
        return false;
    }
}

TYPE operandTypeCheck(TYPE type1, TYPE type2){
    if(type1 == TYPE::VOID_T || type2 == TYPE::VOID_T){
        printError("Void function used in expression");
        return TYPE::NONE;
    }
    else if(type1 == TYPE::NONE || type2 == TYPE::NONE){
        return TYPE::NONE;
    }
    else if(type1 == TYPE::FLOAT_T || type2 == TYPE::FLOAT_T){
        return TYPE::FLOAT_T;
    }
    else{
        return TYPE::INT_T;
    }
}

SymbolInfo* expectSymbol(std::string name, KIND kind){
    SymbolInfo* info = symbolTable->lookUp(name);
    if(info == nullptr){
        if(kind == KIND::FUNCTION){
            printError("Undeclared function " + name);
        }
        else{
            printError("Undeclared variable " + name);
        }
        
        return nullptr;
    }
    else if(info->idType != kind){
        std::string kindtext = "";
        switch(info->idType){
            case KIND::VARIABLE:
                kindtext = "a variable";
            break;
            case KIND::FUNCTION:
                kindtext = "a function";
            break;
            case KIND::ARRAY:
                kindtext = "an array";
            break;
        }

        printError("Type mismatch, " + name + " is " + kindtext);
        return nullptr;
    }
    else{
        return info;
    }
}

TYPE cast(TYPE expected, TYPE type, std::string msg){
    if(type == TYPE::NONE){
        return TYPE::NONE;
    }
    if(type == TYPE::VOID_T){
        if(msg.empty())
            msg = "Void function used in expression";
        printError(msg);
        return TYPE::NONE;
    }
    else if(expected == TYPE::INT_T && type == TYPE::FLOAT_T){
        if(msg.empty())
            msg = "Type Mismatch";
        printError(msg);
        return TYPE::NONE;
    }
    else{
        return expected;
    }
}

TYPE typeFromString(std::string type){
    if(type == "void")
        return TYPE::VOID_T;
    else if(type == "int")
        return TYPE::INT_T;
    else if(type == "float")
        return TYPE::FLOAT_T;
    else{
        1/0;
    }
}

void exitScope(){
    //symbolTable->printAllScopes(logFile);
    symbolTable->exitScope();
    //logFile<<std::endl;
}

void yyerror(char * msg){
    errorCount++;
    errorFile<<"Error at line " << lineCount << ": " << msg<<std::endl;
    errorFile<<std::endl;
}