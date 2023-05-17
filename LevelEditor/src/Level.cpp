#include "Level.h"

Level::Level(std::string name, s2d::Scene *scene) : name(name), scene(scene)
{
    canvas = new s2d::Object();
    canvas->size = glm::vec2(600.f);
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
            std::cout << "Drop edge" << std::endl;
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
                std::cout << "Select edge" << std::endl;
                break;
            }
        }
    }
}

void Level::moveEdge(glm::vec2 mouse)
{
    if (edgeSelected == edges[0])
    {
    }
    else if (edgeSelected == edges[1])
    {
        float dx = mouse.x - edges[1]->getPosition().x;
        edgeSelected->move(dx, 0);
    }
    else if (edgeSelected == edges[2])
    {
    }
    else if (edgeSelected == edges[3])
    {
    }
    updateCanvas();
}

void Level::updateCanvas()
{
    glm::vec2 pos = edges[0]->getPosition();
    float canvasWidth = edges[1]->getPosition().x - edges[0]->getPosition().x;
    float canvasHeight = edges[3]->getPosition().y - edges[2]->getPosition().y;
    canvas->move(pos.x, pos.y);
    canvas->size = glm::vec2(canvasWidth, canvasHeight);
}

void Level::initCanvasEdges()
{
    glm::vec2 pos = glm::vec2(0.f);

    // left
    s2d::Object *left = new s2d::Object();
    left->label = "left";
    left->setPosition(pos);
    left->size = glm::vec2(20.f, 600.f);
    left->color = glm::vec3(0.f);
    scene->addChild(left, "canvas_edges");

    // right
    s2d::Object *right = new s2d::Object();
    right->label = "right";
    right->setPosition(glm::vec2(pos.x + canvas->size.x, pos.y));
    right->size = glm::vec2(20.f, 600.f);
    right->color = glm::vec3(0.f);
    scene->addChild(right, "canvas_edges");

    // top
    s2d::Object *top = new s2d::Object();
    top->label = "top";
    top->setPosition(canvas->getPosition());
    top->size = glm::vec2(600.f, 20.f);
    top->color = glm::vec3(0.f);
    scene->addChild(top, "canvas_edges");

    // bottom
    s2d::Object *bottom = new s2d::Object();
    bottom->label = "bottom";
    bottom->setPosition(glm::vec2(pos.x, pos.y + canvas->size.y));
    bottom->size = glm::vec2(600.f, 20.f);
    bottom->color = glm::vec3(0.f);
    scene->addChild(bottom, "canvas_edges");

    edges.push_back(left);
    edges.push_back(right);
    edges.push_back(top);
    edges.push_back(bottom);
}