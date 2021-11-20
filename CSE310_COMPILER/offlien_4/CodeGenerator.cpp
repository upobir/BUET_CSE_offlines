#include <algorithm>

#include "CodeGenerator.h"
#include "SymbolTable.h"

using SymbolTable::SymbolInfo;
using namespace std;

void CodeGenerator::printCode(ostream& os) {
    os<<".MODEL SMALL "<<endl;
    os<<".STACK 400H "<<endl;
    os<<".DATA "<<endl;
    os<<"\t_print_arg dw ?"<<endl;
    os<<data<<endl;
    os<<".CODE "<<endl;
    os<<code<<endl;

    os<<R"(println PROC
    mov _print_arg, AX
    push BX
    push DX
    cmp _print_arg, 0
    je _print_zero
    jl _print_neg
    jmp _print_non_zero
_print_zero:
    mov AH, 2
    mov DL, '0'
    int 21H
    jmp _print_newline
_print_neg:
    mov AH, 2
    mov DL, '-'
    int 21H
    neg _print_arg
_print_non_zero:
    push 0
    mov AX, _print_arg
_print_build:
    cmp AX, 0
    je _print_loop
    cwd
    mov BX, 10
    div BX
    add DL, '0'
    push DX
    jmp _print_build
_print_loop:
    pop DX
    cmp DL, 0
    je _print_newline
    mov AH, 2
    int 21H
    jmp _print_loop
_print_newline:
    mov AH, 2
    mov DL, 0DH
    int 21H
    mov DL, 0AH
    int 21H
    pop DX
    pop BX
    ret
println ENDP)";
}

CodeGenerator::CodeGenerator(SymbolTable::SymbolTable* table)
    : table(table){
    stackSizes.push_back(0);
    freeStacks.emplace_back();
}

std::string CodeGenerator::newLabel() 
{
    ++labelCount;
    return "_L"+to_string(labelCount);
}

int CodeGenerator::newTemp() 
{
    if(freeStacks.back().empty()){
        stackSizes.back() -= 2;
        return stackSizes.back();
    }
    else{
        int tmp = freeStacks.back().back();
        freeStacks.back().pop_back();
        return tmp;
    }
}

void CodeGenerator::release(Node* node) 
{
    if(!node->code.empty()){
        freeStacks.back().push_back(node->offset);
    }
}

std::string CodeGenerator::getAssembly(Node* expression) 
{
    if(expression->offset == 0){
        return expression->text;
    }
    else if(expression->offset == 1){
        return expression->text;
    }
    else{
        return "word [BP + "+to_string(expression->offset) + "]";
    }
}

string CodeGenerator::declareVariable(TYPE type, std::string name, int dimension) {
    auto info = table->lookUp(name);
    if(table->getCurrentID() == "1"){
        if(dimension){
            data += "\t"+name+" dw " + to_string(dimension) + " dup(?)\n";
        }
        else{
            data += "\t" + name + " dw ?\n";
        }

        return string();
    }
    else{
        if(info == nullptr) return "";

        int size = 2*max(dimension, 1);
        stackSizes.back() -= size;
        info->offset = stackSizes.back();
        return "";//\tsub SP, "+to_string(size)+"\t; " + name + "\toffset: "+ to_string(info->offset) +"\n";
    }
}

std::string CodeGenerator::return_expression(Node* expression) 
{
    std::string code = expression->code;
    code += "\tmov AX, "+getAssembly(expression)+"\t; return " + expression->text + "\n";
    code += "\tmov SP, BP\n";

    if(returnLabel.empty()){
        returnLabel = newLabel();
    }
    code += "\tjmp "+returnLabel+"\n";
    release(expression);
    return code;
}

void CodeGenerator::parameters(Node* parameters) 
{
    int cnt = 1;
    for(auto node : parameters->collection){
        if(!node->text.empty() && node->text != "$"){
            auto info = table->lookUp(node->text);
            if(info == nullptr) return;

            info->offset = 2*(6+parameters->collection.size()-cnt);
            cnt++;
        }
    }
}

