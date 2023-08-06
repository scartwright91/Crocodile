cmake_minimum_required(VERSION 3.26.3)

project(LevelEditor)
add_executable(${PROJECT_NAME}
    main.cpp
    src/StartScreen.h    
    src/StartScreen.cpp    
    src/Canvas.h    
    src/Canvas.cpp    
    src/Project.h    
    src/Project.cpp    
    src/Editor.h
    src/Editor.cpp
    src/Level.h
    src/Level.cpp
    src/Entity.h
    src/Entity.cpp
    src/TextEntity.h
    src/TextEntity.cpp
    src/ParticleEntity.h
    src/ParticleEntity.cpp
    src/Grid.h
    src/Grid.cpp
)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURE_DIR}/build)
target_link_libraries(${PROJECT_NAME} PRIVATE Crocodile::Crocodile)

## Dependencies

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
