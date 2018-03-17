cmake_minimum_required(VERSION 3.8)


#### Basic project settings
# Project name
project(Caramel-frontend)

# TODO : Setup version numbers
#set(VERSION_MAJOR 0)
#set(VERSION_MINOR 0)
#set(VERSION_PATCH 1)

# Change the required C++ standard
set(CMAKE_CXX_STANDARD 17)

# Make the compiler more strict
#set(CXX_STRICT_WARNINGS "-Werror")
set(CXX_STRICT_WARNINGS "-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Winit-self")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STRICT_WARNINGS}")

# Change default output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/cpp-bin)

# Print the source directory
message("Current source directory: " ${CMAKE_CURRENT_SOURCE_DIR})


#### Library setup : copy shared object + define grammar as a library
set(PROJECT_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)

# Find Antlr runtime library and store the path in ANTLR_RUNTIME
find_library(ANTLR_RUNTIME NAMES libantlr4-runtime.so.4.7.1 PATHS ${PROJECT_LIBRARY_DIR})

# Copy shared library in output directory (we avoid setting LD_LIBRARY_PATH)
configure_file(${PROJECT_LIBRARY_DIR}/libantlr4-runtime.so.4.7.1
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libantlr4-runtime.so.4.7.1 COPYONLY)

# Get every cpp file path into GRAMMAR_BASE
file(GLOB_RECURSE GRAMMAR_BASE ${CMAKE_SOURCE_DIR}/build/cpp-grammar/*.cpp)

# Define Grammar as a library managed and built by CMake
add_library(Grammar ${GRAMMAR_BASE})
target_include_directories(Grammar SYSTEM PRIVATE ${PROJECT_LIBRARY_DIR}/antlr4-runtime-headers)


#### Executable setup: define source files, executable, include directories and library links
set(SOURCE_FILES
        src/main.cpp
        src/listeners/DotExportListener.h
        src/exceptions/NotImplementedException.h
        src/datastructure/Context.cpp
        src/datastructure/Statement.cpp
        src/datastructure/SymbolTable.cpp
        src/datastructure/Declaration.cpp
        src/datastructure/Definition.cpp
        src/datastructure/Expression.cpp
        src/datastructure/ControlBlock.cpp
        src/datastructure/Jump.cpp
        src/datastructure/VariableDeclaration.cpp
        src/datastructure/FunctionDefinition.cpp
        src/datastructure/VariableDefinition.cpp
        src/datastructure/AtomicExpression.cpp
        src/datastructure/FunctionCall.cpp
        src/datastructure/UnaryExpression.cpp
        src/datastructure/BinaryExpression.cpp
        src/datastructure/WhileBlock.cpp
        src/datastructure/IfBlock.cpp
        src/datastructure/ReturnStatement.cpp
        src/datastructure/Constant.cpp
        src/datastructure/LValue.cpp
        src/datastructure/ArrayAccess.cpp
        src/datastructure/Identifier.cpp
        src/datastructure/Operator.cpp
        src/datastructure/UnaryOperator.cpp
        src/datastructure/BinaryOperator.cpp
        )

add_executable(Caramel_frontend ${SOURCE_FILES})

# Check if Chef has been invoked before
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/build/cpp-grammar")
    message("You have to run `./chef.py build --grammar --language cpp --brew` first.")
endif()

# For Grammar library and Caramel, set include directories. Caramel should have Grammar headers included.
target_include_directories(Caramel_frontend PRIVATE ${PROJECT_LIBRARY_DIR}/antlr4-runtime-headers)
target_include_directories(Caramel_frontend PRIVATE ${CMAKE_SOURCE_DIR}/build/cpp-grammar)

# Link Caramel
target_link_libraries(Caramel_frontend ${ANTLR_RUNTIME})
target_link_libraries(Caramel_frontend Grammar)