void CodeGenerator::optimize() 
{
    size_t cur = 0;
    pair<string, string> opt;
    string optCode;
    while(cur < code.size()){
        auto nxt = code.find_first_of("\n", cur);
        if(nxt == string::npos)
            nxt = code.size();

        auto to = code.find_first_of("\n\t", code[cur] == '\t'? cur+1 : cur);
        if(to == string::npos)
            to = code.size();

        if(code.size() - cur >= 4 && code.substr(cur, 4) == "\tmov"){
            pair<string, string> me;
            auto tmp = code.find_first_of(",", cur);
            me.first = code.substr(cur+5, tmp-(cur+5));
            auto tmp2 = code.find_first_of("\t\n", tmp);
            me.second = code.substr(tmp+2, tmp2-(tmp+2));

            if(opt.first == me.first && opt.second == me.second || opt.first == me.second && opt.second == me.first){

            }
            else{
                opt = me;
                optCode += code.substr(cur, to-cur);
                optCode += "\n";
            }

            // cerr<<"'"<<me.first<<"'"<<endl;
            // cerr<<"'"<<me.second<<"'"<<endl;
        }
        else{
            opt = {""s, ""s};
            optCode += code.substr(cur, to-cur);
            optCode += "\n";
        }

        cur = nxt+1;
    }

    code = optCode;
}

void CodeGenerator::var_id(Node* id) 
{
    auto info = table->lookUp(id->text);
    if(info == nullptr) return;

    id->offset = info->offset;
    id->address = getAssembly(id);
}

void CodeGenerator::factor_var(Node* factor) 
{
    if(!factor->code.empty()){
        factor->offset = newTemp();
        factor->code += "\tmov AX, "+factor->address + "\n";
        factor->code += "\tmov "+getAssembly(factor)+", AX\t; "+factor->text+"\n";
        //factor->code += "";//\tpush "+factor->address + "\t; "+factor->text+"\toffset: "+ to_string(factor->offset) +"\n";
    }
    
}

void CodeGenerator::assignment(Node* self, Node* variable, Node* expression) 
{
    self->code = variable->code;
    string special;

    if(!self->code.empty()){
        int tmp = self->code.find_last_of("\t");
        tmp = self->code.find_last_of("\t", tmp-1);
        special = self->code.substr(tmp);
        self->code.resize(tmp);
    }
    
    self->code += expression->code;
    self->code += "\tmov AX, " + getAssembly(expression) + "\n";

    self->code += special;

    auto address = variable->address;
    self->code += "\tmov "+address+", AX\t; "+variable->text + " = "+expression->text+"\n";

    self->offset = newTemp();
    self->code += "\tmov AX, "+address + "\n";
    self->code += "\tmov "+getAssembly(self)+", AX\n";
    //self->code += "";//\tpush " + address + "\t; offset: "+ to_string(self->offset)+"\n";

    release(expression);

}

void CodeGenerator::addop(Node* self, Node* addop, Node* expression) 
{
    if(addop->text == "+"){
        self->code = expression->code;
        self->offset = expression->offset;
    }
    else{
        self->code = expression->code;
        self->code += "\tmov AX, "+getAssembly(expression)+"\n";
        self->code += "\tneg AX\t; "+self->text+"\n";
        self->offset = newTemp();
        self->code += "\tmov "+getAssembly(self)+", AX\n";//\tpush AX\t; offset: "+ to_string(self->offset)+"\n";

        release(expression);
    }

    
}

void CodeGenerator::not_op(Node* self, Node* expression) 
{
    self->code = expression->code;
    
    self->code += "\tmov AX, "+getAssembly(expression)+"\n";
    self->code += "\tcmp AX, 0\n";
    string label = newLabel();
    self->code += "\tje "+label+"\n";
    self->code += "\tmov AX, 1\n";
    self->code += label+":\n";
    self->code += "\txor AX, 1\t; "+self->text+"\n";
    self->offset = newTemp();
    self->code += "\tmov "+getAssembly(self)+", AX\n";//basicPush("AX", self->offset);
    release(expression);
}

std::string CodeGenerator::println(Node* id) 
{
    auto info = table->lookUp(id->text);
    if(info == nullptr) return "";

    id->offset = info->offset;
    string code = "\tmov AX, "+ getAssembly(id)+"\n";
    code += "\tcall println\t; println("+ id->text +")\n";
    return code;
}

void CodeGenerator::decop(Node* self, Node* variable) 
{
    self->code = variable->code;
    self->offset = newTemp();
    self->code += "\tmov AX, "+variable->address + "\n";
    self->code += "\tmov "+getAssembly(self)+", AX\n";
    //self->code += "";//\tpush "+variable->address+"\t; offset: " + to_string(self->offset) + "\n";
    self->code += "\tdec "+variable->address+"\t; "+variable->text+"--\n";
}

void CodeGenerator::incop(Node* self, Node* variable) 
{
    self->code = variable->code;
    self->offset = newTemp();
    self->code += "\tmov AX, "+variable->address + "\n";
    self->code += "\tmov "+getAssembly(self)+", AX\n";
    //self->code += "";//\tpush "+variable->address+"\t; offset: " + to_string(self->offset) + "\n";
    self->code += "\tinc "+variable->address+"\t; "+variable->text+"++\n";
}

