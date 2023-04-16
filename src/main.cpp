#include <iostream>
#include <vector>

#include "command_input.hpp"
#include "reflection_parser.hpp"

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

    input_args = read_file_paths_from_args(input_args, args);
    if (input_args == nullptr) {
        return -2;
    }

    ReflectionParser* reflection_parser = new ReflectionParser{};
    reflection_parser = parse_reflection_files(reflection_parser, input_args);
    if (reflection_parser == nullptr) {
        delete input_args;
        return -3;
    }

    delete input_args;
    delete reflection_parser;

    return 0;
}