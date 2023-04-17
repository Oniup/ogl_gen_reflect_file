#include <iostream>
#include <vector>

#include "command_input.hpp"
#include "parser.hpp"
#include "gen_reflection_file.hpp"

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

    ParserHandler* parser_handler = new ParserHandler{};
    GenReflection* gen_reflection = new GenReflection{};
    parser_handler->gen_reflection_handler = gen_reflection;
    parser_handler = parse_reflection_files(parser_handler, input_args);
    if (parser_handler == nullptr) {
        delete input_args;
        return -3;
    }

    delete input_args;
    delete parser_handler;
    delete gen_reflection;

    return 0;
}