void CodeGenerator::mulop(Node* self, Node* term, Node* op, Node* expression) 
{
    self->code = term->code;
    self->code += expression->code;
    self->code += "\tmov AX, "+getAssembly(term)+ "\n";
    auto second = getAssembly(expression);
    if(expression->offset == 1){
        self->code += "\tmov BX, "+second+"\n";
        second = "BX";
    }

    if(op->text == "*"){
        self->code += "\timul "+ second +"\t; "+ self->text +"\n";
        self->offset = newTemp();
        self->code += "\tmov "+getAssembly(self)+", AX\n";//basicPush("AX", self->offset);
    }
    else if(op->text == "/"){
        self->code += "\tCWD\n";

        self->code += "\tIDIV "+second+"\t; " + self->text +"\n";
        self->offset = newTemp();
        self->code += "\tmov "+getAssembly(self)+", AX\n";//basicPush("AX", self->offset);
    }
    else{
        self->code += "\tCWD\n";

        self->code += "\tIDIV "+second+"\t; " + self->text +"\n";
        self->offset = newTemp();
        self->code += "\tmov "+getAssembly(self)+", DX\n";//basicPush("DX", self->offset);
    }

    release(term);
    release(expression);
}

void CodeGenerator::addop(Node* self, Node* term, Node* op, Node* expression) 
{
    self->code = term->code;
    self->code += expression->code;
    self->code += "\tmov AX, "+getAssembly(term)+ "\n";

    if(op->text == "+"){
        self->code += "\tadd AX, "+ getAssembly(expression) +"\t; "+ self->text +"\n";
        self->offset = newTemp();
        self->code += "\tmov "+getAssembly(self)+", AX\n";//basicPush("AX", self->offset);
    }
    else{
        self->code += "\tsub AX, "+ getAssembly(expression) +"\t; "+ self->text +"\n";
        self->offset = newTemp();
        self->code += "\tmov "+getAssembly(self)+", AX\n";//basicPush("AX", self->offset);
    }

    release(term);
    release(expression);
}

void CodeGenerator::rel_expression(Node* self, Node* expr1, Node* op, Node* expr2) 
{
    self->code += expr1->code;
    self->code += expr2->code;
    self->code += "\tmov BX, "+getAssembly(expr1)+"\n";
    self->code += "\txor AX, AX\n";
    self->code += "\tcmp BX, "+getAssembly(expr2)+"\t; "+self->text+"\n";
    auto label = newLabel();
    if(op->text == "<"){
        self->code += "\tjge "+label+"\n";
    }
    else if(op->text == ">"){
        self->code += "\tjle "+label+"\n";
    }
    else if(op->text == "<="){
        self->code += "\tjg "+label+"\n";
    }
    else if(op->text == ">="){
        self->code += "\tjl "+label+"\n";
    }
    else if(op->text == "=="){
        self->code += "\tjne "+label+"\n";
    }
    else if(op->text == "!="){
        self->code += "\tje "+label+"\n";
    }
    self->code += "\tmov AX, 1\n";
    self->code += label+":\n";
    self->offset = newTemp();
    self->code += "\tmov "+getAssembly(self)+", AX\n";//basicPush("AX", self->offset);

    release(expr1);
    release(expr2);
}

void CodeGenerator::logic_expression(Node* self, Node* expr1, Node* op, Node* expr2) 
{
    self->offset = newTemp();
    auto label = newLabel();

    if(op->text == "&&"){

        self->code += "\tmov "+getAssembly(self)+", 0\n";//basicPush("0", self->offset);
    
        self->code += expr1->code;
        self->code += "\tcmp "+getAssembly(expr1)+", 0\n";
        self->code += "\tje "+label+"\n";

        self->code += expr2->code;
        self->code += "\tcmp "+getAssembly(expr2)+", 0\n";
        self->code += "\tje "+label+"\n";
    
        self->code += "\tmov "+getAssembly(self)+", 1\t; "+ self->text +"\n";
    }
    else{
        self->code += "\tmov "+getAssembly(self)+", 1\n";//basicPush("1", self->offset);
    
        self->code += expr1->code;
        self->code += "\tcmp "+getAssembly(expr1)+", 0\n";
        self->code += "\tjne "+label+"\n";

        self->code += expr2->code;
        self->code += "\tcmp "+getAssembly(expr2)+", 0\n";
        self->code += "\tjne "+label+"\n";
    
        self->code += "\tmov "+getAssembly(self)+", 0\t; "+ self->text +"\n";
    }

    self->code += label+":\n";

    release(expr1);
    release(expr2);
}

