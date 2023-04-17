#include "parser.hpp"
#include "gen_reflection_file.hpp"

ParserObjectMember::ParserObjectMember(const ParserObjectMember& other) : name(other.name), template_types(other.template_types) { }
ParserObjectMember::ParserObjectMember(ParserObjectMember&& other) : name(std::move(other.name)), template_types(std::move(other.template_types)) { }

ParserObjectMember& ParserObjectMember::operator=(const ParserObjectMember& other) {
    name = other.name;
    template_types = other.template_types;

    return *this;
}

ParserObjectMember& ParserObjectMember::operator=(ParserObjectMember&& other) {
    name = std::move(other.name);
    template_types = std::move(other.template_types);

    return *this;
}

ParserObject::ParserObject(const ParserObject& other) : name(other.name), template_type(other.template_type), members(other.members) { }
ParserObject::ParserObject(ParserObject&& other) : name(std::move(other.name)), template_type(std::move(other.template_type)), members(std::move(other.members)) { }

ParserObject& ParserObject::operator=(const ParserObject& other) {
    name = other.name;
    template_type = other.template_type;
    members = other.members;

    return *this;
}

ParserObject& ParserObject::operator=(ParserObject&& other) {
    name = std::move(other.name);
    template_type = std::move(other.template_type);
    members = std::move(other.members);

    return *this;
}

ParserHandler* parse_reflection_files(ParserHandler* handler, InputArgs* input_args) {
    if (handler == nullptr) {
        print_debug(input_args, "[ERROR: ReflectParser* parse_reflection_files(ReflectParser*, InputArgs*)] --> ParserHandler is null\n");
        return nullptr;
    }

    if (input_args->read_file_paths.size() == 0) {
        print_debug(input_args, "[ERROR: ReflectParser* parse_reflection_file(ReflectParser*, InputArgs*)] --> InputArg::read_file_path.size() == 0\n");

        delete handler;
        return nullptr;
    }

    for (const char* path : input_args->read_file_paths) {
        std::FILE* file = std::fopen(path, "rb");
        if (file == nullptr) {
            print_debug(input_args, std::string("[ERROR: ReflectParser* parse_reflection_file(ReflectParser*, InputArgs*)] --> failed to open file with path: ") + path + "\n");

            delete handler;
            return nullptr;
        }

        int line_number = 0;
        while (std::fgets(handler->line, handler->line_size, file) != nullptr) {
            std::string keyword{};
            size_t offset = 0;
            size_t line_size = strlen(handler->line);
            size_t keyword_count = 0;
            while (parse_construct_keyword_from_line(keyword, offset, handler->line, line_size)) {
                print_debug(input_args, keyword + "\n");
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

    return handler;
}

bool parse_construct_keyword_from_line(std::string& keyword, size_t& offset, const char* code_line, size_t code_line_size) {
    constexpr size_t breaking_characters_include_in_keyword_index = 4;
    constexpr std::array<char, 7> breaking_characters {
        ' ', '\n', '\r', '\t', ',', '<', '>'
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
                for (size_t i = breaking_characters_include_in_keyword_index; i < breaking_characters.size(); i++) {
                    if (code_line[i] == breaking_characters[i]) {
                        keyword_size++;
                        break;
                    }
                }
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

void parse_keyword(ParserHandler* handler, const std::string& keyword) {
    switch (handler->current_process) {
    case ParsingProcess_FindTarget:
        if (keyword == "namespace") {
            handler->current_process = ParsingProcess_Namespace;
        }
        else if (keyword == "struct" || keyword == "class") {
            handler->current_process = ParsingProcess_Object;
        }
        else if (keyword == "template" || keyword == "template<") {
            handler->current_process = ParsingProcess_Template;
        }

        return;
    case ParsingProcess_Namespace:
        
        break;
    }

    if (keyword == "{") {
    }

}

bool remove_object_from_parser(ParserHandler* handler) {
    if (handler->gen_reflection_handler == nullptr) {
        return false;
    }

    return true;
}