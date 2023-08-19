cmake_minimum_required(VERSION 3.22)

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
    src/World.h
    src/World.cpp
    src/Level.h
    src/Level.cpp
    src/Grid.h
    src/Grid.cpp
    src/EntityManager.h
    src/EntityManager.cpp
    src/entities/Entity.h
    src/entities/Entity.cpp
    src/entities/TextEntity.h
    src/entities/TextEntity.cpp
    src/entities/ParticleEntity.h
    src/entities/ParticleEntity.cpp
    src/entities/ZoneEntity.h
    src/entities/ZoneEntity.cpp
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
