cmake_minimum_required(VERSION 3.5)
project(rusa VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)
add_library(rusa INTERFACE)
target_sources(rusa INTERFACE ${HEADERS})

target_include_directories(rusa INTERFACE include)
target_compile_features(rusa INTERFACE cxx_std_17)

option(RUSA_BUILD_EXAMPLES "Build examples" ON)
if(RUSA_BUILD_EXAMPLES)
    project(rusa-simple VERSION 1.0.0 LANGUAGES CXX)
    add_executable(rusa-simple examples/simple.cpp)
    target_link_libraries(rusa-simple rusa)
endif()
