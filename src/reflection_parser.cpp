#include "reflection_parser.hpp"

ReflectionParser* parse_reflection_files(ReflectionParser* reflect_parser, InputArgs* input_args) {
    if (reflect_parser == nullptr) {
        print_debug(input_args, "[ERROR: ReflectParser* parse_reflection_files(ReflectParser*, InputArgs*)] --> ReflectionParser is null\n");
        return nullptr;
    }

    if (input_args->read_file_paths.size() == 0) {
        print_debug(input_args, "[ERROR: ReflectParser* parse_reflection_file(ReflectParser*, InputArgs*)] --> InputArg::read_file_path.size() == 0\n");

        delete reflect_parser;
        return nullptr;
    }

    for (const char* path : input_args->read_file_paths) {
        std::FILE* file = std::fopen(path, "rb");
        if (file == nullptr) {
            print_debug(input_args, std::string("[ERROR: ReflectParser* parse_reflection_file(ReflectParser*, InputArgs*)] --> failed to open file with path: ") + path + "\n");

            delete reflect_parser;
            return nullptr;
        }

        constexpr int str_size = 1024;
        char line[str_size]{};
        std::string* current_namespace = nullptr;
        ReflectObject* current_object = nullptr;
        int number_of_brackets = 0;

        int line_number = 0;
        while (std::fgets(line, str_size, file) != nullptr) {
            std::string keyword{};
            size_t offset = 0;
            size_t line_size = strlen(line);
            size_t keyword_count = 0;
            while (parse_construct_keyword_from_line(keyword, offset, line, line_size)) {
                print_debug(input_args, keyword + "\n");
                if (current_object == nullptr) {
                    // TODO: ...
                }
                else {
                    // TODO: ...
                }
                keyword_count++;
            }

            line_number++;
            print_debug(
                input_args, 
                std::string("******finished line ") + std::to_string(line_number) + ", keyword count: " + 
                std::to_string(keyword_count) + ", keyword size: " + std::to_string(keyword.size()) + "******\n"
            );
        }

        std::fclose(file);
    }

    return reflect_parser;
}

ReflectionParser* create_reflection_definition_file(ReflectionParser* reflect_parser, InputArgs* input_args) {
    if (input_args->reflection_definition_path == nullptr) {
        print_debug(input_args, "[ERROR: ReflectParser* create_reflection_definition_file(ReflectParser*, InputArgs*)] --> InputArg::reflection_definition_path is null\n");

        delete reflect_parser;
        return nullptr;
    }

    const char* file_name = "reflection_definitions.cpp";
    size_t file_name_size = std::strlen(file_name);

    char final_path[1024]{};
    size_t path_size = std::strlen(input_args->reflection_definition_path);
    std::strncpy(final_path, input_args->reflection_definition_path, path_size);

    if (input_args->reflection_definition_path[path_size - 1] != '/') {
        final_path[path_size] = '/';
        path_size++;
    }

    std::strncpy(final_path + path_size, file_name, file_name_size);
    final_path[path_size + file_name_size] = '\0';

    std::FILE* file = std::fopen(final_path, "w+");
    if (file == nullptr) {
        print_debug(
            input_args, 
            std::string("[ERROR: ReflectParser* create_reflection_definition_file(ReflectParser*, InputArgs*)] --> failed to create or open file with path: ") + 
            input_args->reflection_definition_path + "\n"
        );

        delete reflect_parser;
        return nullptr;
    }

    return reflect_parser;
}

bool parse_construct_keyword_from_line(std::string& keyword, size_t& offset, const char* code_line, size_t code_line_size) {
    constexpr std::array<char, 4> breaking_characters {
        ' ', '\n', '\r', '\t'
    };

    size_t keyword_size = 0;

    size_t starting_offset = offset;
    size_t i = starting_offset;
    for (; i < code_line_size; i++) {
        bool found_breaking_character = false;

        for (const char& character : breaking_characters) {
            if (code_line[i] == character) {
                found_breaking_character = true;
                break;
            }
        }

        if (found_breaking_character) {
            if (keyword_size > 0) {
                break;
            }
            else {
                starting_offset++;
            }
        }
        else {
            keyword_size++;
        }
    }

    offset += i;

    if (keyword_size > 0) {
        keyword = std::string(code_line + starting_offset, keyword_size);
        return true;
    }

    return false;
}