cmake_minimum_required(VERSION 3.10)

# Project Name
project(pTable VERSION 2.0.0 DESCRIPTION "Periodic Table")

add_library(pTable SHARED
  lib/src/table.cpp
)

set_target_properties(pTable PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION}
  PUBLIC_HEADER lib/include/pTable.h
  LIBRARY_OUTPUT_DIRECTORY lib/bin
)
target_include_directories(pTable PRIVATE lib/include)

# Build executables
add_executable(pTerminal src/main.cpp)
target_link_libraries(pTerminal pTable)
target_include_directories(pTerminal PRIVATE lib/include)
set_target_properties(pTerminal PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
