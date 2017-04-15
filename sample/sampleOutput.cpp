??=pragma clang diagnostic ignored "-Wtrigraphs"
??=include <iostream>

int main() ??<
    if (true ??!??! false) ??<
        // This is a comment|
        std::cout << "|Hello, World!|" << std::endl;
    ??>

    if (true ??!??! false) ??<
        /* This is a multi
         * line comment |o|
         * "|o|"
         */
        std::cout << "The //computer said \"|Hi!|\"" << std::endl;
    ??>
??>
