#include "Level.h"
#include "utils/operations.h"

Level::Level(std::string name, s2d::Scene *scene, glm::vec2 canvasSize) : name(name), scene(scene)
{
    canvas = new s2d::Object();
    canvas->size = canvasSize;
    canvas->color = glm::vec3(0.8f, 0.3f, 0.1f);
    scene->addChild(canvas, "canvas");
    initCanvasEdges();

    // init some layers
    layers.push_back(new s2d::Layer("background", 0.f));
    layers.push_back(new s2d::Layer("entity", 0.f));
    layers.push_back(new s2d::Layer("foreground", 0.f));
}

void Level::update()
{
    selectEdge();
}

void Level::renderImGui()
{
    ImGui::ShowDemoWindow();
    ImGui::Begin("Level options");
    levelOptions();
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
            if (bbox.intersectsPoint(mouse))
            {
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
                {
                    edgeSelected = obj;
                    break;
                }
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

void Level::levelOptions()
{
    if (ImGui::CollapsingHeader("Layers"))
    {
        addLayer();
        createLayersTable();
        if (rowSelection.size() > 0)
            if (ImGui::Button("Remove layer"))
                layers.erase(layers.begin() + rowSelection[0]);
    }
    if (ImGui::CollapsingHeader("Entities"))
    {
        addEntity();
        createEntitiesTable();
    }
    if (ImGui::CollapsingHeader("Resources"))
    {
    }
}

void Level::addLayer()
{
    ImGui::InputText("Layer name", tmpLayerName, 64);
    ImGui::SameLine();
    if (ImGui::Button("New layer"))
    {
        std::string layerName = std::string(tmpLayerName);
        bool layerExists = false;
        for (s2d::Layer *layer : layers)
            if (layer->name == layerName)
                layerExists = true;
        if (layerExists)
            std::cout << "Layer name already exists." << std::endl;
        else
            layers.push_back(new s2d::Layer(std::string(tmpLayerName)));
    }
}

void Level::createLayersTable()
{
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("level_layers", 4, flags))
    {
        ImGui::TableSetupColumn("name");
        ImGui::TableSetupColumn("position");
        ImGui::TableSetupColumn("depth");
        ImGui::TableSetupColumn("visible");

        ImGui::TableHeadersRow();

        for (int row = 0; row < layers.size(); row++)
        {
            ImGui::TableNextRow();
            char label[32];
            const bool item_is_selected = rowSelection.contains(row);
            ImGui::PushID(row);

            if (ImGui::TableSetColumnIndex(0))
            {
                if (ImGui::Selectable(layers[row]->name.c_str(), item_is_selected))
                {
                    if (rowSelection.size() == 0)
                        rowSelection.push_back(row);
                    else
                    {
                        if (row == rowSelection[0])
                            rowSelection.clear();
                        else
                        {
                            rowSelection.clear();
                            rowSelection.push_back(row);
                        }
                    }
                }
            }
            if (ImGui::TableSetColumnIndex(1))
            {
                if (ImGui::SmallButton("+"))
                    if (row + 1 <= layers.size())
                        move(layers, row, row + 1);
                ImGui::SameLine();
                if (ImGui::SmallButton("-"))
                    if (row - 1 >= 0)
                        move(layers, row, row - 1);
            }
            if (ImGui::TableSetColumnIndex(2))
                ImGui::SliderFloat("depth", &layers[row]->depth, 0.0f, 5.0f);
            if (ImGui::TableSetColumnIndex(3))
            {
                bool hide = layers[row]->hide;
                std::string visible = std::to_string(!hide);
                if (ImGui::Button(visible.c_str(), ImVec2(-FLT_MIN, 0.0f)))
                    layers[row]->hide = !hide;
            }
        }
        ImGui::EndTable();
    }
}

void Level::addEntity()
{
    if (ImGui::TreeNode("New entity"))
    {
        ImGui::InputText("Entity name", tmpEntityName, 64);
        ImGui::ColorEdit3("Colour", (float *)&tmpEntityColour);
        if (tmpWidth <= 0)
            tmpWidth = 0;
        if (tmpHeight <= 0)
            tmpHeight = 0;
        ImGui::InputInt("Width", &tmpWidth);
        ImGui::InputInt("Height", &tmpHeight);
        if (ImGui::Button("Add"))
        {
            std::string entityName = std::string(tmpEntityName);
            s2d::Object *obj = new s2d::Object();
            obj->label = entityName;
            obj->size = glm::vec2((float)tmpWidth, (float)tmpHeight);
            obj->color = tmpEntityColour;
            entities[entityName] = obj;
        }
    }
}

void Level::createEntitiesTable()
{
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("entities", 4, flags))
    {
        ImGui::TableSetupColumn("name");
        ImGui::TableSetupColumn("size");
        ImGui::TableSetupColumn("colour");
        ImGui::TableSetupColumn("texture");

        ImGui::TableHeadersRow();

        std::map<std::string, s2d::Object *>::iterator it;
        for (it = entities.begin(); it != entities.end(); it++)
        {
            ImGui::TableNextRow();

            if (ImGui::TableSetColumnIndex(0))
            {
                ImGui::Text(it->first.c_str());
            }
            if (ImGui::TableSetColumnIndex(1))
            {
                glm::vec2 size = it->second->size;
                std::string out = "(";
                out += std::to_string((unsigned int)size.x);
                out += ", ";
                out += std::to_string((unsigned int)size.y);
                out += ")";
                ImGui::Text(out.c_str());
            }
            if (ImGui::TableSetColumnIndex(2))
                ImGui::Text("placeholder");
            if (ImGui::TableSetColumnIndex(3))
                ImGui::Text("placeholder");
        }

        ImGui::EndTable();
    }
}
