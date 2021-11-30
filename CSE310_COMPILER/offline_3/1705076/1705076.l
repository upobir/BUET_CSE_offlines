%option noyywrap

%x CHARACTER
%x STRING
%x SINGLE_COMMENT
%x MULTI_COMMENT

%{
// libraries
#include<stdio.h>
#include<cstdlib>
#include<string>

// symbol table
#include "SymbolInfo.h"
using SymbolTable::SymbolInfo;

void printError(std::string message);
#include "Node.hpp"

#define YYSTYPE Node*
#include "parser.hpp"


int lineCount = 1;
int tempNewLines = 0;

std::string currentToken, currentLexeme;

char convertEscapedChar(char c);
void logToken(std::string type, std::string lexeme, std::string symbol = "");
void printToken(std::string type, std::string symbol = "");
void logError(std::string msg);


extern YYSTYPE yylval;

// TODO what to do about print token

%}

WHITESPACE [ \t\f\r\v]+ 
LETTER [a-zA-Z]
DIGIT [0-9]
EXPONENT ([eE][\+\-]?)
FRACTION (({DIGIT}+(\.{DIGIT}*)?)|(\.{DIGIT}+))
ESCAPE (\\[\\ntafrbv0\'\"])
NEWLINE (\n|(\r\n))



%%

"printf"    {
                logToken("PRINTLN", yytext);
                printToken("PRINTLN");
                return PRINTLN;
            }

"if"        {
                logToken("IF", yytext);
                printToken("IF");
                return IF;
            }

"else"      {
                logToken("ELSE", yytext);
                printToken("ELSE");
                return ELSE;
                
            }

"for"      {
                logToken("FOR", yytext);
                printToken("FOR");
                return FOR;
            }

"while"     {
                logToken("WHILE", yytext);
                printToken("WHILE");
                return WHILE;
            }

"do"        {
                logToken("DO", yytext);
                printToken("DO");
                // TODO unspecified
            }

"break"     {
                logToken("BREAK", yytext);
                printToken("BREAK");
                // TODO unspecified
            }

"int"       {
                logToken("INT", yytext);
                printToken("INT");
                return INT;
            }

"char"      {
                logToken("CHAR", yytext);
                printToken("CHAR");
                // TODO unspecified
            }

"float"     {
                logToken("FLOAT", yytext);
                printToken("FLOAT");
                return FLOAT;
            }

"double"    {
                logToken("DOUBLE", yytext);
                printToken("DOUBLE");
                // TODO unspecified
            }

"void"      {
                logToken("VOID", yytext);
                printToken("VOID");
                return VOID;
            }

"return"    {
                logToken("RETURN", yytext);
                printToken("RETURN");
                return RETURN;
            }

"switch"    {
                logToken("SWITCH", yytext);
                printToken("SWITCH");
                // TODO unspecified
            }

"case"      {
                logToken("CASE", yytext);
                printToken("CASE");
                // TODO unspecified
            }

"default"   {
                logToken("DEFAULT", yytext);
                printToken("DEFAULT");
                // TODO unspecified
            }

"continue"  {
                logToken("CONTINUE", yytext);
                printToken("CONTINUE");
                // TODO unspecified
            }

    /* ^ Keywords */

{DIGIT}+    {
                logToken("CONST_INT", yytext);
                printToken("CONST_INT", yytext);
                yylval = new Node{.text = yytext};
                return CONST_INT;
            }

    /* ^ Integer */

{FRACTION}({EXPONENT}{DIGIT}+)? {
                                    logToken("CONST_FLOAT", yytext);
                                    printToken("CONST_FLOAT", yytext);
                                    yylval = new Node{.text = yytext};
                                    return CONST_FLOAT;
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
                    }

                    BEGIN INITIAL;

                    // TODO unspecified
                }

<CHARACTER>{NEWLINE}    {
                            logError(std::string("Unterminated character ") + currentLexeme);
                            lineCount++;

                            BEGIN INITIAL;

                            // TODO unspecified
                        }

<CHARACTER><<EOF>>      {
                            logError(std::string("Unterminated character ") + currentLexeme);

                            yyterminate();

                            // TODO unspecified
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

                // TODO unspecified
            }

<STRING>{NEWLINE}   {
                        logError(std::string("Unterminated String ") + currentLexeme);

                        lineCount += tempNewLines;
                        lineCount++;

                        BEGIN INITIAL;

                        // TODO unspecified
                    }

<STRING><<EOF>>     {
                        logError(std::string("Unterminated String ") + currentLexeme);

                        lineCount += tempNewLines;

                        yyterminate();

                        // TODO unspecified
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
            yylval = new Node{.text = yytext};
            return ADDOP;
        }

[\*/%]  {
            logToken("MULOP", yytext);
            printToken("MULOP", yytext);
            yylval = new Node{.text = yytext};
            return MULOP;
        }

(\+\+)  {
            logToken("INCOP", yytext);
            printToken("INCOP", yytext);
            yylval = new Node{.text = yytext};
            return INCOP;
        }

(\-\-)  {
            logToken("DECOP", yytext);
            printToken("DECOP", yytext);
            yylval = new Node{.text = yytext};
            return DECOP;
        }

\<|\>|([\<\>=!]=)   {
                        logToken("RELOP", yytext);
                        printToken("RELOP", yytext);
                        yylval = new Node{.text = yytext};
                        return RELOP;
                    }

=   {
        logToken("ASSIGNOP", yytext);
        printToken("ASSIGNOP", yytext);
        return ASSIGNOP;
    }

(&&)|(\|\|) {
                logToken("LOGICOP", yytext);
                printToken("LOGICOP", yytext);
                yylval = new Node{.text = yytext};
                return LOGICOP;
            }

!   {
        logToken("NOTOP", yytext);
        printToken("NOTOP", yytext);
        return NOT;
    }

,   {
        logToken("COMMA", yytext);
        printToken("COMMA", yytext);
        return COMMA;
    }

;   {
        logToken("SEMICOLON", yytext);
        printToken("SEMICOLON", yytext);
        return SEMICOLON;
    }

\(  {
        logToken("LPAREN", yytext);
        printToken("LPAREN", yytext);
        return LPAREN;
    }

\)  {
        logToken("RPAREN", yytext);
        printToken("RPAREN", yytext);
        return RPAREN;
    }

\{  {
        logToken("LCURL", yytext);
        printToken("LCURL", yytext);
        return LCURL;
    }

\}  {
        logToken("RCURL", yytext);
        printToken("RCURL", yytext);
        return RCURL;
    }

\[  {
        logToken("LTHIRD", yytext);
        printToken("LTHIRD", yytext);
        return LTHIRD;
    }

\]  {
        logToken("RTHIRD", yytext);
        printToken("RTHIRD", yytext);
        return RTHIRD;
    }
    /* ^ Operators */

({LETTER}|_)({LETTER}|{DIGIT}|_)*   {
                                        logToken("ID", yytext);
                                        printToken("ID", yytext);
                                        yylval = new Node{.text = yytext};
                                        return ID;
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
                                                                
                                                                printError(msg);
                                                            }

{FRACTION}{EXPONENT}(({LETTER}|\.)({DIGIT}|{LETTER}|\.)*)?  {
                                                                std::string msg("Exponent has no digit ");
                                                                msg += yytext;
                                                                
                                                                printError(msg);
                                                            }

{FRACTION}{EXPONENT}{DIGIT}+{LETTER}({DIGIT}|{LETTER}|\.)*  {
                                                                std::string msg("Invalid suffix on Float ");
                                                                msg += yytext;
                                                                
                                                                printError(msg);
                                                            }

{FRACTION}{EXPONENT}{DIGIT}+\.({DIGIT}|{LETTER}|\.)+    {
                                                            std::string msg("Ill formed number ");
                                                            msg += yytext;
                                                                
                                                            printError(msg);
                                                        }

{DIGIT}+({LETTER}|_)({LETTER}|{DIGIT}|_)*   {
                                                std::string msg("Invalid prefix on ID or invalid suffix on Number ");
                                                msg += yytext;

                                                printError(msg);
                                            }

.   {
        std::string msg("Unrecognized character ");
        msg += yytext;

        printError(msg);
    }

    /* ^ Errors */

<<EOF>>     {
                yyterminate();
            }
%%



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

void logToken(std::string type, std::string lexeme, std::string symbol) {}
// {
//     logFile<<std::endl;
//     logFile<<"Line no "<<lineCount<<": Token <"<<type<<"> Lexeme "<<lexeme<<" found";
//     if(!symbol.empty()){
//         logFile<<" --> "<<"<"<<type<<", "<<symbol<<"> ";
//     }
//     logFile<<std::endl;
// }

void printToken(std::string type, std::string symbol) {}
// {

//     tokenFile<<"<"<<type;
//     if(!symbol.empty()){
//         tokenFile<<", "<<symbol;
//     }
//     tokenFile<<"> ";

//     return;
// }

void logError(std::string msg) {}
// {
//     errorCount++;
//     logFile<<std::endl;
//     logFile<<"Error at line no "<<lineCount<<": "<<msg<<std::endl;
// }