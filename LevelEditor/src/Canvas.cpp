#include "Canvas.h"

Canvas::Canvas(s2d::Scene *scene,
               glm::vec2 pos,
               glm::vec2 size) : scene(scene)
{
    canvas = new s2d::Object();
    canvas->setPosition(pos);
    canvas->size = size;
    canvas->color = canvasColour;
    scene->addChild(canvas, "canvas");
    initCanvasEdges();
}

Canvas::Canvas(s2d::Scene *scene,
               glm::vec2 pos,
               glm::vec2 size,
               glm::vec3 color) : scene(scene)
{
    canvas = new s2d::Object();
    canvas->setPosition(pos);
    canvas->size = size;
    canvasColour = color;
    canvas->color = canvasColour;
    scene->addChild(canvas, "canvas");
    initCanvasEdges();
}

Canvas::~Canvas()
{
    scene->removeChild(canvas, "canvas");
    delete canvas;
    for (s2d::Object *edge : edges)
    {
        scene->removeChild(edge, "canvas_edges");
        delete edge;
    }
    delete grid;
}

void Canvas::update(float dt, glm::vec2 mouse)
{
    sceneMousePos = glm::vec2(mouse.x, scene->window->getMouseScreenPosition().y + (2 * ImGui::GetStyle().FramePadding.y)); // why this works I have no idea
    calculateWorldPosition();
    selectEdge();
}

void Canvas::renderImGui()
{
    if (ImGui::CollapsingHeader("Canvas"))
    {
        if (ImGui::ColorEdit3("Canvas colour", (float *)&canvasColour))
            canvas->color = canvasColour;
        if (ImGui::ColorEdit3("Edge colour", (float *)&edgeColour))
            for (s2d::Object *edge : edges)
                edge->color = edgeColour;
    }
    if (ImGui::CollapsingHeader("Grid"))
    {
        ImGui::Checkbox("Show", &showGrid);
        if (showGrid)
        {
            if (!grid->visible)
                grid->show();
        }
        else
        {
            if (grid->visible)
                grid->hide();
        }
        ImGui::InputInt("Grid X", &gridSizeX);
        ImGui::InputInt("Grid Y", &gridSizeY);
        if (ImGui::Button("Create"))
            initGrid();
    }
}

void Canvas::calculateWorldPosition()
{
    s2d::col::BoundingBox bbox = canvas->getScreenBoundingBox(
        scene->camera->getViewMatrix(),
        scene->camera->getProjectionMatrix(true),
        scene->camera->zoom,
        scene->windowWidth,
        scene->windowHeight);
    if (bbox.intersectsPoint(sceneMousePos))
    {
        glm::vec2 size = canvas->size;
        float dx = size.x * (sceneMousePos.x - bbox.x) / bbox.width;
        float dy = size.y * (sceneMousePos.y - bbox.y) / bbox.height;
        mouseWorldPos = glm::vec2(dx, dy);
        mouseWorldPosGrid = grid->getGridPosition(mouseWorldPos);
    }
}

glm::vec2 Canvas::getWorldGridPosition()
{
    return mouseWorldPosGrid * glm::vec2(grid->gridSizeX, gridSizeY);
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
    grid = new Grid(scene, canvas->getPosition(), (int)canvas->size.x, (int)canvas->size.y, (int)gridSizeX, (int)gridSizeY);
}
