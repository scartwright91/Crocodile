
# Crocodile Game Engine

Crocodile is a hobby project to learn C++ and engine development. It is very much in a rough state and I intend to continue developing it. Contributions of any form (albeit with polite communication) are very much welcome!

## Installation

1. Clone repository

```
$ git clone https://github.com/scartwright91/Crocodile.git
```

1. Install dependencies

The engine requires the following packages to be installed:
* GLEW
* GLFW
* Freetype
* OpenAL
* libsndfile

I installed them using [vcpkg](https://vcpkg.io/en/)


## Project structure

### Engine

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.

### LevelEditor

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.

### Examples

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.

## Examples

Check out the [Examples](https://github.com/scartwright91/Crocodile/tree/main/Examples) directory in this repository for sample games and demos built using Crocodile.

## Build

### Web (Emscripten)

Note to build using emscripten, you need to remove the ImGuiFileDialog files.

1. Open emsdk terminal
   * clone https://github.com/emscripten-core/emsdk and run emcmdprompt.bat

2. Navigate to `./build_web` directory (create if missing)

```
$ cd build_web
```

3. Build makefile using emcmake

```
$ emcmake cmake ..
```

4. Build application

```
$ emmake make
```

5. Run application

```
emrun <PROJECT>.html
```
