#include "hex_out.h"

int hexbegin (int j, std::ostream &stream, int rowNum) {
  // Output table outline
  stream << "Hexadecimal Output" << '\n';
  stream << '\n' << "        0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  : 0 1 2 3 4 5 6 7 8 9 a b c d e f" << '\n' << "        ----------------------------------------------- : -------------------------------";
  // Fill empty space for the columns
  for (int i = 0; i < rowNum; i++) {
    j += 16;
    stream << "                                                :                                " << '\n' << std::internal << std::setfill('0') << std::hex << std::showbase << std::setw(5) << j << " | ";
  }
  return j;
}
//----------------------------------------------------------
void hexoutput (BYTE sector[], std::ostream &stream, int initJ, int bytecount, int mode) {
  int i = 0; //Overall byte count
  int k = 0; //col count
  int a = 0; //Char count
  static int j = initJ+1;

  if (j % 512 == 0) {
    stream << '\n' << "        ----------------------------------------------- : -------------------------------";
  }
  j--;
  stream << '\n'<< std::hex << std::showbase << std::setw(5) << j << " | ";

  // if (bytecount < 16) {
  //   k = (16 - bytecount)+1;
  //   // Fill empty space for the rows
  // }
  j +=16;
  if (mode == 1) {
    for (int i = 0; i < 16-bytecount; i++) {
      stream << "   ";
    }
  }

  for (int o = 0; o < bytecount; o++)
    stream << std::hex << std::noshowbase << std::setw(2) << (int)sector[o] << ' ';
  if (mode == 2) {
    for (int o = 0; o < 16 - bytecount; o++) {
      stream << "   ";
    }
  }
  stream << ": ";
  if (mode == 1) {
    for (int o = 0; o < 16 - bytecount; o++) {
      stream << "  ";
    }
  }
  for (int o = 0; o < bytecount; o++) {
    if((sector[o] <= 0x20) || (sector[o] == 0x7F)) {
      stream << ". ";
    } else {
      stream << sector[o] << ' ';
    }
  }
/*
  j += 16;
  while( i != bytecount )	{
    if (k != 16) { //Print one char, repeat
      stream << std::hex << std::noshowbase << std::setw(2) << (int)sector[i] << ' ';
      k++;
    } else {
      // stream << std::hex << std::noshowbase << std::setw(2) << (int)sector[i] << ' ';
      if (mode == 2) {
        for (size_t o = 0; o < (16 - bytecount)+1; o++) {
          stream << "   ";
        }
        stream << ": ";
      } else {
        // stream << std::hex << std::noshowbase <<  std::setw(2) << (int)sector[i] << " : ";
        stream << " : ";
      }
      i-= (bytecount-1);
      if (mode == 1) {
        for (size_t o = 0; o < 16 - bytecount; o++) {
          stream << "  ";
        }
      }
      for (a=0; a < bytecount; a++) {//char loop
        if((sector[i] <= 0x20) || (sector[i] == 0x7F)) {
          stream << ". ";
        } else {
          stream << sector[i] << ' ';
        }
        i++; //Compensate for subbing 15
      }
      i--;
      k = 0; //For the new line
    }
    i++;
  }
*/
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
