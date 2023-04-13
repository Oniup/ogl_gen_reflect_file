#include "command_input.hpp"

InputArgs* read_command_args(InputArgs* input_args, const std::vector<const char*>& args) {
    if (input_args == nullptr) {
        std::cout << "[ERROR: InputArgs* read_command_args(InputArgs*, const std::vector<const char*>&)] --> input_args is nullptr\n";
        return nullptr;
    }

    for (size_t i = 0; i < args.size(); i++) {
        const char* arg = args[i];

        if (arg[0] == '-') {
            input_args->command_offsets.resize(input_args->command_offsets.size() + 1);
            std::get<size_t>(input_args->command_offsets.back()) = i;
            CommandArgs& command = std::get<CommandArgs>(input_args->command_offsets.back());

            switch (arg[1]) {
            case 'g':
                command = CommandArgs_GenReflectionFile;
                break;
            case 'c':
                command = CommandArgs_GenCmakeConfiguration;
                if (std::strlen(arg) < 3) {
                    std::cout << "-c command requires to enter either a 'd' (shared) or 's' (static) to determine the type of library to build to\n";

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
                    std::cout << "-c command requires to enter either a 'd' (shared) or 's' (static) to determine the type of library to build\n";

                    delete input_args;
                    return nullptr;
                }

                break;
            case 'b':
                command = CommandArgs_Build;
                break;
            default:
                std::cout << "unknown command '" << arg << "'\n";

                delete input_args;
                return nullptr;
            }

            input_args->commands |= command;
        }
    }

    if (input_args->command_offsets.size() == 0) {
        std::cout << "no commands were passed\n";

        delete input_args;
        return nullptr;
    }

    return input_args;
}

InputArgs* read_file_paths_from_args(InputArgs* input_args, const std::vector<const char*>& args) {
    if (input_args == nullptr) {
        std::cout << "[ERROR: InputArgs* read_file_paths_from_args(InputArgs*, const std::vector<const char*>&)] --> input_args is nullptr\n";
        return nullptr;
    }

    bool defined_sources{ false };
    bool defined_reflection_source_destination{ false };
    bool defined_cmake_directory{ false };

    for (size_t i = 0; i < input_args->command_offsets.size(); i++) {
        CommandArgs command = std::get<CommandArgs>(input_args->command_offsets[i]);
        size_t offset = std::get<size_t>(input_args->command_offsets[i]);

        if (command & CommandArgs_GenReflectionFile) {
            if (!defined_sources) {
                for (size_t j = i; j < input_args->command_offsets.size(); j++) {
                    if (args[j][0] == '-') {
                        break;
                    }

                    if (!check_valid_path_in_args(args[j], offset, true)) {
                        delete input_args;
                        return nullptr;
                    }

                    input_args->read_file_paths.push_back(args[j]);
                    offset++;
                }

                defined_sources = true;
            }

            if (!defined_reflection_source_destination) {
                if (!check_valid_path_in_args(args[offset], offset, false)) {
                    delete input_args;
                    return nullptr;
                }

                input_args->reflection_definition_path = args[offset];
            }
        }
        else if (command & CommandArgs_GenCmakeConfiguration) {
            if (!defined_reflection_source_destination) {
                if (!check_valid_path_in_args(args[offset], offset, false)) {
                    delete input_args;
                    return nullptr;
                }

                input_args->reflection_definition_path = args[offset];
                offset++;
            }

            if (!check_valid_path_in_args(args[offset], offset, false)) {
                delete input_args;
                return nullptr;
            }

            input_args->cmake_file_path = args[offset];
        }
        else {
            std::cout << "[Error: InputArgs* read_file_paths_from_args(InputArgs*, const std::vector<const char*>&)] --> unknown command\n";

            delete input_args;
            return nullptr;
        }
    }

    return input_args;
}

bool check_valid_path_in_args(const char* input_arg_path, size_t arg_offset, bool should_already_exist) {
    if (std::strlen(input_arg_path) == 0) {
        std::cout << "invalid path at index " << arg_offset << " as size is 0\n";

        return false;
    }

    if (input_arg_path[0] == '-') {
        std::cout << "invalid path at index " << arg_offset << ": " << input_arg_path << "\n";

        return false;
    }

    if (should_already_exist) {
        std::FILE* file = fopen(input_arg_path, "rb");
        if (file != nullptr) {
            std::fclose(file);
        }
        else {
            std::cout << "invalid path at index " << arg_offset << ": " << input_arg_path << " as the file doesn't exist\n";

            return false;
        }
    }

    return true;
}