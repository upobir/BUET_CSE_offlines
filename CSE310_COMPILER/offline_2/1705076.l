%option noyywrap

%x CHARACTER
%x STRING
%x SINGLE_COMMENT
%x MULTI_COMMENT

%{
// libraries
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<string>

// symbol table
#include "SymbolTable.h"

// symbol table variable declare
const int bucketCount = 7;
SymbolTable::SymbolTable *symbolTable;

std::ofstream tokenFile;
std::ofstream logFile;
int lineCount = 1;
int tempNewLines = 0;
int errorCount = 0;

std::string currentToken, currentLexeme;

char convertEscapedChar(char c);
void logToken(std::string type, std::string lexeme, std::string symbol = "");
void printToken(std::string type, std::string symbol = "");
void insertSymbol(std::string type, std::string symbol);
void logError(std::string msg);

%}

WHITESPACE [ \t\f\r\v]+ 
LETTER [a-zA-Z]
DIGIT [0-9]
EXPONENT ([eE][\+\-]?)
FRACTION (({DIGIT}+(\.{DIGIT}*)?)|(\.{DIGIT}+))
ESCAPE (\\[\\ntafrbv0\'\"])
NEWLINE (\n|(\r\n))

%%

"if"        {
                logToken("IF", yytext);
                printToken("IF");
            }

"else"      {
                logToken("WHILE", yytext);
                printToken("WHILE");
            }

"for"      {
                logToken("FOR", yytext);
                printToken("FOR");
            }

"while"     {
                logToken("WHILE", yytext);
                printToken("WHILE");
            }

"do"        {
                logToken("DO", yytext);
                printToken("DO");
            }

"break"     {
                logToken("BREAK", yytext);
                printToken("BREAK");
            }

"int"       {
                logToken("INT", yytext);
                printToken("INT");
            }

"char"      {
                logToken("CHAR", yytext);
                printToken("CHAR");
            }

"float"     {
                logToken("FLOAT", yytext);
                printToken("FLOAT");
            }

"double"    {
                logToken("DOUBLE", yytext);
                printToken("DOUBLE");
            }

"void"      {
                logToken("VOID", yytext);
                printToken("VOID");
            }

"return"    {
                logToken("RETURN", yytext);
                printToken("RETURN");
            }

"switch"    {
                logToken("SWITCH", yytext);
                printToken("SWITCH");
            }

"case"      {
                logToken("CASE", yytext);
                printToken("CASE");
            }

"default"   {
                logToken("DEFAULT", yytext);
                printToken("DEFAULT");
            }

"continue"  {
                logToken("CONTINUE", yytext);
                printToken("CONTINUE");
            }

    /* ^ Keywords */

{DIGIT}+    {
                logToken("CONST_INT", yytext);
                printToken("CONST_INT", yytext);
                insertSymbol("CONST_INT", yytext);
            }

    /* ^ Integer */

{FRACTION}({EXPONENT}{DIGIT}+)? {
                                    logToken("CONST_FLOAT", yytext);
                                    printToken("CONST_FLOAT", yytext);
                                    insertSymbol("CONST_FLOAT", yytext);
                                }

    /* ^ Float */

\'  {
        BEGIN CHARACTER;

        currentLexeme.clear();
        currentLexeme.push_back('\'');

        currentToken.clear();
    }

<CHARACTER>{ESCAPE} {
                        currentLexeme.insert(currentLexeme.size(), yytext);
                        currentToken.push_back(convertEscapedChar(yytext[1]));
                    }

<CHARACTER>[^\n\']  {
                        currentLexeme.push_back(yytext[0]);
                        currentToken.push_back(yytext[0]);
                    }


<CHARACTER>\'   {
                    currentLexeme.push_back('\'');

                    if(currentToken.empty()){
                        logError("Empty character constant error \'\'");
                    }
                    else if(currentToken.size() > 1){
                        logError(std::string("Multi character constant error ") + currentLexeme);
                    }
                    else{
                        logToken("CONST_CHAR", currentLexeme, currentToken);
                        printToken("CONST_CHAR", currentToken);
                        insertSymbol("CONST_CHAR", currentLexeme);
                    }

                    BEGIN INITIAL;
                }

<CHARACTER>{NEWLINE}    {
                            logError(std::string("Unterminated character ") + currentLexeme);
                            lineCount++;

                            BEGIN INITIAL;
                        }

<CHARACTER><<EOF>>      {
                            logError(std::string("Unterminated character ") + currentLexeme);

                            yyterminate();
                        }

    /* ^ Char */

