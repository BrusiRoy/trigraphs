#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

enum class State { NORMAL, COMMENT, LITERAL };

class Parser {
   public:
    Parser(const std::string& inputFileName, const std::string& outputFileName);
    ~Parser();

    void parse() noexcept;

   private:
    void insert(char character) noexcept;
    void insertRestOfLine(const std::string& line, size_t startIndex) noexcept;
    void toggle(const State state) noexcept;

    /// Buffer
    std::stringstream buffer_;
    /// Current state
    State currentState_ = State::NORMAL;
    /// Input file
    std::ifstream inputFile_;
    /// Ouput file
    std::ofstream outputFile_;
    /// Trigraphs equivalents
    std::unordered_map<char, std::string> trigraphEquivalent_;
};

Parser::Parser(const std::string& inputFileName,
               const std::string& outputFileName)
    : inputFile_(inputFileName), outputFile_(outputFileName) {
    // TODO: Improve the reading speed
    // buffer_ << inputFile_.rdbuf();
    // Use hex representation
    std::string trigraph = "?";
    trigraph.append("?!");
    trigraphEquivalent_.emplace('|', trigraph);
}

Parser::~Parser() {
    inputFile_.close();
    outputFile_.close();
}

void Parser::parse() noexcept {
    std::string line;
    while (getline(inputFile_, line)) {
        char previousChar = 0;

        for (size_t i = 0; i < line.size(); ++i) {
            // Single line comment
            if (line[i] == '/' && line[i] == previousChar) {
                currentState_ = State::COMMENT;
                insertRestOfLine(line, i);
                break;  // We are in a single line comment
            } else if (line[i] == '\"' && previousChar != '\\') {
                toggle(State::LITERAL);
            }
            insert(line[i]);
            previousChar = line[i];
        }
        insert('\n');
    }
}

void Parser::insert(char character) noexcept {
    // If in a comment or a string literal, don't change symbols
    if (currentState_ == State::COMMENT || currentState_ == State::LITERAL ||
        character == '\n') {
        outputFile_ << character;
    } else {
        const auto it = trigraphEquivalent_.find(character);
        if (it != trigraphEquivalent_.end()) {
            outputFile_ << it->second;
        } else {
            outputFile_ << character;
        }
    }
}

void Parser::insertRestOfLine(const std::string& line,
                              size_t startIndex) noexcept {
    for (; startIndex < line.size(); ++startIndex) {
        insert(line[startIndex]);
    }
}

void Parser::toggle(State state) noexcept {
    if (currentState_ == State::NORMAL) {
        currentState_ = state;
    } else if (currentState_ == state) {
        currentState_ = State::NORMAL;
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
