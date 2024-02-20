cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")


set(MACOSX_RPATH ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_definitions(DEBUG)


find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)

include_directories(/usr/local/include)
include_directories(${SFML_INCLUDE_DIRS})

project(Main)



add_subdirectory("${PROJECT_SOURCE_DIR}/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

add_executable(Main ${SOURCES} src/main.cpp)


target_link_libraries(Main sfml-graphics sfml-window sfml-system)

target_include_directories(Main PUBLIC ${HEADERS})
