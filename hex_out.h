#ifndef HEX_OUT_H
#define HEX_OUT_H

typedef unsigned char BYTE;

struct inputinfo {
	int beginVal, endVal, bytestoline, loopcount, bytesafterline;
	__int64 diff;
	std::string inputString, outputString;
	bool binoutput, rawoutput;
};

struct outputInfo {
	int beginningJ, rowNumBeg, rowNumEnd, colNumBeg, colNumEnd, bytesintosector;
	bool overline;
};

class Hex_out
{
private:
	std::ostream *o_stream;

public:
	Hex_out(std::ostream& o_stream = std::cout): o_stream(&o_stream) {	}

	int hexbegin (struct outputInfo *hexInfo);
	void hexoutput (BYTE sector[], int j, int bytecount, int mode);
	void hexLT16out (BYTE *sector, int initJ, int start, int bytecount);
};

#endif