\"  {
        BEGIN STRING;

        currentLexeme.clear();
        currentLexeme.push_back('\"');

        currentToken.clear();
        currentToken.push_back('\"');

        tempNewLines = 0;
    }

<STRING>{ESCAPE}    {
                        currentLexeme.insert(currentLexeme.size(), yytext);
                        currentToken.push_back(convertEscapedChar(yytext[1]));
                    }

<STRING>[^\n\"]     {
                        currentLexeme.push_back(yytext[0]);
                        currentToken.push_back(yytext[0]);
                    }

<STRING>\\{NEWLINE} {
                        currentLexeme.insert(currentLexeme.size(), yytext);
                        tempNewLines++;
                    }

<STRING>\"  {
                currentLexeme.push_back('\"');
                currentToken.push_back('\"');

                logToken("STRING", currentLexeme, currentToken);
                printToken("STRING", currentToken);

                lineCount += tempNewLines;

                BEGIN INITIAL;
            }

<STRING>{NEWLINE}   {
                        logError(std::string("Unterminated String ") + currentLexeme);

                        lineCount += tempNewLines;
                        lineCount++;

                        BEGIN INITIAL;
                    }

<STRING><<EOF>>     {
                        logError(std::string("Unterminated String ") + currentLexeme);

                        lineCount += tempNewLines;

                        yyterminate();
                    }

    /* ^ String */

\/\/    {
            BEGIN SINGLE_COMMENT;

            currentLexeme.clear();
            currentLexeme.insert(currentLexeme.size(), yytext);

            tempNewLines = 0;
        }

<SINGLE_COMMENT>.   {
                        currentLexeme.push_back(yytext[0]);
                    }

<SINGLE_COMMENT>\\{NEWLINE} {
                                currentLexeme.insert(currentLexeme.size(), yytext);
                                tempNewLines++;
                            }

<SINGLE_COMMENT>{NEWLINE}   {
                                logToken("COMMENT", currentLexeme);

                                lineCount += tempNewLines;
                                lineCount++;

                                BEGIN INITIAL;
                            }

<SINGLE_COMMENT><<EOF>>     {
                                logToken("COMMENT", currentLexeme);

                                lineCount += tempNewLines;

                                yyterminate();
                            }

    /* ^ Single Line Comment */

\/\*    {
            BEGIN MULTI_COMMENT;

            currentLexeme.clear();
            currentLexeme.insert(currentLexeme.size(), yytext);

            tempNewLines = 0;
        }

<MULTI_COMMENT>{NEWLINE}    {
                                currentLexeme.push_back(yytext[0]);
                                tempNewLines++;
                            }

<MULTI_COMMENT>.    {
                        currentLexeme.push_back(yytext[0]);
                    }

<MULTI_COMMENT>\*\/ {
                        currentLexeme.insert(currentLexeme.size(), yytext);

                        logToken("COMMENT", currentLexeme);
                        lineCount += tempNewLines;

                        BEGIN INITIAL;
                    }

<MULTI_COMMENT><<EOF>>  {
                            std::string msg("Unterminated Comment ");
                            msg += currentLexeme;

                            logError(msg);

                            lineCount += tempNewLines;

                            yyterminate();
                        }

    /* ^ Multi Line Comment */

[\+\-]  {
            logToken("ADDOP", yytext);
            printToken("ADDOP", yytext);
        }

[\*/%]  {
            logToken("MULOP", yytext);
            printToken("MULOP", yytext);
        }

(\+\+)|(\-\-)   {
                    logToken("INCOP", yytext);
                    printToken("INCOP", yytext);
                }

\<|\>|([\<\>=!]=)   {
                        logToken("RELOP", yytext);
                        printToken("RELOP", yytext);
                    }

=   {
        logToken("ASSIGNOP", yytext);
        printToken("ASSIGNOP", yytext);
    }

(&&)|(\|\|) {
                logToken("LOGICOP", yytext);
                printToken("LOGICOP", yytext);
            }

!   {
        logToken("NOTOP", yytext);
        printToken("NOTOP", yytext);
    }

,   {
        logToken("COMMA", yytext);
        printToken("COMMA", yytext);
    }

;   {
        logToken("SEMICOLON", yytext);
        printToken("SEMICOLON", yytext);
    }

\(  {
        logToken("LPAREN", yytext);
        printToken("LPAREN", yytext);
    }

\)  {
        logToken("RPAREN", yytext);
        printToken("RPAREN", yytext);
    }

\{  {
        logToken("LCURL", yytext);
        printToken("LCURL", yytext);
        symbolTable->enterScope();
    }

\}  {
        logToken("RCURL", yytext);
        printToken("RCURL", yytext);
        symbolTable->exitScope();
    }

