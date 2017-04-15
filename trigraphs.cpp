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
    // Fill the trigraphs equivalent map
    trigraphEquivalent_.emplace('#', "\x3f\x3f\x3d");   // ??=
    trigraphEquivalent_.emplace('\\', "\x3f\x3f\x2f");  // ??/
    trigraphEquivalent_.emplace('^', "\x3f\x3f\x27");   // ??'
    trigraphEquivalent_.emplace('[', "\x3f\x3f\x28");   // ??(
    trigraphEquivalent_.emplace(']', "\x3f\x3f\x29");   // ??)
    trigraphEquivalent_.emplace('|', "\x3f\x3f\x21");   // ??!
    trigraphEquivalent_.emplace('{', "\x3f\x3f\x3c");   // ??<
    trigraphEquivalent_.emplace('}', "\x3f\x3f\x3e");   // ??>
    trigraphEquivalent_.emplace('~', "\x3f\x3f\x2d");   // ??-

    outputFile_ << "#pragma clang diagnostic ignored \"-Wtrigraphs\"\n";
}

Parser::~Parser() {
    inputFile_.close();
    outputFile_.close();
}

void Parser::parse() noexcept {
    std::string line;
    // Parse one line at a time
    while (getline(inputFile_, line)) {
        char previousChar = 0;

        // TODO: Fix comments inside a literal or literal inside comments
        for (size_t i = 0; i < line.size(); ++i) {
            // Single line comment
            if (line[i] == '/' && line[i] == previousChar) {
                // currentState_ = State::COMMENT;
                insertRestOfLine(line, i);
                break;  // We are in a single line comment
            }
            // Multi-line comment
            else if (previousChar == '/' && line[i] == '*') {
                toggle(State::COMMENT);
            } else if (previousChar == '*' && line[i] == '/') {
                toggle(State::COMMENT);
            }
            // String literal
            else if (line[i] == '\"' && previousChar != '\\') {
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
    outputFile_ << line.substr(startIndex, line.size());
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
