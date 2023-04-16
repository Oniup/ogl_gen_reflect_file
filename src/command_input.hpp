#ifndef __COMMAND_INPUT_HPP__
#define __COMMAND_INPUT_HPP__

#include <iostream>
#include <vector>
#include <tuple>
#include <string>

typedef int CommandArgs;
typedef int ErrorNumber;

enum CommandArgs_ {
    CommandArgs_None                    = 0x000,
    CommandArgs_GenReflectionFile       = 0x001,
    CommandArgs_GenCmakeConfiguration   = 0x002,
    CommandArgs_MakeCmakeLibraryShared  = 0x004,
    CommandArgs_MakeCmakeLibraryStatic  = 0x008,
    CommandArgs_Build                   = 0x010,
    CommandArgs_PrintDebug              = 0x020,
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

bool check_valid_path_in_args(InputArgs* input_args, const char* input_arg_path, size_t arg_offset);
bool get_source_files_from_args(InputArgs* input_args, const std::vector<const char*>& args, size_t& offset);
bool get_reflection_destination_path(InputArgs* input_args, const std::vector<const char*>& args, size_t& offset);

void print_debug(InputArgs* input_args, std::string log);

#endif