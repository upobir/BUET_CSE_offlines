@echo off

dir /s /b *.java > sources.txt
javac @sources.txt