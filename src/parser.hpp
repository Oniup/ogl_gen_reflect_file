#ifndef __parser_HPP__
#define __parser_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "command_input.hpp"

enum ParsingProcess {
    ParsingProcess_FindTarget,
    ParsingProcess_Namespace,
    ParsingProcess_Template,
    ParsingProcess_Object,
    ParsingProcess_TemplateWithinObject,
    ParsingProcess_ObjectMember,
    ParsingProcess_ObjectMemberPtrFunc,
    ParsingProcess_NoSerializable,
    ParsingProcess_NoReflection,
};

struct ParserObjectMember {
    std::string name{};
    std::vector<std::string> template_types{};
    bool reflect{ false };

    ParserObjectMember() = default;
    ParserObjectMember(const ParserObjectMember& other);
    ParserObjectMember(ParserObjectMember&& other);
    ParserObjectMember& operator=(const ParserObjectMember& other);
    ParserObjectMember& operator=(ParserObjectMember&& other);
};

struct ParserObject {
    std::string name{};
    std::vector<ParserObject> template_type{};
    std::vector<ParserObjectMember> members{};

    bool is_primitive{ false };
    bool reflect{ false };

    ParserObject() = default;
    ParserObject(const ParserObject& other);
    ParserObject(ParserObject&& other);
    ParserObject& operator=(const ParserObject& other);
    ParserObject& operator=(ParserObject&& other);
};

struct ParserScope {
    std::string namespace_name{};
    ParserObject* object{ nullptr };
};

struct ParserHandler {
    static constexpr size_t line_size = 1024;
    char line[line_size]{};
    std::string current_namespace{};
    ParserObject object{};
    std::vector<ParserScope> scope{};
    struct GenReflection* gen_reflection_handler{ nullptr };

    ParsingProcess current_process{ ParsingProcess_FindTarget };
    bool add_reflect_object{ false };
    std::vector<std::tuple<std::string, ParserObject>> objects{};
};

ParserHandler* parse_reflection_files(ParserHandler* handler, InputArgs* input_args);

bool parse_construct_keyword_from_line(std::string& keyword, size_t& offset, const char* code_line, size_t code_line_size);
void parse_keyword(ParserHandler* handler, const std::string& keyword);
bool remove_object_from_parser(ParserHandler* handler);

#endif