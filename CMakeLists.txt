cmake_minimum_required(VERSION 3.26.3)

project(Project)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

## Compile options

if (EMSCRIPTEN)

    # Set emscripten flags
    set(USE_FLAGS "-lopenal -s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -s FULL_ES3=1 -s USE_FREETYPE=1 -D CROCODILE_EMSCRIPTEN=1 --preload-file res --preload-file assets")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

endif() # Emscripten

# build engine library
add_subdirectory(Engine)

# # build example
# add_subdirectory(Examples/collision)
# file(COPY Engine/assets DESTINATION ${CMAKE_BINARY_DIR}/Examples/collision)

# add_subdirectory(TestGame)
# file(COPY Engine/assets DESTINATION ${CMAKE_BINARY_DIR}/TestGame)

# build level editor
add_subdirectory(LevelEditor)
file(COPY Engine/assets DESTINATION ${CMAKE_BINARY_DIR}/Examples/collision)
