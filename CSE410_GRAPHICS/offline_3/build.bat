@echo off

rem set path=C:\Program Files (x86)\CodeBlocks\MinGW\bin;%path%

@echo on

g++ --std=c++14 -O2 -s -Iinclude -Llib -o bin/main.exe src/1705076_main.cpp -lGlaux -lGLU32 -lglui32 -lglut32 -lOPENGL32