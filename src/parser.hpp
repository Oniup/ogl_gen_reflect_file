/****************************************************************************/
/* parser.hpp                                                               */
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

#ifndef __parser_HPP__
#define __parser_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "command_input.hpp"

enum ParsingProcess {
    ParsingProcess_FindTarget,
    ParsingProcess_NamespaceName,
    ParsingProcess_Template,
    ParsingProcess_TemplateParameter,
    ParsingProcess_TemplateWithinObject,
    ParsingProcess_ObjectName,
    ParsingProcess_ObjectContent,
    ParsingProcess_ObjectFunction,
    ParsingProcess_NoSerializable,
    ParsingProcess_NoReflection,
    ParsingProcess_Function,
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
    std::vector<ParserScope> scopes{};
    ParserScope* previous_scope{ nullptr };
};

struct ParserHandler {
    static constexpr size_t line_size = 1024;
    char line[line_size]{};
    std::string current_namespace{};
    ParserObject object{};
    std::vector<ParserScope> scopes{};
    ParserScope* current_scope{ nullptr };
    bool next_keyword_open_scope{ false };
    struct GenReflection* gen_reflection_handler{ nullptr };

    ParsingProcess current_process{ ParsingProcess_FindTarget };
    std::vector<std::tuple<std::string, ParserObject>> objects{};
};

ParserHandler* parse_reflection_files(ParserHandler* handler, InputArgs* input_args);

bool parse_construct_keyword_from_line(std::string& keyword, size_t& offset, const char* code_line, size_t code_line_size);
bool parse_keyword(ParserHandler* handler, const std::string& keyword);
bool remove_object_from_parser(ParserHandler* handler);

void parser_push_scope(ParserHandler* handler, ParserScope&& new_scope);
bool parser_end_scope(ParserHandler* handler);

#endif