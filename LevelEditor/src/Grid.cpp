#include "Grid.h"

Grid::Grid(
    unsigned int canvasSizeX,
    unsigned int canvasSizeY,
    unsigned int gridSizeX,
    unsigned int gridSizeY) : canvasSizeX(canvasSizeX), canvasSizeY(canvasSizeY), gridSizeX(gridSizeX), gridSizeY(gridSizeY)
{
    createLines();
}

Grid::~Grid()
{
    for (s2d::Object *line : lines)
        delete line;
}

void Grid::show()
{
    visible = true;
}

void Grid::hide()
{
    visible = false;
}

void Grid::createLines()
{
}
