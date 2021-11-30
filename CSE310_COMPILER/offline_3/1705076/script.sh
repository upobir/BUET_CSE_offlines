yacc -d -y 1705076.y -o parser.cpp
echo 'parser generated'
g++ -w -c -o parser.o parser.cpp
flex -o lexer.cpp 1705076.l
echo 'lexer generated'
g++ -w -c -o lexer.o lexer.cpp 
g++ -c -o SymbolInfo.o SymbolInfo.cpp
g++ -c -o ScopeTable.o ScopeTable.cpp
g++ -c -o SymbolTable.o SymbolTable.cpp
g++ -o compiler.out *.o -lfl
echo 'compiler.out generated'
./compiler.out input.txt
echo 'finished'