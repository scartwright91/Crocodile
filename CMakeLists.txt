cmake_minimum_required(VERSION 3.26.3)

set(CMAKE_INSTALL_PREFIX C:/Users/samca/Projects/c++/cmake_packages)

project(LD53_Delivery)

add_executable(${PROJECT_NAME} 
    LD53_Delivery/main.cpp
    LD53_Delivery/src/Player.h
    LD53_Delivery/src/Player.cpp
    LD53_Delivery/src/World.h
    LD53_Delivery/src/World.cpp
    LD53_Delivery/src/PostOffice.h
    LD53_Delivery/src/PostOffice.cpp
    LD53_Delivery/src/deliveries/Seagull.h
    LD53_Delivery/src/deliveries/Seagull.cpp
    LD53_Delivery/src/deliveries/Clownfish.h
    LD53_Delivery/src/deliveries/Clownfish.cpp
    LD53_Delivery/src/Fish.h
    LD53_Delivery/src/Fish.cpp
    LD53_Delivery/src/fish/Shark.h
    LD53_Delivery/src/fish/Shark.cpp
    LD53_Delivery/src/fish/Jellyfish.h
    LD53_Delivery/src/fish/Jellyfish.cpp
    LD53_Delivery/src/Cave.h
    LD53_Delivery/src/Cave.cpp
    LD53_Delivery/src/deliveries/Anglerfish.h
    LD53_Delivery/src/deliveries/Anglerfish.cpp
)

## Compile options

if (EMSCRIPTEN)

    # Set emscripten flags
    set(USE_FLAGS "-lopenal -s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -s FULL_ES3=1 -s USE_FREETYPE=1 -D CROCODILE_EMSCRIPTEN=1 --preload-file res --preload-file assets")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

endif() # Emscripten

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURE_DIR}/build)
target_link_libraries(${PROJECT_NAME} PRIVATE Crocodile::Crocodile)

add_subdirectory(Engine)

# Add asset files
file(COPY Engine/assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${PROJECT_NAME}/res DESTINATION ${CMAKE_BINARY_DIR})

### Dependencies

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
