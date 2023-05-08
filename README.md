
# Crocodile Game Engine

*** Fixes
* Fragment shaders not compiling on laptop
* Sound emscripten (code to load wav/obj files)
* fullscreen / resizing

*** Improvements
* move to shared/unique pointers
* reduce complexity in Object class (especially position attributes)

*** Features
* 2d grid system + pathfinding
* Level editor (imgui)
* Custom shaders
  * default shader assigned to object based on render type
  * can be overrided with custom shader class
* Logging
