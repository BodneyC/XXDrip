#include <string>
#include <tclap/CmdLine.h>
#include "hex_out.h"

struct inputinfo {
	int beginVal, endVal;
	std::string inputString, outputString;
	bool writetofile, rawoutput;
} CMDArgs;

void processArgs (int argn, char const *args[]) {
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
int main(int argc, char const *argv[]) {
	processArgs(argc, argv);

	std::cout << CMDArgs.beginVal << CMDArgs.outputString << CMDArgs.inputString << '\n';

	return 0;
}
