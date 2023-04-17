/****************************************************************************/
/* command_input.hpp                                                        */
/* This file is part of ogl_gen_reflection_file                             */
/****************************************************************************/
/* Copyright 2023 Ewan Robson                                               */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the "License");          */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an "AS IS" BASIS,        */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/****************************************************************************/

#ifndef __COMMAND_INPUT_HPP__
#define __COMMAND_INPUT_HPP__

#include <iostream>
#include <vector>
#include <tuple>
#include <string>

typedef int CommandArgs;

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