#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <tclap/CmdLine.h>
#include "hex_out.h"

/*------------------------------------------------------------
 * Argument Processing (outputInfo)
 *------------------------------------------------------------*/
void processArgs (struct inputinfo *CMDArgs, struct outputInfo *hexInfo)
{
	__int64 tempDiff;

	CMDArgs->diff = tempDiff = CMDArgs->endVal - CMDArgs->beginVal;
	hexInfo->bytesintosector = CMDArgs->beginVal % 512;
	CMDArgs->bytestoline = 512 - hexInfo->bytesintosector;
	CMDArgs->bytesafterline = CMDArgs->endVal % 512;
	CMDArgs->loopcount = tempDiff / 16;

	if (tempDiff < 16)
		CMDArgs->loopcount = 0;

	hexInfo->colNumBeg = hexInfo->bytesintosector % 16;
	hexInfo->rowNumBeg = (hexInfo->bytesintosector - hexInfo->colNumBeg) / 16;
	hexInfo->colNumEnd = CMDArgs->bytesafterline % 16;
	hexInfo->rowNumEnd = CMDArgs->bytesafterline / 16;
	hexInfo->beginningJ = CMDArgs->beginVal - hexInfo->bytesintosector;
}

/*------------------------------------------------------------
 * Argument Returning (CMDArgs)
 *------------------------------------------------------------*/
