#pragma once

#include <vector>
#include "Crocodile/s2d/Object.h"

using namespace Crocodile;

class Grid
{
public:
    unsigned int canvasSizeX;
    unsigned int canvasSizeY;
    unsigned int gridSizeX;
    unsigned int gridSizeY;

    std::vector<s2d::Object *> lines = {};

    Grid(unsigned int canvasSizeX, unsigned int canvasSizeY, unsigned int gridSizeX, unsigned int gridSizeY);
    ~Grid();

    void show();
    void hide();

private:
    bool visible = true;
    void createLines();
};