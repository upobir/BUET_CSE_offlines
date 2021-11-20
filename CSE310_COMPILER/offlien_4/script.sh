yacc -d -y 1705076.y -o parser.cpp
echo 'parser generated'
g++ -g -w -c -o parser.o parser.cpp
flex -o lexer.cpp 1705076.l
echo 'lexer generated'
g++ -g -w -c -o lexer.o lexer.cpp 
g++ -g -c -o SymbolInfo.o SymbolInfo.cpp
g++ -g -c -o ScopeTable.o ScopeTable.cpp
g++ -g -c -o SymbolTable.o SymbolTable.cpp
g++ -g -c -o CodeGenerator.o CodeGenerator.cpp
g++ -g -o compiler.out *.o -lfl
echo 'compiler.out generated'
./compiler.out $1
echo 'finished'