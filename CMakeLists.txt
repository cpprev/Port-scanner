# SETUP
cmake_minimum_required(VERSION 3.12)
project(portscanner)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

include_directories(src/)

#List of source files
set(SRC_FILES
        # Scanner
        src/scanner/scanner.cc

        # Parsing
        src/parsing/input_parsing.cc

        # Utils
        src/utils/utils.cc
        )

# Compilation flags
set(CANDY_FLAGS "-Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${} -std=c++17 -fsanitize=address -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

# Executables
add_executable(scanner src/main.cc ${SRC_FILES})