#include "hex_out.h"

void hexbegin (int beginingJ, std::ostream &stream) {
  stream << "Hexadecimal Output" << '\n';
  stream << '\n' << "        0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  : 0 1 2 3 4 5 6 7 8 9 a b c d e f" << '\n';
  stream << "       ------------------------------------------------ : -------------------------------" << '\n' << std::internal << std::setfill('0') << std::hex << std::showbase << std::setw(5) << j << " | ";
}

void hexoutput (BYTE sector[], int bytecount, std::ostream &stream) {
  int i = 0; //Overall byte count
  int k = 0; //Line count
  int j = 0; //Sector line count
  int a = 0; //Char count

  j+=16;
  while( i != bytecount )	{
    if (k != 15) { //Print one char, repeat
      stream << std::hex << std::noshowbase << std::setw(2) << (int)sector[i] << ' ';
      k++;
    } else if (k == 15) {
      if(j % 512 != 0) {//Check for sector break
        stream << std::hex << std::noshowbase <<  std::setw(2) << (int)sector[i] << " : ";
        i-=15; //Return line for char
        for (a=0; a <= 15; a++) {//char loop
          if((sector[i] <= 0x20) || (sector[i] == 0x7F)) {
            stream << ". ";
          } else {
            stream << sector[i] << ' ';
          }
          i++; //Compensate for subbing 15
        }
        i--;
        stream << '\n' << std::hex << std::showbase << std::setw(5) << j << " | ";
        k = 0; //For the new line
        j = j+16; //Setup next line
      } else {
        stream << std::hex << std::noshowbase << std::setw(2) << (int)sector[i] << " : ";
        i-=15;
        for (a=0; a <= 15; a++) { //Print chars
          if((sector[i] <= 0x20) || (sector[i] == 0x7F)) { //ASCII control
            stream << ". ";
          } else {
            stream << sector[i] << ' ';
          }
          i++;
        }
        i--;
        if(i != bytecount-1) {
          stream << '\n' << "       ------------------------------------------------ : -------------------------------" << '\n' << std::internal << std::setfill('0') << std::hex << std::showbase << std::setw(5) << j << " | ";
        }
        j=j+16;
        k = 0;
      }
    }
    i++;
  }
}

// std::vector<BYTE> readFileintoVec(const char* filename){
//   // open the file:
//   std::ifstream file(filename, std::ios::binary);
//
//   // Stop eating new lines in binary mode!!!
//   file.unsetf(std::ios::skipws);
//
//   // get its size:
//   std::streampos fileSize;
//
//   file.seekg(0, std::ios::end);
//   fileSize = file.tellg();
//   file.seekg(0, std::ios::beg);
//
//   // reserve capacity
//   std::vector<BYTE> vec;
//   vec.reserve(fileSize);
//
//   // read the data:
//   vec.insert(vec.begin(), std::istream_iterator<BYTE>(file), std::istream_iterator<BYTE>());
//
//   return vec;
// }
