#include "Grid.h"

Grid::Grid(
    s2d::Scene *scene,
    glm::vec2 position,
    unsigned int canvasSizeX,
    unsigned int canvasSizeY,
    unsigned int gridSizeX,
    unsigned int gridSizeY) : scene(scene), position(position), canvasSizeX(canvasSizeX), canvasSizeY(canvasSizeY), gridSizeX(gridSizeX), gridSizeY(gridSizeY)
{
    createLines();
}

Grid::~Grid()
{
    for (s2d::shapes::Line *line : lines)
    {
        scene->removeChild(line, "grid");
        delete line;
    }
}

glm::vec2 Grid::getGridPosition(glm::vec2 pos)
{
    return glm::vec2((int)pos.x / gridSizeX, (int)pos.y / gridSizeY);
}

void Grid::show()
{
    visible = true;
    for (s2d::shapes::Line *line : lines)
        scene->addChild(line, "grid");
}

void Grid::hide()
{
    visible = false;
    for (s2d::shapes::Line *line : lines)
        scene->removeChild(line, "grid");
}

void Grid::createLines()
{
    // create horizontal lines
    for (unsigned int i = 0; i < canvasSizeY / gridSizeY + 1; i++)
    {
        glm::vec2 a = glm::vec2(0.f, i * gridSizeY);
        glm::vec2 b = glm::vec2(canvasSizeX, i * gridSizeY);
        s2d::shapes::Line *line = new s2d::shapes::Line(a + position, b + position);
        lines.push_back(line);
    }
    // create vertical lines
    for (unsigned int i = 0; i < canvasSizeX / gridSizeX + 1; i++)
    {
        glm::vec2 a = glm::vec2(i * gridSizeX, 0.f);
        glm::vec2 b = glm::vec2(i * gridSizeX, canvasSizeY);
        s2d::shapes::Line *line = new s2d::shapes::Line(a + position, b + position);
        lines.push_back(line);
    }
}
