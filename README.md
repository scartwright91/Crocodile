
# Crocodile Game Engine

Crocodile is a hobby project to learn C++, OpenGL and game engine development. Whilst there is still a lot of work and improvement to be done, I have used it to create game-jam games. Contributions of any form (albeit with polite communication) are very much welcome.

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

## Build

### Windows

```
$ cmake .
```

### Web (Emscripten)

1. Open emsdk terminal
   * clone https://github.com/emscripten-core/emsdk and run emcmdprompt.bat

2. Create and navigate to `./build_web` directory

```
$ mkdir build_web && cd build_web
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
emrun <PROJECT>/<PROJECT>.html
```

## Roadmap

* Documentation of code and features
* Demonstration of features using example applications
