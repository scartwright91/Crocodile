cmake_minimum_required(VERSION 3.5)

project(Crocodile VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

# Engine files
file(GLOB_RECURSE Src src/*.cpp)
file(GLOB_RECURSE Inc src/*.h)

add_library(Crocodile STATIC ${Src} ${Inc})
add_library(Crocodile::Crocodile ALIAS Crocodile)

target_include_directories(Crocodile PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(Crocodile PUBLIC src)
target_include_directories(Crocodile PUBLIC src/utils)
target_include_directories(Crocodile PUBLIC src/utils/imgui)
target_include_directories(Crocodile PUBLIC src/utils/imgui/backends)

### Dependencies

## Cross-platform dependencies

if (NOT EMSCRIPTEN)

    # OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

    # GLEW
    find_package(GLEW REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

    # GLFW
    find_package(glfw3 3.3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

    # Freetype
    find_package(Freetype 2.12.1 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)

    # OpenAL
    find_package(OpenAL CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)

    # libsndfile
    find_package(SndFile CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE SndFile::sndfile)

    # lua
    find_package(Lua REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
    
endif()
