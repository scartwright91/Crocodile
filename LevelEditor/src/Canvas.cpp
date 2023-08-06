#include "Canvas.h"

Canvas::Canvas(s2d::Scene *scene) : scene(scene)
{
}

Canvas::~Canvas()
{
}

void Canvas::update(float dt, glm::vec2 mouse)
{
    sceneMousePos = glm::vec2(mouse.x, scene->window->getMouseScreenPosition().y + (2 * ImGui::GetStyle().FramePadding.y)); // why this works I have no idea
    selectEdge();
}

void Canvas::renderImGui()
{
}

void Canvas::updateCanvas()
{
    float minx = edges[0]->getPosition().x + edgeWidth;
    float maxx = edges[1]->getPosition().x;
    float miny = edges[2]->getPosition().y + edgeWidth;
    float maxy = edges[3]->getPosition().y;
    canvas->setPosition(glm::vec2(minx, miny));
    if (minx != 0 || miny != 0)
    {
        for (std::string layerName : scene->layerStack->getLayerNames())
        {
            s2d::Layer *layer = scene->layerStack->getLayer(layerName);
            if (layer->cameraScroll)
                for (s2d::Object *obj : layer->objects)
                    obj->move(-minx, -miny);
        }
    }
    canvas->size = glm::vec2(maxx - minx, maxy - miny);
}

void Canvas::initCanvasEdges()
{
    // left
    s2d::Object *left = new s2d::Object();
    left->label = "left";
    scene->addChild(left, "canvas_edges");
    // right
    s2d::Object *right = new s2d::Object();
    right->label = "right";
    scene->addChild(right, "canvas_edges");
    // top
    s2d::Object *top = new s2d::Object();
    top->label = "top";
    scene->addChild(top, "canvas_edges");
    // bottom
    s2d::Object *bottom = new s2d::Object();
    bottom->label = "bottom";
    scene->addChild(bottom, "canvas_edges");

    edges.push_back(left);
    edges.push_back(right);
    edges.push_back(top);
    edges.push_back(bottom);

    for (s2d::Object *edge : edges)
        edge->color = edgeColour;

    updateEdges();
}

void Canvas::selectEdge()
{
    if (edgeSelected != NULL)
    {
        if (edgeSelected == edges[0] || edgeSelected == edges[1])
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        else
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
        if (!scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
        {
            moveEdge();
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
            if (bbox.intersectsPoint(sceneMousePos))
            {
                if (obj == edges[0] || obj == edges[1])
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                else
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
                {
                    edgeSelected = obj;
                    break;
                }
            }
        }
    }
}

void Canvas::moveEdge()
{
    glm::vec2 edgeScreenPos = edgeSelected->getScreenPosition(
        true,
        scene->camera->getViewMatrix(),
        scene->camera->getProjectionMatrix(true),
        scene->windowWidth,
        scene->windowHeight);
    float dx = (sceneMousePos.x - edgeScreenPos.x) * scene->camera->zoom;
    float dy = (sceneMousePos.y - edgeScreenPos.y) * scene->camera->zoom;
    if (edgeSelected == edges[0] || edgeSelected == edges[1])
        edgeSelected->move(dx, 0);
    else if (edgeSelected == edges[2] || edgeSelected == edges[3])
        edgeSelected->move(0, dy);
    updateCanvas();
    updateEdges();
}

void Canvas::scaleEdges()
{
    edgeWidth = startEdgeWidth * scene->camera->zoom;
    updateEdges();
}

void Canvas::updateEdges()
{
    glm::vec2 size = canvas->size;
    edges[0]->size = glm::vec2(edgeWidth, size.y);
    edges[1]->size = glm::vec2(edgeWidth, size.y);
    edges[2]->size = glm::vec2(size.x, edgeWidth);
    edges[3]->size = glm::vec2(size.x, edgeWidth);
    glm::vec2 pos = canvas->getPosition();
    edges[0]->setPosition(glm::vec2(pos.x - edgeWidth, pos.y));
    edges[1]->setPosition(glm::vec2(pos.x + size.x, pos.y));
    edges[2]->setPosition(glm::vec2(pos.x, pos.y - edgeWidth));
    edges[3]->setPosition(glm::vec2(pos.x, pos.y + canvas->size.y));
    initGrid();
}

void Canvas::initGrid()
{
    delete grid;
    grid = new Grid(scene, (int)canvas->size.x, (int)canvas->size.y, (int)gridSizeX, (int)gridSizeY);
}
