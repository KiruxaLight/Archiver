#include "archiver.h"
#include "writing.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    std::vector<std::string> arguments;
    for (char** arg = argv; *arg; ++arg) {
        arguments.push_back(*arg);
    }
    if (argc >= 4 && arguments[1] == "-c") {
        // Compress!
        std::cout << "Compressing\n";
        Archiver archiver;
        std::ofstream out(arguments[2], std::ios::binary);
        Writer writer(out);
        for (size_t index = 3; index < arguments.size(); ++index) {
            bool end_archive = (index + 1 == arguments.size());
            archiver.Compress(arguments[index], writer, end_archive);
        }
        writer.Clear();
    } else if (argc == 3 && arguments[1] == "-d") {
        // Decompress
        std::cout << "Decompressing\n";
        Archiver archiver;
        archiver.Decompress(arguments[2]);
    } else {
        // Help
        Archiver archiver;
        archiver.OutHelp();
    }
}
