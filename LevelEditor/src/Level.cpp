#include "Level.h"

Level::Level(std::string name, s2d::Scene *scene, glm::vec2 canvasSize) : name(name), scene(scene)
{
    canvas = new s2d::Object();
    canvas->size = canvasSize;
    canvas->color = glm::vec3(0.8f, 0.3f, 0.1f);
    scene->addChild(canvas, "canvas");
    initCanvasEdges();
}

void Level::update()
{
    selectEdge();
}

void Level::selectEdge()
{
    glm::vec2 mouse = scene->window->getMouseScreenPosition();
    if (edgeSelected != NULL)
    {
        if (!scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
        {
            moveEdge(mouse);
            edgeSelected = nullptr;
        }
    }
    else
    {
        for (s2d::Object *obj : edges)
        {
            s2d::col::BoundingBox bbox = obj->getScreenBoundingBox(
                scene->camera->getViewMatrix(),
                scene->camera->getProjectionMatrix(true),
                scene->camera->zoom,
                scene->windowWidth,
                scene->windowHeight);
            if (bbox.intersectsPoint(mouse) && scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
            {
                edgeSelected = obj;
                break;
            }
        }
    }
}

void Level::moveEdge(glm::vec2 mouse)
{
    glm::vec2 edgeScreenPos = edgeSelected->getScreenPosition(
        scene->camera->getViewMatrix(),
        scene->camera->getProjectionMatrix(true),
        scene->windowWidth,
        scene->windowHeight);
    float dx = (mouse.x - edgeScreenPos.x) * scene->camera->zoom;
    float dy = (mouse.y - edgeScreenPos.y) * scene->camera->zoom;
    if (edgeSelected == edges[0] || edgeSelected == edges[1])
        edgeSelected->move(dx, 0);
    else if (edgeSelected == edges[2] || edgeSelected == edges[3])
        edgeSelected->move(0, dy);
    updateCanvas();
    updateEdges();
}

void Level::updateCanvas()
{
    float minx = edges[0]->getPosition().x;
    float miny = edges[2]->getPosition().y;
    float canvasWidth = edges[1]->getPosition().x - edges[0]->getPosition().x;
    float canvasHeight = edges[3]->getPosition().y - edges[2]->getPosition().y;
    canvas->setPosition(glm::vec2(minx, miny));
    canvas->size = glm::vec2(canvasWidth, canvasHeight);
}

void Level::updateEdges()
{
    glm::vec2 size = canvas->size;
    edges[0]->size = glm::vec2(edgeWidth, size.y);
    edges[1]->size = glm::vec2(edgeWidth, size.y);
    edges[2]->size = glm::vec2(size.x, edgeWidth);
    edges[3]->size = glm::vec2(size.x, edgeWidth);
    glm::vec2 pos = canvas->getPosition();
    edges[0]->setPosition(pos);
    edges[1]->setPosition(glm::vec2(pos.x + size.x, pos.y));
    edges[2]->setPosition(pos);
    edges[3]->setPosition(glm::vec2(pos.x, pos.y + canvas->size.y));
}

void Level::initCanvasEdges()
{
    // left
    s2d::Object *left = new s2d::Object();
    left->label = "left";
    left->color = glm::vec3(0.f);
    scene->addChild(left, "canvas_edges");

    // right
    s2d::Object *right = new s2d::Object();
    right->label = "right";
    right->color = glm::vec3(0.f);
    scene->addChild(right, "canvas_edges");

    // top
    s2d::Object *top = new s2d::Object();
    top->label = "top";
    top->color = glm::vec3(0.f);
    scene->addChild(top, "canvas_edges");

    // bottom
    s2d::Object *bottom = new s2d::Object();
    bottom->label = "bottom";
    bottom->color = glm::vec3(0.f);
    scene->addChild(bottom, "canvas_edges");

    edges.push_back(left);
    edges.push_back(right);
    edges.push_back(top);
    edges.push_back(bottom);

    updateEdges();
}