@echo off

rem set path=C:\Program Files (x86)\CodeBlocks\MinGW\bin;%path%

@echo on

g++ --std=c++14 -Iinclude -Llib -o bin/%1.exe src/%1.cpp -lGlaux -lGLU32 -lglui32 -lglut32 -lOPENGL32