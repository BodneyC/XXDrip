#include <string>
#include <tclap/CmdLine.h>
#include "hex_out.h"

struct inputinfo {
	int beginVal, endVal, bytestoline, loopcount, bytesafterline;
	__int64 diff;
	std::string inputString, outputString;
	bool writetofile, rawoutput;
} CMDArgs;

struct outputInfo {
	int beginningJ, rowNumBeg, rowNumEnd, colNumBeg, colNumEnd, bytesintosector;
} hexInfo;
/*------------------------------------------------------------
Argument Processing (outputInfo)
------------------------------------------------------------*/
void processArgs () {
	__int64 tempDiff;
	hexInfo.bytesintosector = CMDArgs.beginVal % 512;
	CMDArgs.bytestoline = 512 - hexInfo.bytesintosector;
	CMDArgs.bytesafterline = CMDArgs.endVal % 512;
	CMDArgs.diff = tempDiff = CMDArgs.endVal - CMDArgs.beginVal;
	tempDiff-= (CMDArgs.bytestoline + CMDArgs.bytesafterline);
	CMDArgs.loopcount = tempDiff / 512;
	hexInfo.colNumBeg = hexInfo.bytesintosector % 16;
	hexInfo.rowNumBeg = (hexInfo.bytesintosector - hexInfo.colNumBeg) / 16;
	hexInfo.colNumEnd = CMDArgs.bytesafterline % 16;
	hexInfo.rowNumEnd = (CMDArgs.bytesafterline - hexInfo.colNumEnd) / 16;
	hexInfo.beginningJ = CMDArgs.beginVal - hexInfo.bytesintosector;
}
/*------------------------------------------------------------
Argument Returning (CMDArgs)
------------------------------------------------------------*/
void returnArgs (int argn, char const *args[]) {
		try {
			TCLAP::CmdLine cmd("XXDrip", ' ', "0.01");
			TCLAP::ValueArg<std::string> inputArg("i", "input", "Input File-path", true, "empty", "String");
			TCLAP::ValueArg<int> beginArg("b", "begin", "Offset of Begin Read", false, 0, "Int");
			TCLAP::ValueArg<int> endArg("e", "end", "Offset of End Read", false, 0, "Int");
			TCLAP::SwitchArg rawSwitch("r", "raw", "Output Raw Hex", cmd, 0);
			cmd.add(inputArg);
			cmd.add(beginArg);
			cmd.add(endArg);
			cmd.parse(argn, args);
			CMDArgs.beginVal = beginArg.getValue();
			CMDArgs.inputString = inputArg.getValue();
			CMDArgs.endVal = endArg.getValue();
			CMDArgs.rawoutput = rawSwitch.getValue();
		} catch (TCLAP::ArgException &e) {
			std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
}
/*-------------------------------------------------------------
MAIN
-------------------------------------------------------------*/
int main(int argc, char const *argv[]) {
	BYTE *sectorArr = new BYTE[512];
	BYTE *lineremaining = new BYTE[16];
	BYTE *presec, *postsec;
	returnArgs(argc, argv);
	// Open inputFile and check if open
	std::ifstream inputFile (CMDArgs.inputString.c_str(), std::ios::binary);
	if(!inputFile.is_open()) {
		std::cout << "Cannot open input file" << '\n';
	}
	// If endVal hasn't been provided, set endVal to EOF
	if(!CMDArgs.endVal) {
		inputFile.seekg (0, inputFile.end);
		CMDArgs.endVal = inputFile.tellg();
	}
	// Set inputFile pointer to beginVal
	inputFile.seekg(CMDArgs.beginVal, inputFile.beg);
	// Process arguments with filebased beginVal
	processArgs();
	presec = new BYTE[CMDArgs.bytestoline];
	postsec = new BYTE[CMDArgs.bytesafterline];

	if(!CMDArgs.rawoutput) {
		hexInfo.beginningJ = hexbegin(hexInfo.beginningJ, std::cout, hexInfo.rowNumBeg);

		inputFile.read ((char*)lineremaining, 16-hexInfo.colNumBeg);
		hexoutput(lineremaining, std::cout, hexInfo.beginningJ, 16-hexInfo.colNumBeg);
		for (size_t i = 0; i < 32 - (hexInfo.rowNumBeg+1); i++) {
			inputFile.read ((char*)lineremaining, 16);
			hexoutput(lineremaining, std::cout, 0, 16);
		}

		for (size_t i = 0; i < CMDArgs.loopcount; i++) {
			for (size_t j = 0; j < 32; j++) {
				inputFile.read ((char*)lineremaining, 16);
				hexoutput(lineremaining, std::cout, 0, 16);
			}
		}

		for (size_t i = 0; i < 32 - (hexInfo.rowNumEnd+1); i++) {
			inputFile.read ((char*)lineremaining, 16);
			hexoutput(lineremaining, std::cout, 0, 16);
		}
		inputFile.read ((char*)lineremaining, 16-hexInfo.colNumEnd);
		hexoutput(lineremaining, std::cout, hexInfo.beginningJ, 16-hexInfo.colNumEnd);
		// Loop:
			// Read 512 bytes
			// Send to hexoutput

		// Read rmaining bytes
		// Send to hexoutput
	} else {
		// outputInfo.diff / 512
		// R/w
		// outputInfo.diff % 512
		// R/w
	}
	delete[] sectorArr;
	return 0;
}
