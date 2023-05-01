cmake_minimum_required(VERSION 3.26.3)
project(Crocodile VERSION 0.0.1)

# Engine files
file(GLOB_RECURSE Src src/*.cpp)
file(GLOB_RECURSE Inc src/*.h)

add_library(Crocodile STATIC ${Src} ${Inc})
add_library(Crocodile::Crocodile ALIAS Crocodile)

target_include_directories(Crocodile PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(Crocodile PUBLIC src)
target_include_directories(Crocodile PUBLIC src/utils)

### Dependencies

## Cross-platform dependencies

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

if (NOT EMSCRIPTEN)
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

endif()