void returnArgs (int argn, char const *args[], struct inputinfo *CMDArgs)
{
	try {
		TCLAP::CmdLine cmd("XXDrip", ' ', "1.02");
		TCLAP::UnlabeledValueArg<std::string> inputArg("input", "Input File-path", true, "empty", "String", false);
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
		CMDArgs->beginVal = beginArg.getValue();
		CMDArgs->inputString = inputArg.getValue();
		CMDArgs->outputString = outputArg.getValue();
		CMDArgs->endVal = endArg.getValue();
		CMDArgs->rawoutput = rawSwitch.getValue();
		CMDArgs->binoutput = binSwitch.getValue();
	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}
 
/*-------------------------------------------------------------
 * Output stream if needed
 *-------------------------------------------------------------*/
int ret_ofstream(std::ofstream& outputFile, struct inputinfo *CMDArgs)
{
	// Open output file as binary if requested...
	if (CMDArgs->outputString != "empty" && CMDArgs->binoutput == true) {
		CMDArgs->rawoutput = true; // If outputting as binary, also output as raw
		outputFile.open (CMDArgs->outputString.c_str(), std::ios::binary | std::ios::trunc);

		if(!outputFile.is_open()) {
			std::cout << "Cannot open output file as binary" << '\n';
			return 1;
		}

		// If not as binary, output formatted
	} else if (CMDArgs->outputString != "empty" && CMDArgs->binoutput == false) {
		outputFile.open (CMDArgs->outputString.c_str(), std::ios::trunc);

		if(!outputFile.is_open()) {
			std::cout << "Cannot open output file" << '\n';
			return 1;
		}
	}

	return 0;
}

/*-------------------------------------------------------------
 * Formatted output option
 *-------------------------------------------------------------*/
void format_out(Hex_out hex_out, std::ifstream& inputFile, struct inputinfo *CMDArgs, struct outputInfo *hexInfo)
{
	BYTE *lineArr = new BYTE[16];
	hexInfo->beginningJ = hex_out.hexbegin(hexInfo); // Output starting formatting
	inputFile.read ((char*)lineArr, 16);

	// Section for first-liners
	if (inputFile.tellg() % 16 == 0 && CMDArgs->diff < 16)
		hex_out.hexoutput(lineArr, hexInfo->beginningJ, hexInfo->colNumEnd, 2); // Post-value whitespace
	else if (inputFile.tellg() % 16 == 0 && CMDArgs->diff >= 16)
		hex_out.hexoutput(lineArr, hexInfo->beginningJ, 16, 0); // Full first-line
	else if (inputFile.tellg() % 16 > 0 && CMDArgs->diff > 16 - (inputFile.tellg() % 16))
		hex_out.hexoutput(lineArr, hexInfo->beginningJ, 16 - hexInfo->colNumBeg, 1); // Pre-value whitespace, more than one line
	else
		hex_out.hexLT16out(lineArr, hexInfo->beginningJ, hexInfo->colNumBeg, CMDArgs->diff); // Pre-value whitespace, single line

	// If the output goes over the 16-byte mark, loop and output
	if(hexInfo->overline) {
		for (int i = 0; i < (CMDArgs->loopcount) - 1; i++) {
			inputFile.read ((char*)lineArr, 16);
			hex_out.hexoutput(lineArr, 0, 16, 0);
		}

		if (CMDArgs->diff % 16 != 0) {
			inputFile.read ((char*)lineArr, 16);
			hex_out.hexoutput(lineArr, hexInfo->beginningJ, hexInfo->colNumEnd, 2);
		}
	}

	delete[] lineArr;
}

/*-------------------------------------------------------------
 * Raw output option
 *-------------------------------------------------------------*/
void raw_out(Hex_out hex_out, std::ifstream& inputFile, std::ostream *stream, struct inputinfo *CMDArgs, struct outputInfo *hexInfo)
{
	BYTE *lineArr = new BYTE[16];
	for (int i = 0; i < CMDArgs->diff / 16; i++) {
		inputFile.read ((char*)lineArr, 16);

		if (CMDArgs->binoutput == true)
			(*stream).write((char*)lineArr, 16); // If binary/raw out, write as binary
		else
			for (int p = 0; p < 16; p++)
				(*stream) << (int)lineArr[p]; // If not binary but raw out, write as plaintext
	}

	// Handling the last portion of a line
	inputFile.read ((char*)lineArr, 16);

	if (CMDArgs->binoutput == true)
		(*stream).write((char*)lineArr, CMDArgs->diff % 16);
	else
		for (int i = 0; i < CMDArgs->diff % 16; i++)
			(*stream) << (int)lineArr[i];

	delete[] lineArr;
}

/*-------------------------------------------------------------
 * MAIN
 *-------------------------------------------------------------*/
int main(int argc, char const *argv[])
{
	struct inputinfo *CMDArgs = new struct inputinfo;
	struct outputInfo *hexInfo = new struct outputInfo;

	returnArgs(argc, argv, CMDArgs);
	std::ifstream inputFile (CMDArgs->inputString.c_str(), std::ios::binary);

	/** inputFile */
	if(!inputFile.is_open()) {
		std::cout << "Cannot open input file" << '\n';
		return 1;
	}

	/** outputFile */
	std::ofstream outputFile;
	bool output_check = ret_ofstream(outputFile, CMDArgs);
	if(output_check) 
		return 1;

	std::ostream& stream = CMDArgs->outputString == "empty" ? std::cout : outputFile; // Set stream to cout or outputFile depending
	Hex_out hex_out(stream);

	/** If endVal hasn't been provided, set endVal to EOF */
	if(!CMDArgs->endVal) {
		inputFile.seekg (0, inputFile.end);
		CMDArgs->endVal = inputFile.tellg();
	}

	inputFile.seekg(CMDArgs->beginVal, inputFile.beg);	// Set inputFile pointer to beginVal
	processArgs(CMDArgs, hexInfo);	// Process arguments with filebased beginVal

	/** Overline on first */
	if ((inputFile.tellg() % 16) + (CMDArgs->diff % 16) > 16 || CMDArgs->diff > 16)
		hexInfo->overline = true;
	else
		hexInfo->overline = false;

	/** Raw output or not */
	if(CMDArgs->rawoutput) 
		raw_out(hex_out, inputFile, &stream, CMDArgs, hexInfo);
	else 
		format_out(hex_out, inputFile, CMDArgs, hexInfo);

	/** Closing handles and freeing memory */
	std::cout << '\n' << "Program complete" << '\n';
	inputFile.close();
	outputFile.close();
	delete CMDArgs, hexInfo;
	return 0;
}
