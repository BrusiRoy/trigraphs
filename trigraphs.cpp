#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/// Show the correct usage
static void showUsage() {
    std::cout << "trigraphs: invalid arguments" << std::endl;
    std::cout << "usage: trigraphs SOURCE DEST" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        showUsage();
        return -1;
    }

    const std::string inputFileName = argv[1];
    const std::string outputFileName = argv[2];

    std::cout << inputFileName << std::endl;
    std::cout << outputFileName << std::endl;

    // Reading
    std::ifstream inputFile(inputFileName.c_str());
    // TODO: Improve reading speed
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    inputFile.close();

    // Writing
    std::ofstream outputFile(outputFileName.c_str());
    outputFile << buffer.str();
    outputFile.close();

    return 0;
}
