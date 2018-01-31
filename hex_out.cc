#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "hex_out.h"

/*-------------------------------------------------------------
 * FUNC: Preamble to formatted hexoutput
 *-------------------------------------------------------------*/
int Hex_out::hexbegin (struct outputInfo *hexInfo)
{
	// Output table outline
	(*o_stream) << "Hexadecimal Output" << '\n';
	(*o_stream) << '\n' << "		  0  1	2  3  4  5	6  7  8  9	a  b  c  d	e  f  : 0 1 2 3 4 5 6 7 8 9 a b c d e f" << '\n' << "		   ----------------------------------------------- : -------------------------------" << '\n';

	// Fill empty space for the columns
	for (int i = 0; i < hexInfo->rowNumBeg; i++) {
		(*o_stream) << std::internal << std::setfill('0') << std::hex << std::showbase << std::setw(7) << hexInfo->beginningJ << " |												 :								  " << '\n';
		hexInfo->beginningJ += 16;
	}

	return hexInfo->beginningJ;
}

/*-------------------------------------------------------------
 * FUNC: If a single ilne of both sides missing bytes
 *-------------------------------------------------------------*/
void Hex_out::hexLT16out (BYTE *sector, int initJ, int start, int bytecount)
{
	(*o_stream) << std::hex << std::showbase << std::setfill(' ') << std::setw(7) << initJ << " | ";

	for (int i = 0; i < start; i++)
		(*o_stream) << "   ";

	for (int i = 0; i < bytecount; i++)
		(*o_stream) << std::hex << std::noshowbase << std::uppercase << std::setw(2) << std::setfill('0') << (int)sector[i] << ' ';

	for (int i = 0; i < 16 - (start + bytecount); i++)
		(*o_stream) << "   ";

	(*o_stream) << ": ";

	for (int i = 0; i < start; i++)
		(*o_stream) << "  ";

	for (int i = 0; i < bytecount; i++) {
		if((sector[i] <= 0x20) || (sector[i] == 0x7F)) {
			(*o_stream) << ". ";
		} else
			(*o_stream) << sector[i] << ' ';
	}

	(*o_stream) << '\n';
}

/*-------------------------------------------------------------
 * FUNC: Primary line-by-line outputter
 *-------------------------------------------------------------*/
void Hex_out::hexoutput (BYTE *sector, int initJ, int bytecount, int mode)
{
	static int j = initJ;

	(*o_stream) << std::hex << std::showbase << std::nouppercase << std::setfill(' ') << std::setw(7) << j << " | ";

	j += 16;

	if (mode == 1)
		for (int i = 0; i < 16 - bytecount; i++)
			(*o_stream) << "   ";

	for (int o = 0; o < bytecount; o++)
		(*o_stream) << std::hex << std::noshowbase << std::uppercase << std::setw(2) << std::setfill('0') << (int)sector[o] << ' ';

	if (mode == 2)
		for (int o = 0; o < 16 - bytecount; o++)
			(*o_stream) << "   ";

	(*o_stream) << ": ";

	if (mode == 1)
		for (int o = 0; o < 16 - bytecount; o++)
			(*o_stream) << "  ";

	for (int o = 0; o < bytecount; o++)
		if((sector[o] <= 0x20) || (sector[o] == 0x7F))
			(*o_stream) << ". ";
		else
			(*o_stream) << sector[o] << ' ';

	(*o_stream) << '\n' ;

	if (j % 512 == 0)
		(*o_stream) << "		------------------------------------------------- : -------------------------------" << '\n';
}

