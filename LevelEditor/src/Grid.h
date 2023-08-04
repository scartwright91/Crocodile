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

    bool visible = true;
    void show();
    void hide();
    glm::vec2 getGridPosition(glm::vec2 pos);

private:
    s2d::Scene *scene = nullptr;
    void createLines();
};