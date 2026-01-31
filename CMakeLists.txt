cmake_minimum_required(VERSION 3.31)

project(compiler)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(PkgConfig REQUIRED)

SET(CMAKE_CXX_STANDARD 23)
SET(FLAGS -ggdb3)

include(FetchContent)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)

FetchContent_MakeAvailable(argparse)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})


BISON_TARGET(Parser src/parser/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.hpp)
FLEX_TARGET(Scanner src/lexer/lexer.lex  ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

BISON_TARGET(VMParser src/integrated-vm/vm-parser.y 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/integrated-vm/vm-parser.cpp 
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/integrated-vm/vm-parser.hpp
    COMPILE_FLAGS "--name-prefix=vm_yy")
FLEX_TARGET(VMScanner src/integrated-vm/vm-lexer.lex  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/integrated-vm/vm-lexer.cpp
    COMPILE_FLAGS "--prefix=vm_yy")
ADD_FLEX_BISON_DEPENDENCY(VMScanner VMParser)


file(GLOB_RECURSE CXX_SOURCES 
    CONFIGURE_DEPENDS
    "src/*.cpp"
)
list(APPEND CXX_SOURCES 
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    ${BISON_VMParser_OUTPUTS}
    ${FLEX_VMScanner_OUTPUTS}
)


add_executable(kompilator ${CXX_SOURCES})
target_compile_options(kompilator PRIVATE ${FLAGS})
target_link_libraries(kompilator 
    PRIVATE stdc++exp
    PRIVATE ${CLN_LIBRARIES}
)