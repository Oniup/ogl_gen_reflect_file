#ifndef __COMMAND_INPUT_HPP__
#define __COMMAND_INPUT_HPP__

#include <iostream>
#include <vector>
#include <tuple>
#include <string>

typedef int CommandArgs;
enum CommandArgs_ {
    CommandArgs_GenReflectionFile       = 0x001,
    CommandArgs_GenCmakeConfiguration   = 0x002,
    CommandArgs_MakeCmakeLibraryShared  = 0x004,
    CommandArgs_MakeCmakeLibraryStatic  = 0x008,
    CommandArgs_Build                   = 0x010,
};

struct InputArgs {
    CommandArgs commands{};
    std::vector<std::tuple<CommandArgs, size_t>> command_offsets{};
    std::vector<const char*> read_file_paths{};
    const char* reflection_definition_path{};
    const char* cmake_file_path{};
};

InputArgs* read_command_args(InputArgs* input_args, const std::vector<const char*>& args);
InputArgs* read_file_paths_from_args(InputArgs* input_args, const std::vector<const char*>& args);

bool check_valid_path_in_args(const char* input_arg_path, size_t arg_offset, bool should_already_exist);

#endif