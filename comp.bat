@echo off
g++ -c hex_out.cc && g++ -Wall -I ./include main.cc hex_out.o -o main.exe
