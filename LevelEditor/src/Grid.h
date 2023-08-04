#pragma once

#include <vector>
#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class Grid
{
public:
    unsigned int canvasSizeX;
    unsigned int canvasSizeY;
    unsigned int gridSizeX;
    unsigned int gridSizeY;

    std::vector<s2d::shapes::Line *> lines = {};

    Grid(s2d::Scene *scene, unsigned int canvasSizeX, unsigned int canvasSizeY, unsigned int gridSizeX, unsigned int gridSizeY);
    ~Grid();

    void show();
    void hide();

private:
    s2d::Scene *scene = nullptr;
    bool visible = true;
    void createLines();
};