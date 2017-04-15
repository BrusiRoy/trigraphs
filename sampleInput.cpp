#include <iostream>

int main() {
    if (true || false) {
        // This is a comment|
        std::cout << "|Hello, World!|" << std::endl;
    }

    if (true && false) {
        // Do this
        std::cout << "The computer said \"|Hi!|\"" << std::endl;
    }
}
