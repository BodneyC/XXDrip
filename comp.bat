@echo on
g++ -I ./include -c hex_out.cc XXDrip.cc && g++ -Wall -o XXDrip XXDrip.o hex_out.o 
