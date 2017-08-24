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
	int beginingJ, rowNum, colNum;
} hexInfo;
/*------------------------------------------------------------
Argument Processing (outputInfo)
------------------------------------------------------------*/
void processArgs () {
	__int64 tempDiff;
	hexInfo.beginingJ = CMDArgs.beginVal % 512;
	CMDArgs.bytestoline = 512 - hexInfo.beginingJ;
	CMDArgs.bytesafterline = CMDArgs.endVal % 512;
	CMDArgs.diff = tempDiff = CMDArgs.beginVal - CMDArgs.endVal;
	tempDiff-= (CMDArgs.bytestoline + CMDArgs.bytesafterline);
	CMDArgs.loopcount = tempDiff / 512;
	hexInfo.rowNum = hexInfo.beginingJ % 16;
	hexInfo.colNum = (hexInfo.beginingJ - hexInfo.rowNum) / 16;
}
/*------------------------------------------------------------
Argument Returning (CMDArgs)
------------------------------------------------------------*/
void returnArgs (int argn, char const *args[]) {
		try {
			TCLAP::CmdLine cmd("XXDrip", ' ', "0.01");
			TCLAP::ValueArg<std::string> inputArg("i", "input", "Input File-path", true, "empty", "String");
			TCLAP::ValueArg<std::string> outputArg("o", "output", "Output File-path", false, "output.txt", "String");
			TCLAP::ValueArg<int> beginArg("b", "begin", "Offset of Begin Read", false, 0, "Int");
			TCLAP::ValueArg<int> endArg("e", "end", "Offset of End Read", false, 0, "Int");
			TCLAP::SwitchArg writeSwitch("w", "write", "Write to File [-o]", cmd, 0);
			TCLAP::SwitchArg rawSwitch("r", "raw", "Output Raw Hex", cmd, 0);
			cmd.add(inputArg);
			cmd.add(outputArg);
			cmd.add(beginArg);
			cmd.add(endArg);
			cmd.parse(argn, args);
			CMDArgs.beginVal = beginArg.getValue();
			CMDArgs.inputString = inputArg.getValue();
			CMDArgs.outputString = outputArg.getValue();
			CMDArgs.endVal = endArg.getValue();
			CMDArgs.writetofile = writeSwitch.getValue();
			CMDArgs.rawoutput = rawSwitch.getValue();
		} catch (TCLAP::ArgException &e) {
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		}
}
/*-------------------------------------------------------------
MAIN
-------------------------------------------------------------*/
int main(int argc, char const *argv[]) {
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
	// If -w, open outputFile and check if open
	if(CMDArgs.writetofile) {
		std::ofstream  outputFile(CMDArgs.outputString.c_str(), std::ios::trunc);
		if(!outputFile.is_open()) {
			std::cout << "Cannot open output file" << '\n';
		}
	}

	if(!CMDArgs.rawoutput) {
		// Read X bytes up to sector lines
		// Send to hexoutput

		// Loop:
			// Read 512 bytes
			// Send to hexoutput

		// Read rmaining bytes
		// Send to hexoutput
	} else {
		// outputInfo.diff / 512
	}
	return 0;
}
