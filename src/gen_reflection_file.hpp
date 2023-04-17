/****************************************************************************/
/* gen_reflection_file.hpp                                                  */
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

#ifndef __GEN_REFLECTION_FILE_HPP__
#define __GEN_REFLECTION_FILE_HPP__

#include "parser.hpp"

struct GenReflection {
    std::vector<ParserObject> objects{};
};

#endif