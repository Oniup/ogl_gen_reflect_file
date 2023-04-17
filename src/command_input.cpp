/****************************************************************************/
/* command_input.cpp                                                        */
/* This file is part of: ogl_gen_reflection_file                            */
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

#include "command_input.hpp"

InputArgs* read_command_args(InputArgs* input_args, const std::vector<const char*>& args) {
    if (input_args == nullptr) {
        return nullptr;
    }

    for (size_t i = 0; i < args.size(); i++) {
        const char* arg = args[i];

        if (arg[0] == '-') {
            CommandArgs command = CommandArgs_None;

            switch (arg[1]) {
            case 'g':
                command = CommandArgs_GenReflectionFile;
                break;
            case 'c':
                command = CommandArgs_GenCmakeConfiguration;
                if (std::strlen(arg) < 3) {
                    print_debug(input_args, "-c command requires to enter either a 'd' (shared) or 's' (static) to determine the type of library to build to\n");

                    delete input_args;
                    return nullptr;
                }

                if (arg[2] == 'd') {
                    command |= CommandArgs_MakeCmakeLibraryShared;
                }
                else if (arg[2] == 's') {
                    command |= CommandArgs_MakeCmakeLibraryStatic;
                }
                else {
                    print_debug(input_args, "-c command requires to enter either a 'd' (shared) or 's' (static) to determine the type of library to build\n");

                    delete input_args;
                    return nullptr;
                }

                break;
            case 'b':
                command = CommandArgs_Build;
                break;
            case 'd':
                input_args->commands |= CommandArgs_PrintDebug;
                continue;
            default:
                print_debug(input_args, "unknown command\n");

                delete input_args;
                return nullptr;
            }

            input_args->command_offsets.push_back(std::make_tuple(command, i + 1));
            input_args->commands |= command;
        }
    }

    if (input_args->command_offsets.size() == 0) {
        print_debug(input_args, "no commands were passed\n");

        delete input_args;
        return nullptr;
    }

    return input_args;
}

InputArgs* read_file_paths_from_args(InputArgs* input_args, const std::vector<const char*>& args) {
    if (input_args == nullptr) {
        print_debug(input_args, "[ERROR: InputArgs* read_file_paths_from_args(InputArgs*, const std::vector<const char*>&)] --> input_args is nullptr\n");
        return nullptr;
    };

    bool defined_sources{ false };
    bool defined_reflection_source_destination{ false };
    bool defined_cmake_directory{ false };

    for (size_t i = 0; i < input_args->command_offsets.size(); i++) {
        CommandArgs command = std::get<CommandArgs>(input_args->command_offsets[i]);
        size_t offset = std::get<size_t>(input_args->command_offsets[i]);

        if (command & CommandArgs_GenReflectionFile) {
            if (!defined_sources) {
                if (!get_source_files_from_args(input_args, args, offset)) {
                    delete input_args;
                    return nullptr;
                }
                defined_sources = true;
            }

            if (!defined_reflection_source_destination) {
                if (!get_reflection_destination_path(input_args, args, offset)) {
                    delete input_args;
                    return nullptr;
                }
                defined_reflection_source_destination = true;
            }
        }
        else if (command & CommandArgs_GenCmakeConfiguration) {
            if (!defined_reflection_source_destination) {
                if (!get_reflection_destination_path(input_args, args, offset)) {
                    delete input_args;
                    return nullptr;
                }
                defined_reflection_source_destination = true;
            }

            print_debug(input_args, std::string("possible cmake directory path: ") + args[offset] + "\n");
            if (!check_valid_path_in_args(input_args, args[offset], offset)) {
                delete input_args;
                return nullptr;
            }

            print_debug(input_args, "\t* added cmake file directory\n");
            input_args->cmake_file_path = args[offset];
        }
        else {
            print_debug(input_args, "[Error: InputArgs* read_file_paths_from_args(InputArgs*, const std::vector<const char*>&)] --> unknown command\n");

            delete input_args;
            return nullptr;
        }
    }

    return input_args;
}

bool check_valid_path_in_args(InputArgs* input_args, const char* input_arg_path, size_t arg_offset) {
    if (std::strlen(input_arg_path) == 0) {
        if (input_args->commands & CommandArgs_PrintDebug) {
            std::cout << "invalid path at index " << arg_offset << " as size is 0\n";
        }

        return false;
    }

    if (input_arg_path[0] == '-') {
        if (input_args->commands & CommandArgs_PrintDebug) {
            std::cout << "invalid path at index " << arg_offset << ": " << input_arg_path << "\n";
        }

        return false;
    }

    return true;
}

bool get_source_files_from_args(InputArgs* input_args, const std::vector<const char*>& args, size_t& offset) {
    for (size_t j = offset; j < args.size(); j++) {
        print_debug(input_args, std::string("possible source file: ") + args[j] + "\n");

        if (args[j][0] == '-') {
            break;
        }

        if (!check_valid_path_in_args(input_args, args[j], offset)) {
            delete input_args;
            return false;
        }

        std::FILE* file = std::fopen(args[j], "rb");
        if (file == nullptr) {
            break;
        }
        std::fclose(file);

        print_debug(input_args, "\t* adding source file reference\n");
        input_args->read_file_paths.push_back(args[j]);
        offset++;
    }

    return true;
}

bool get_reflection_destination_path(InputArgs* input_args, const std::vector<const char*>& args, size_t& offset) {
    if (!check_valid_path_in_args(input_args, args[offset], offset)) {
        return false;
    }

    print_debug(input_args, "\t* adding reflection generation path\n");
    input_args->reflection_definition_path = args[offset];

    return true;
}

void print_debug(InputArgs* input_args, std::string log) {
    if (input_args->commands & CommandArgs_PrintDebug) {
        std::cout << log;
    }
}