void CodeGenerator::function_call(Node* self, Node* id, Node* args) 
{
    self->offset = newTemp();
    for(auto node : args->collection){
        self->code += node->code;
    }
    for(auto node : args->collection){
        self->code += "\tpush "+getAssembly(node)+"\n";
        release(node);
    }
    self->code += "\tcall "+id->text+"\t; "+id->text+"(..)\n";
    self->code += "\tmov "+getAssembly(self)+", AX\n";
}

std::string CodeGenerator::if_stmt(Node* expression, Node* statement) 
{
    auto code = expression->code;
    auto label = newLabel();
    code += "\tcmp "+getAssembly(expression)+", 0\t; if("+expression->text+")\n";
    code += "\tje "+label+"\n";
    code += statement->code;
    code += label+":\n";

    release(expression);

    return code;
}

std::string CodeGenerator::if_else_stmt(Node* expression, Node* stmt1, Node* stmt2) 
{
    auto code = expression->code;
    auto label = newLabel();
    auto labelFalse = newLabel();
    code += "\tcmp "+getAssembly(expression)+", 0\t; if("+expression->text+")\n";
    code += "\tje "+labelFalse+"\n";
    code += stmt1->code;
    code += "\tjmp "+label+"\n";
    code += labelFalse+":\n";
    code += stmt2->code;
    code += label+":\n";

    release(expression);
    return code;
}

std::string CodeGenerator::while_stmt(Node* expression, Node* statement) 
{
    auto labelLoop = newLabel();
    auto label = newLabel();

    auto code = labelLoop+":\n";
    code += expression->code;
    code += "\tcmp "+getAssembly(expression)+", 0\t; while("+ expression->text +")\n";
    code += "\tje "+label+"\n";
    code += statement->code;
    code += "\tjmp "+labelLoop+"\n";
    code += label+":\n";

    release(expression);

    return code;
}

std::string CodeGenerator::for_loop(Node* expr1, Node* expr2, Node* expr3, Node* stmt) 
{
    auto labelLoop = newLabel();
    auto label = newLabel();

    auto code = expr1->code;
    code += labelLoop+":\n";
    if(!expr2->code.empty()){
        code += expr2->code;
        code += "\tcmp "+getAssembly(expr2)+", 0\t; for("+expr1->text+expr2->text+expr3->text+")\n";
        code += "\tje "+label+"\n";
    }
    code += stmt->code;
    code += expr3->code;
    code += "\tjmp "+labelLoop+"\n";
    code += label+":\n";

    release(expr1);
    release(expr2);
    release(expr3);

    return code;
}

string CodeGenerator::func_defintion(Node* id, Node* parameter_list, Node* body) 
{
    
    string name = id->text;

    string code = name + " PROC\n";

    if(name == "main")
        code += "\tmov AX, @data\n\tmov DS, AX\n";

    code += "\tpush BX\n\tpush CX\n\tpush DX\n\tpush SI\n\tpush BP\n";
    code += "\tmov BP, SP\n";

    code += body->code;

    if(returnLabel.size() > 0)
        code += returnLabel+":\n";

    code += "\tpop BP\n\tpop SI\n\tpop DX\n\tpop CX\n\tpop BX\n";

    if(name == "main")
        code += "\tmov AH, 4CH\n\tint 21H\n";
    else
        code += "\tret " + to_string(2*parameter_list->collection.size()) + "\n";

    code += name + " ENDP\n\n";

    returnLabel.clear();

    //cerr<<code<<endl;

    return code;
}

std::string CodeGenerator::compound_statement(Node* statments) 
{
    string code = "\tsub SP, "s + to_string(stackSizes.end()[-2] - stackSizes.end()[-1]) + "\t; scope begin\n";
    code += statments->code;
    code += "\tadd SP, " + to_string(stackSizes.end()[-2] - stackSizes.end()[-1]) + "\t; scope end\n";
    stackSizes.pop_back();
    freeStacks.pop_back();
    return code;
}

void CodeGenerator::access(Node* self, Node* id, Node* expression) 
{
    self->code = expression->code;
    auto info = table->lookUp(id->text);
    if(info == nullptr) return;

    self->code += "\tmov SI, "+getAssembly(expression) + "\n";
    self->code += "\tadd SI, SI\n";
    if(info->offset == 0){
        self->address = "["+info->getName()+" + SI]";
    }
    else{
        self->address = "word [BP + SI + " + to_string(info->offset) + "]";
    }

    return;
}
