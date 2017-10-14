#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "hex_out.h"
/*-------------------------------------------------------------
FUNC: Preamble to formatted hexoutput
-------------------------------------------------------------*/
int hexbegin (int j, std::ostream &stream, int rowNum) 
{
  // Output table outline
  stream << "Hexadecimal Output" << '\n';
  stream << '\n' << "          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  : 0 1 2 3 4 5 6 7 8 9 a b c d e f" << '\n' << "          ----------------------------------------------- : -------------------------------" << '\n';
  
  // Fill empty space for the columns
  for (int i = 0; i < rowNum; i++)
  {
    stream << std::internal << std::setfill('0') << std::hex << std::showbase << std::setw(7) << j << " |                                                 :                                " << '\n';
    j += 16;
  }

  return j;
}
/*-------------------------------------------------------------
FUNC: If a single ilne of both sides missing bytes
-------------------------------------------------------------*/
void hexLT16out (BYTE *sector, std::ostream &stream, int initJ, int start, int bytecount) 
{
  stream << std::hex << std::showbase << std::setfill(' ') << std::setw(7) << initJ << " | ";

  for (int i = 0; i < start; i++)
    stream << "   ";

  for (int i = 0; i < bytecount; i++)
    stream << std::hex << std::noshowbase << std::uppercase << std::setw(2) << std::setfill('0') << (int)sector[i] << ' ';

  for (int i = 0; i < 16 - (start + bytecount); i++)
    stream << "   ";

  stream << ": ";

  for (int i = 0; i < start; i++)
    stream << "  ";

  for (int i = 0; i < bytecount; i++) 
  {
    if((sector[i] <= 0x20) || (sector[i] == 0x7F)) 
	{
      stream << ". ";
    } 
	else
	{
      stream << sector[i] << ' ';
    }
  }

  stream << '\n';
}
/*-------------------------------------------------------------
FUNC: Primary line-by-line outputter
-------------------------------------------------------------*/
void hexoutput (BYTE *sector, std::ostream &stream, int initJ, int bytecount, int mode) 
{
  static int j = initJ;

  stream << std::hex << std::showbase << std::nouppercase << std::setfill(' ') << std::setw(7) << j << " | ";

  j +=16;
  if (mode == 1) 
    for (int i = 0; i < 16-bytecount; i++)
      stream << "   ";

  for (int o = 0; o < bytecount; o++)
    stream << std::hex << std::noshowbase << std::uppercase << std::setw(2) << std::setfill('0') << (int)sector[o] << ' ';

  if (mode == 2) 
    for (int o = 0; o < 16 - bytecount; o++)
      stream << "   ";

  stream << ": ";

  if (mode == 1) 
    for (int o = 0; o < 16 - bytecount; o++) 
      stream << "  ";

  for (int o = 0; o < bytecount; o++) 
    if((sector[o] <= 0x20) || (sector[o] == 0x7F)) 
      stream << ". ";
    else
      stream << sector[o] << ' ';
    
  stream << '\n' ;
  if (j % 512 == 0)
    stream << "        ------------------------------------------------- : -------------------------------" << '\n';
}

/*
  std::vector<BYTE> readFileintoVec(const char* filename){
  // open the file:
  std::ifstream file(filename, std::ios::binary);

  // Stop eating new lines in binary mode!!!
  file.unsetf(std::ios::skipws);

  // get its size:
  std::streampos fileSize;

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // reserve capacity
  std::vector<BYTE> vec;
  vec.reserve(fileSize);

  // read the data:
  vec.insert(vec.begin(), std::istream_iterator<BYTE>(file), std::istream_iterator<BYTE>());

  return vec;
}
*/