\[  {
        logToken("LTHIRD", yytext);
        printToken("LTHIRD", yytext);
    }

\]  {
        logToken("RTHIRD", yytext);
        printToken("RTHIRD", yytext);
    }
    /* ^ Operators */

({LETTER}|_)({LETTER}|{DIGIT}|_)*   {
                                        logToken("ID", yytext);
                                        printToken("ID", yytext);
                                        insertSymbol("ID", yytext);
                                    }

    /* ^ Identifier */

{WHITESPACE}    {}

    /* ^ Whitespace */

{NEWLINE}   {
                lineCount++;
            }

    /* ^ Newline */



{FRACTION}(\.{DIGIT}*){2,}{EXPONENT}?({DIGIT}|{LETTER}|\.)*    {
                                                                std::string msg("Too many decimal points ");
                                                                msg += yytext;
                                                                
                                                                logError(msg);
                                                            }

{FRACTION}{EXPONENT}(({LETTER}|\.)({DIGIT}|{LETTER}|\.)*)?  {
                                                                std::string msg("Exponent has no digit ");
                                                                msg += yytext;
                                                                
                                                                logError(msg);
                                                            }

{FRACTION}{EXPONENT}{DIGIT}+{LETTER}({DIGIT}|{LETTER}|\.)*  {
                                                                std::string msg("Invalid suffix on Float ");
                                                                msg += yytext;
                                                                
                                                                logError(msg);
                                                            }

{FRACTION}{EXPONENT}{DIGIT}+\.({DIGIT}|{LETTER}|\.)+    {
                                                            std::string msg("Ill formed number ");
                                                            msg += yytext;
                                                                
                                                            logError(msg);
                                                        }

{DIGIT}+({LETTER}|_)({LETTER}|{DIGIT}|_)*   {
                                                std::string msg("Invalid prefix on ID or invalid suffix on Number ");
                                                msg += yytext;

                                                logError(msg);
                                            }

.   {
        std::string msg("Unrecognized character ");
        msg += yytext;

        logError(msg);
    }

    /* ^ Errors */

<<EOF>>     {
                yyterminate();
            }
%%

int main(int argc,char **argv){
    if(argc < 2){
        printf("please enter the input file name\n");
        return 0;
    }

    FILE *fileIn = fopen(argv[1],"r");
    tokenFile.open("1705076_token.txt", std::ios::out);
    logFile.open("1705076_log.txt", std::ios::out);

    symbolTable = new SymbolTable::SymbolTable(bucketCount);

	if(fileIn==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
    else{
        printf("Lexing %s\n", argv[1]);
    }

	yyin = fileIn;

	yylex();

    logFile<<std::endl;
    symbolTable->printAllScopes(logFile);

    logFile<<std::endl;
    logFile<<"Total lines: "<<lineCount<<std::endl;
    logFile<<"Total errors: "<<errorCount<<std::endl;

	fclose(fileIn);
    tokenFile.close();
    logFile.close();

    delete symbolTable;
	
	return 0;
}

char convertEscapedChar(char c){
    switch(c){
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case '\\':
            return '\\';
        case 'a':
            return '\a';
        case 'f':
            return '\f';
        case 'r':
            return '\r';
        case 'b':
            return '\b';
        case 'v':
            return '\v';
        case '0':
            return '\0';
        case '\'':
            return '\'';
        case '\"':
            return '\"';
    }
    return EOF;
}

void logToken(std::string type, std::string lexeme, std::string symbol){
    logFile<<std::endl;
    logFile<<"Line no "<<lineCount<<": Token <"<<type<<"> Lexeme "<<lexeme<<" found";
    if(!symbol.empty()){
        logFile<<" --> "<<"<"<<type<<", "<<symbol<<"> ";
    }
    logFile<<std::endl;
}

void printToken(std::string type, std::string symbol){
    tokenFile<<"<"<<type;
    if(!symbol.empty()){
        tokenFile<<", "<<symbol;
    }
    tokenFile<<"> ";
}

void insertSymbol(std::string type, std::string symbol){
    bool success = symbolTable->insert(symbol, type);

    logFile<<std::endl;
    if(success){
        symbolTable->printAllScopes(logFile);
    }
    else{
        logFile<<symbol<<" already exists in current ScopeTable"<<std::endl;
    }
    
}

void logError(std::string msg){
    errorCount++;
    logFile<<std::endl;
    logFile<<"Error at line no "<<lineCount<<": "<<msg<<std::endl;
}