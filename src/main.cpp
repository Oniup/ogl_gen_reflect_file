#include <iostream>
#include <vector>

#include "command_input.hpp"

int main(int argc, char** argv) {
    std::vector<const char*> args(argc);
    for (size_t i = 0; i < args.size(); i++) {
        args[i] = argv[i];
    }

    InputArgs* input_args = new InputArgs{};
    input_args = read_command_args(input_args, args);
    if (input_args == nullptr) {
        return -1;
    }

    if (input_args->commands & CommandArgs_GenReflectionFile) {
        std::cout << "read cpp files and create reflection file\n";
    }

    if (input_args->commands & CommandArgs_GenCmakeConfiguration) {
        std::cout << "create cmake configuration file and create the library as ";
        if (input_args->commands & CommandArgs_MakeCmakeLibraryShared) {
            std::cout << "shared\n";
        }
        else if (input_args->commands & CommandArgs_MakeCmakeLibraryStatic) {
            std::cout << "static\n";
        }
    }

    delete input_args;
    return 0;
}