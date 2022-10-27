#ifndef IOLIB_H
#define IOLIB_H

#include <iostream>
#include <fstream>
#include <assert.h>

// Pre:  Takes the current file argument passed from the console and two empty 
//       strings denoted as the input file and output file
// Post: Fills the string with the appropriate file name and returns void
void SetIOFiles(std::string arg, std::string& inputFile, std::string& outputFile) {
        std::string fileName = arg;
        int pos_of_last_slash = -1;
        for (int i = 0; i < fileName.size(); i++) {
            if (fileName[i] == '/') pos_of_last_slash = i;
        }
        // Set input file
        inputFile = fileName;

        // Set proper output file
        if (pos_of_last_slash != -1) {
            outputFile = fileName.substr(0, pos_of_last_slash + 1) + "OUTPUT_" + fileName.substr(pos_of_last_slash + 1);
        }else {
            outputFile = "OUTPUT_" + fileName;
        }
}

// Pre:  Takes the status of fin.fail() & the file name in question as parameters
// Post: Returns void, if fin failed to open the file in question, it will display
//       an error message and quit the program
void AssertInputFileOpenSuccess(bool finStatus, std::string fileName) {

    // Print error message if load failed
    if (finStatus) std::cout << "ERROR:  Could not open file " << fileName << ".\nFile does not exist.\n";

    // Quit the program if the fin open failed
    assert(!finStatus);
}

#endif