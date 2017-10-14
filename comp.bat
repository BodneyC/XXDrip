@echo off
g++ -c hex_out.cc XXDrip.cc -I ./include && g++ -Wall XXDrip.o hex_out.o -o XXDrip 
