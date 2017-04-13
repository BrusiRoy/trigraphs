#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

enum class STATE { NORMAL, COMMENT, LITERAL };

class Parser {
   public:
    Parser(const std::string& inputFileName, const std::string& outputFileName);
    ~Parser();

    void parse() noexcept;

    void toggle(const STATE state) noexcept;

   private:
    /// Buffer
    std::stringstream buffer_;
    /// Current state
    STATE currentState_ = STATE::NORMAL;
    /// Input file
    std::ifstream inputFile_;
    /// Ouput file
    std::ofstream outputFile_;
};

Parser::Parser(const std::string& inputFileName,
               const std::string& outputFileName)
    : inputFile_(inputFileName), outputFile_(outputFileName) {
    // TODO: Improve the reading speed
    buffer_ << inputFile_.rdbuf();
}

Parser::~Parser() {
    inputFile_.close();
    outputFile_.close();
}

void Parser::parse() noexcept {
    // Do the parsing
    char currentChar;

    while (buffer_ >> currentChar) {
        if (currentChar == '\"') {
            toggle(STATE::COMMENT);
            continue;
        }

        // If we are in a `NORMAL` state, we output the character
        if (currentState_ == STATE::NORMAL) {
            outputFile_ << currentChar;
        }
    }
}

void Parser::toggle(STATE state) noexcept {
    if (currentState_ == STATE::NORMAL) {
        currentState_ = state;
    } else if (currentState_ == state) {
        currentState_ = STATE::NORMAL;
    }
}

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

    Parser parser(inputFileName, outputFileName);
    parser.parse();

    return 0;
}
