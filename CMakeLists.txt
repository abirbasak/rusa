cmake_minimum_required(VERSION 3.5)
project(rusa VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)

add_library(rusa INTERFACE)
group_sources(include)
group_sources(source)
group_sources(test)
group_sources(examples)
add_custom_target(rusa-code SOURCES ${HEADERS} ${SOURCES})

target_include_directories(rusa INTERFACE include)
target_compile_features(rusa INTERFACE cxx_std_17)

option(RUSA_BUILD_EXAMPLES "Build examples" ON)
if(RUSA_BUILD_EXAMPLES)
    project(rusa-simple VERSION 1.0.0 LANGUAGES CXX)
    add_executable(rusa-simple examples/simple.cpp)
   
    target_link_libraries(rusa-simple rusa)
endif()

project(rusa-test VERSION 1.0.0 LANGUAGES CXX)
file(GLOB_RECURSE TEST-SOURCES test/*.cpp)
add_executable(rusa-test ${TEST-SOURCES})

target_link_libraries(rusa-test rusa)

