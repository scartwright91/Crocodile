cmake_minimum_required(VERSION 3.26.3)

# project(LevelEditor)
# add_executable(${PROJECT_NAME}
#     LevelEditor/main.cpp
#     LevelEditor/src/StartScreen.h    
#     LevelEditor/src/StartScreen.cpp    
#     LevelEditor/src/Project.h    
#     LevelEditor/src/Project.cpp    
#     LevelEditor/src/Editor.h
#     LevelEditor/src/Editor.cpp
#     LevelEditor/src/Level.h
#     LevelEditor/src/Level.cpp
#     LevelEditor/src/Entity.h
#     LevelEditor/src/Entity.cpp
#     LevelEditor/src/TextEntity.h
#     LevelEditor/src/TextEntity.cpp
#     LevelEditor/src/ParticleEntity.h
#     LevelEditor/src/ParticleEntity.cpp
# )

project(TestGame)
add_executable(${PROJECT_NAME}
    Examples/collision/collision.cpp
)
# add_executable(${PROJECT_NAME}
#     ${PROJECT_NAME}/main.cpp
#     ${PROJECT_NAME}/src/Balloon.h
#     ${PROJECT_NAME}/src/Balloon.cpp
#     ${PROJECT_NAME}/src/Child.h
#     ${PROJECT_NAME}/src/Child.cpp
#     ${PROJECT_NAME}/src/Camera.h
#     ${PROJECT_NAME}/src/Camera.cpp
#     ${PROJECT_NAME}/src/Dog.h
#     ${PROJECT_NAME}/src/Dog.cpp
#     ${PROJECT_NAME}/src/levels/Level0.h
#     ${PROJECT_NAME}/src/levels/Level0.cpp
#     ${PROJECT_NAME}/src/levels/Level1.h
#     ${PROJECT_NAME}/src/levels/Level1.cpp
#     ${PROJECT_NAME}/src/levels/Level2.h
#     ${PROJECT_NAME}/src/levels/Level2.cpp
#     ${PROJECT_NAME}/src/levels/Level3.h
#     ${PROJECT_NAME}/src/levels/Level3.cpp
#     ${PROJECT_NAME}/src/levels/FinalLevel.h
#     ${PROJECT_NAME}/src/levels/FinalLevel.cpp
#     ${PROJECT_NAME}/src/levels/entities/Monster.h
#     ${PROJECT_NAME}/src/levels/entities/Monster.cpp
#     ${PROJECT_NAME}/src/levels/entities/MiniMonster.cpp
#     ${PROJECT_NAME}/src/levels/entities/MiniMonster.cpp
# )

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

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURE_DIR}/build)
target_link_libraries(${PROJECT_NAME} PRIVATE Crocodile::Crocodile)

add_subdirectory(Engine)
add_subdirectory(Examples/collision)

# Add asset files
file(COPY Engine/assets DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY ${PROJECT_NAME}/res DESTINATION ${CMAKE_BINARY_DIR})

# # build options
# option(BUILD_LEVEL_EDITOR "Build level editor" OFF)
# option(BUILD_COLLISION_EXAMPLE "Build an example of using collisions" OFF)

# if (BUILD_LEVEL_EDITOR)
#     add_subdirectory(LevelEditor)
# endif()
# if (BUILD_COLLISION_EXAMPLE)
#     add_subdirectory(Examples/collision)
# endif()

# ### Dependencies

# # OpenGL
# find_package(OpenGL REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
# target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

# if (NOT EMSCRIPTEN)

#     # GLEW
#     find_package(GLEW REQUIRED)
#     target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

#     # GLFW
#     find_package(glfw3 3.3 REQUIRED)
#     target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

#     # Freetype
#     find_package(Freetype 2.12.1 REQUIRED)
#     target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)

#     # OpenAL
#     find_package(OpenAL CONFIG REQUIRED)
#     target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)

#     # libsndfile
#     find_package(SndFile CONFIG REQUIRED)
#     target_link_libraries(${PROJECT_NAME} PRIVATE SndFile::sndfile)
    
# endif()
