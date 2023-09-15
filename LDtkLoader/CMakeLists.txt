cmake_minimum_required(VERSION 3.22)

project(LDtkLoader VERSION 1.2.2)

# get source files
file(GLOB_RECURSE Src src/*.cpp)
file(GLOB_RECURSE Inc include/LDtkLoader/*.hpp)

# create the static lib target
add_library(LDtkLoader STATIC ${Src} ${Inc})
add_library(LDtkLoader::LDtkLoader ALIAS LDtkLoader)

# set include directory for build and install
target_include_directories(LDtkLoader PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(LDtkLoader PUBLIC include/LDtkLoader)
target_include_directories(LDtkLoader PUBLIC include/LDtkLoader/containers)
target_include_directories(LDtkLoader PUBLIC include/LDtkLoader/defs)
target_include_directories(LDtkLoader PUBLIC include/LDtkLoader/thirdparty)
