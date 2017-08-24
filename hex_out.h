#ifndef HEX_OUT_H
#define HEX_OUT_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

typedef unsigned char BYTE;

int hexbegin (int j, std::ostream &stream, int rowNum);
void hexoutput (BYTE sector[], std::ostream &stream, int j, int bytecount, int mode);

#endif
