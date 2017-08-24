#ifndef HEX_OUT_H
#define HEX_OUT_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

typedef unsigned char BYTE;

void hexoutput (BYTE *sector, int bytecount, std::ostream &stream);
std::vector<BYTE> readFileintoVec(const char* filename);

#endif
