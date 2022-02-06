#!/bin/bash

g++ -std=c++17 -Wall -Wconversion -Wshadow -c *.cpp

g++ -o markov.out *.o
