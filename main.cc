#include <string>
#include <tclap/CmdLine.h>
#include "hex_out.h"

struct inputinfo {
	int beginVal, endVal, bytestoline, loopcount, bytesafterline;
	__int64 diff;
	std::string inputString, outputString;
	bool binoutput, rawoutput;
} CMDArgs;

struct outputInfo {
	int beginningJ, rowNumBeg, rowNumEnd, colNumBeg, colNumEnd, bytesintosector;
} hexInfo;
/*------------------------------------------------------------
Argument Processing (outputInfo)
------------------------------------------------------------*/
void processArgs () {
	__int64 tempDiff;
	CMDArgs.diff = tempDiff = CMDArgs.endVal - CMDArgs.beginVal;
	hexInfo.bytesintosector = CMDArgs.beginVal % 512;
	CMDArgs.bytestoline = 512 - hexInfo.bytesintosector;
	CMDArgs.bytesafterline = CMDArgs.endVal % 512;
	CMDArgs.loopcount = tempDiff / 16;
	if (tempDiff < 16) CMDArgs.loopcount = 0;
	hexInfo.colNumBeg = hexInfo.bytesintosector % 16;
	hexInfo.rowNumBeg = (hexInfo.bytesintosector - hexInfo.colNumBeg) / 16;
	hexInfo.colNumEnd = CMDArgs.bytesafterline % 16;
	hexInfo.rowNumEnd = CMDArgs.bytesafterline / 16;
	hexInfo.beginningJ = CMDArgs.beginVal - hexInfo.bytesintosector;
}
/*------------------------------------------------------------
Argument Returning (CMDArgs)
------------------------------------------------------------*/
void returnArgs (int argn, char const *args[]) {
	try {
		TCLAP::CmdLine cmd("XXDrip", ' ', "0.01");
		TCLAP::ValueArg<std::string> inputArg("i", "input", "Input File-path", true, "empty", "String");
		TCLAP::ValueArg<std::string> outputArg("o", "output", "Output to File-path", false, "empty", "String");
		TCLAP::ValueArg<int> beginArg("s", "start", "Offset of Begin Read", false, 0, "Int");
		TCLAP::ValueArg<int> endArg("e", "end", "Offset of End Read", false, 0, "Int");
		TCLAP::SwitchArg rawSwitch("r", "raw", "Output Raw Hex", cmd, 0);
		TCLAP::SwitchArg binSwitch("b", "binary", "Output in binary mode", cmd, 0);
		cmd.add(inputArg);
		cmd.add(outputArg);
		cmd.add(beginArg);
		cmd.add(endArg);
		cmd.parse(argn, args);
		CMDArgs.beginVal = beginArg.getValue();
		CMDArgs.inputString = inputArg.getValue();
		CMDArgs.outputString = outputArg.getValue();
		CMDArgs.endVal = endArg.getValue();
		CMDArgs.rawoutput = rawSwitch.getValue();
		CMDArgs.binoutput = binSwitch.getValue();
	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}
/*-------------------------------------------------------------
MAIN
-------------------------------------------------------------*/
int main(int argc, char const *argv[]) {
	BYTE *sectorArr = new BYTE[512];
	BYTE *lineArr = new BYTE[16];
	bool overline = false;
	returnArgs(argc, argv);
	// outputFile if needed
	std::ofstream outputFile;
	std::ifstream inputFile (CMDArgs.inputString.c_str(), std::ios::binary);
	// Open inputFile and check if open
	if(!inputFile.is_open()) {
		std::cout << "Cannot open input file" << '\n';
	}
	// Open output file as binary if requested...
	if (CMDArgs.outputString != "empty" && CMDArgs.binoutput == true) {
		CMDArgs.rawoutput = true; // If outputting as binary, also output as raw
		outputFile.open (CMDArgs.outputString.c_str(), std::ios::binary | std::ios::trunc);
		if(!outputFile.is_open()) {
			std::cout << "Cannot open output file as binary" << '\n';
		}
	// If not as binary, output formatted
	} else if (CMDArgs.outputString != "empty" && CMDArgs.binoutput == false) {
		outputFile.open (CMDArgs.outputString.c_str(), std::ios::trunc);
		if(!outputFile.is_open()) {
			std::cout << "Cannot open output file" << '\n';
		}
	}
	std::ostream &stream = CMDArgs.outputString == "empty" ? std::cout : outputFile; // Set stream to cout or outputFile depending
	// If endVal hasn't been provided, set endVal to EOF
	if(!CMDArgs.endVal) {
		inputFile.seekg (0, inputFile.end);
		CMDArgs.endVal = inputFile.tellg();
	}
	inputFile.seekg(CMDArgs.beginVal, inputFile.beg);	// Set inputFile pointer to beginVal
	processArgs();	// Process arguments with filebased beginVal
	if ((inputFile.tellg() % 16) + (CMDArgs.diff % 16) > 16 || CMDArgs.diff > 16) overline = true;
	// If output is not meant as raw...
	if(!CMDArgs.rawoutput) {
		hexInfo.beginningJ = hexbegin(hexInfo.beginningJ, stream, hexInfo.rowNumBeg); // Output starting formatting
		inputFile.read ((char*)lineArr, 16);
		// Section for first-liners
		if (inputFile.tellg() % 16 == 0 && CMDArgs.diff < 16) {
			hexoutput(lineArr, stream, hexInfo.beginningJ, hexInfo.colNumEnd, 2); // Post-value whitespace
		} else if (inputFile.tellg() % 16 == 0 && CMDArgs.diff >= 16) {
			hexoutput(lineArr, stream, hexInfo.beginningJ, 16, 0); // Full first-line
		} else if (inputFile.tellg() % 16 > 0 && CMDArgs.diff > 16 - (inputFile.tellg() % 16)) {
			hexoutput(lineArr, stream, hexInfo.beginningJ, 16-hexInfo.colNumBeg, 1); // Pre-value whitespace, more than one line
		} else {
			hexLT16out(lineArr, stream, hexInfo.beginningJ, hexInfo.colNumBeg, CMDArgs.diff); // Pre-value whitespace, single line
		}
		// If the output goes over the 16-byte mark, loop and output
		if(overline) {
			for (int i = 0; i < (CMDArgs.loopcount)-1; i++) {
				inputFile.read ((char*)lineArr, 16);
				hexoutput(lineArr, stream, 0, 16, 0);
			}
			if (CMDArgs.diff % 16 != 0) {
				inputFile.read ((char*)lineArr, 16);
				hexoutput(lineArr, stream, hexInfo.beginningJ, hexInfo.colNumEnd, 2);
			}
		}
	// If not outputting as raw
	} else {
		for (int i = 0; i < CMDArgs.diff / 16; i++) {
			inputFile.read ((char*)lineArr, 16);
			if (CMDArgs.binoutput == true) {
				stream.write((char*)lineArr, 16); // If binary/raw out, write as binary
			} else {
				for (int p = 0; p < 16; p++)
					stream << (int)lineArr[p]; // If not binary but raw out, write as plaintext
			}
		}
		// Handling the last portion of a line
		inputFile.read ((char*)lineArr, 16);
		if (CMDArgs.binoutput == true) {
			stream.write((char*)lineArr, CMDArgs.diff % 16);
		} else {
			for (int i = 0; i < CMDArgs.diff % 16; i++)
				stream << (int)lineArr[i];
		}
	}
	std::cout << '\n' << "Program complete" << '\n';
	// Closing handles and freeing memory
	inputFile.close(); outputFile.close();
	delete[] sectorArr; delete[] lineArr;
	return 0;
}
