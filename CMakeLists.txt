cmake_minimum_required(VERSION 3.2)
project(ogl_gen_reflection_file)

add_executable(
    ogl_gen_reflection_file
    
    src/command_input.hpp
    src/command_input.cpp
    src/parser.hpp
    src/parser.cpp
    src/gen_reflection_file.hpp

    src/main.cpp
)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

set_property(TARGET ogl_gen_reflection_file PROPERTY CXX_STANDARD 20)