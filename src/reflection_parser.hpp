#ifndef __REFLECTION_PARSER_HPP__
#define __REFLECTION_PARSER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "command_input.hpp"

struct ReflectObjectMember {
    std::string name{};
};

struct ReflectObject {
    std::string name{};
    std::vector<ReflectObject> template_type{};
    std::vector<ReflectObjectMember> members{};

    bool is_primitive{ false };
};

struct ReflectionParser {
    std::vector<std::tuple<std::string, ReflectObject>> objects{};
};

ReflectionParser* parse_reflection_files(ReflectionParser* reflect_parser, InputArgs* input_args);
ReflectionParser* create_reflection_definition_file(ReflectionParser* reflect_parser, InputArgs* input_args);
bool parse_construct_keyword_from_line(std::string& keyword, size_t& offset, const char* code_line, size_t code_line_size);

#endif