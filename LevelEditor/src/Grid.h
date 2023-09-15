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

    Grid(
        s2d::Scene *scene,
        glm::vec2 position,
        unsigned int canvasSizeX,
        unsigned int canvasSizeY,
        unsigned int gridSizeX,
        unsigned int gridSizeY);
    ~Grid();

    glm::vec2 position = glm::vec2(0.f);
    bool visible = false;
    void show();
    void hide();
    glm::vec2 getGridPosition(glm::vec2 pos);

private:
    s2d::Scene *scene = nullptr;
    void createLines();
};