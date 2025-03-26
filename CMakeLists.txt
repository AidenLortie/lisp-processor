cmake_minimum_required(VERSION 3.27)
project(LISP_LITE C)

set(CMAKE_C_STANDARD 11)

add_executable(LISP_LITE library.c
        main.c
        lexer.h
        lexer.c)
