#include "Level.h"
#include "utils/operations.h"

Level::Level(std::string name, s2d::Scene *scene, ResourceManager *rm, glm::vec2 canvasSize) : name(name), scene(scene), rm(rm)
{
    canvas = new s2d::Object();
    canvas->size = canvasSize;
    canvas->color = canvasColour;
    scene->addChild(canvas, "canvas");
    initCanvasEdges();
}

void Level::update()
{
    glm::vec2 mouse = scene->window->getMouseScreenPosition();
    selectEdge(mouse);
    if (placementObject != nullptr)
    {
        movePlacementObject(mouse);
        if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
            placementObject = nullptr;
    }
}

void Level::renderImGui()
{
    // ImGui::ShowDemoWindow();
    ImGui::Begin("Manage");
    levelOptions();
    ImGui::End();

    ImGui::Begin("Scene");
    sceneTree();
    ImGui::End();

    ImGui::Begin("Placement");
    placementUI();
    ImGui::End();
}

void Level::selectEdge(glm::vec2 mouse)
{
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

void Level::levelOptions()
{
    if (ImGui::ColorEdit3("Canvas colour", (float *)&canvasColour))
        canvas->color = canvasColour;
    if (ImGui::ColorEdit3("Edge colour", (float *)&edgeColour))
        for (s2d::Object *edge : edges)
            edge->color = edgeColour;
    if (ImGui::CollapsingHeader("Layers"))
    {
        addLayer();
        createLayersTable();
        removeLayer();
    }
    if (ImGui::CollapsingHeader("Entities"))
    {
        addEntity();
        createEntitiesTable();
        modifyEntity();
        removeEntity();
    }
    if (ImGui::CollapsingHeader("Textures"))
    {
        addTexture();
        createTextureTable();
    }
}

void Level::sceneTree()
{
    if (ImGui::TreeNode("Scene"))
    {
        for (s2d::Layer *layer : layers)
        {
            if (ImGui::TreeNode(layer->name.c_str()))
            {
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

void Level::placementUI()
{
    selectPlacementLayer();
    ImGui::Checkbox("Place mode", &placeMode);
    if (placeMode)
    {
        selectPlacementObject();
        ImGui::Checkbox("Place multiple", &placeMultiple);
        float scale = 1.f;
        ImGui::SliderFloat("scale", &scale, 0.f, 5.f);
        float rotation = 0.f;
        ImGui::SliderFloat("rotation", &rotation, -3.14f, 3.14f);
        float alpha = 0.f;
        ImGui::SliderFloat("alpha", &alpha, 0.f, 1.f);
    }
}

void Level::addLayer()
{
    if (ImGui::TreeNode("New layer"))
    {
        ImGui::InputText("Layer name", tmpLayerName, 64);
        if (ImGui::Button("Add"))
        {
            std::string layerName = std::string(tmpLayerName);
            bool layerExists = false;
            for (s2d::Layer *layer : layers)
                if (layer->name == layerName)
                    layerExists = true;
            if (layerExists)
                std::cout << "Layer name already exists." << std::endl;
            else
            {
                s2d::Layer *layer = new s2d::Layer(std::string(tmpLayerName));
                layers.push_back(layer);
                scene->layerStack->addLayer(layer);
            }
        }
        ImGui::TreePop();
    }
}

void Level::removeLayer()
{
    if (layerRowSelection.size() > 0)
        if (ImGui::Button("Remove layer"))
        {
            s2d::Layer *layer = layers[layerRowSelection[0]];
            layers.erase(layers.begin() + layerRowSelection[0]);
            scene->layerStack->removeLayer(layer);
            delete layer;
        }
}

void Level::createLayersTable()
{
    if (ImGui::BeginTable("level_layers", 4, tableFlags, tableSize))
    {
        ImGui::TableSetupColumn("name");
        ImGui::TableSetupColumn("position");
        ImGui::TableSetupColumn("depth");
        ImGui::TableSetupColumn("visible");

        ImGui::TableHeadersRow();

        for (int row = 0; row < layers.size(); row++)
        {
            ImGui::TableNextRow();
            const bool item_is_selected = layerRowSelection.contains(row);
            ImGui::PushID(row);

            if (ImGui::TableSetColumnIndex(0))
            {
                if (ImGui::Selectable(layers[row]->name.c_str(), item_is_selected))
                {
                    if (layerRowSelection.size() == 0)
                        layerRowSelection.push_back(row);
                    else
                    {
                        if (row == layerRowSelection[0])
                            layerRowSelection.clear();
                        else
                        {
                            layerRowSelection.clear();
                            layerRowSelection.push_back(row);
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
        selectEntityTexture();
        if (ImGui::Button("Add"))
        {
            std::string entityName = std::string(tmpEntityName);
            s2d::Object *obj = new s2d::Object();
            obj->label = entityName;
            obj->size = glm::vec2((float)tmpWidth, (float)tmpHeight);
            obj->color = tmpEntityColour;
            obj->setTexture(rm->getTexture(std::string(tmpNewTexture)));
            entities.push_back(obj);
            entitiesMap[entityName] = obj;
        }
        ImGui::TreePop();
    }
}

void Level::modifyEntity()
{
    if (entityRowSelection.size() > 0)
    {
        if (ImGui::TreeNode("Modify"))
        {
            // s2d::Object* obj = entityRowSelection
            ImGui::InputText("Entity name", tmpEntityName, 64);
            ImGui::ColorEdit3("Colour", (float *)&tmpEntityColour);
            if (tmpWidth <= 0)
                tmpWidth = 0;
            if (tmpHeight <= 0)
                tmpHeight = 0;
            ImGui::InputInt("Width", &tmpWidth);
            ImGui::InputInt("Height", &tmpHeight);
            selectEntityTexture();
            if (ImGui::Button("Save"))
            {
                entities[entityRowSelection[0]]->label = tmpEntityName;
                entities[entityRowSelection[0]]->color = tmpEntityColour;
                entities[entityRowSelection[0]]->size = glm::vec2((float)tmpWidth, (float)tmpHeight);
                entities[entityRowSelection[0]]->setTexture(rm->getTexture(std::string(tmpNewTexture)));
            }
            ImGui::TreePop();
        }
    }
}

void Level::removeEntity()
{
    if (entityRowSelection.size() > 0)
        if (ImGui::Button("Remove layer"))
            entities.erase(entities.begin() + entityRowSelection[0]);
}

void Level::selectEntityTexture()
{
    const char *tmpTextures[textures.size()];
    for (int i = 0; i < textures.size(); i++)
        tmpTextures[i] = textures[i].name.c_str();

    if (ImGui::BeginCombo("##combo", tmpNewTexture))
    {
        for (int n = 0; n < IM_ARRAYSIZE(tmpTextures); n++)
        {
            bool is_selected = (tmpNewTexture == tmpTextures[n]);
            if (ImGui::Selectable(tmpTextures[n], is_selected))
            {
                tmpNewTexture = tmpTextures[n];
                ResourceManager::TextureData td = rm->getTexture(std::string(tmpNewTexture));
                tmpWidth = td.width;
                tmpHeight = td.height;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void Level::createEntitiesTable()
{
    if (ImGui::BeginTable("entities", 4, tableFlags, tableSize))
    {
        ImGui::TableSetupColumn("name");
        ImGui::TableSetupColumn("size");
        ImGui::TableSetupColumn("colour");
        ImGui::TableSetupColumn("texture");

        ImGui::TableHeadersRow();

        for (int row = 0; row < entities.size(); row++)
        {
            ImGui::TableNextRow();
            const bool item_is_selected = entityRowSelection.contains(row);
            ImGui::PushID(row);
            s2d::Object *ent = entities[row];

            if (ImGui::TableSetColumnIndex(0))
            {
                if (ImGui::Selectable(ent->label.c_str(), item_is_selected))
                {
                    if (entityRowSelection.size() == 0)
                        entityRowSelection.push_back(row);
                    else
                    {
                        if (row == entityRowSelection[0])
                            entityRowSelection.clear();
                        else
                        {
                            entityRowSelection.clear();
                            entityRowSelection.push_back(row);
                        }
                    }
                }
            }
            if (ImGui::TableSetColumnIndex(1))
            {
                glm::vec2 size = ent->size;
                std::string out = "(";
                out += std::to_string((unsigned int)size.x);
                out += ", ";
                out += std::to_string((unsigned int)size.y);
                out += ")";
                ImGui::Text(out.c_str());
            }
            if (ImGui::TableSetColumnIndex(2))
            {
                std::string out = "(";
                out += std::to_string((int)ent->color.r * 255) + ", ";
                out += std::to_string((int)ent->color.g * 255) + ", ";
                out += std::to_string((int)ent->color.b * 255);
                out += ")";
                ImGui::Text(out.c_str());
            }
            if (ImGui::TableSetColumnIndex(3))
                ImGui::Text(ent->texture.name.c_str());
        }

        ImGui::EndTable();
    }
}

void Level::addTexture()
{
    if (ImGui::TreeNode("Add texture"))
    {
        ImGui::InputText("Texture name", tmpTextureName, 64);
        if (ImGui::Button("Choose texture"))
            ImGuiFileDialog::Instance()->OpenDialog("choose_texture", "Choose a texture", ".png,.jpg,.jpeg", ".");
        // display
        if (ImGuiFileDialog::Instance()->Display("choose_texture"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();
                tmpTexturePath = filename;
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::Text(tmpTexturePath.c_str());
        if (ImGui::Button("Add"))
        {
            rm->loadTexture(tmpTexturePath.c_str(), tmpTextureName, false);
            textures.push_back(rm->getTexture(tmpTextureName));
        }
        ImGui::TreePop();
    }
}

ImVector<char *> Level::getTextureNames()
{
    ImVector<char *> names = {};
    for (ResourceManager::TextureData td : textures)
        names.push_back(const_cast<char *>(td.name.c_str()));
    return names;
}

void Level::createTextureTable()
{
    if (ImGui::BeginTable("textures_table", 3, tableFlags, tableSize))
    {
        ImGui::TableSetupColumn("name");
        ImGui::TableSetupColumn("size");
        ImGui::TableSetupColumn("path");

        ImGui::TableHeadersRow();

        for (int row = 0; row < textures.size(); row++)
        {
            ImGui::TableNextRow();
            ResourceManager::TextureData td = textures[row];

            if (ImGui::TableSetColumnIndex(0))
                ImGui::Text(td.name.c_str());
            if (ImGui::TableSetColumnIndex(1))
            {
                std::string out = "(";
                out += std::to_string((unsigned int)td.width);
                out += ", ";
                out += std::to_string((unsigned int)td.height);
                out += ")";
                ImGui::Text(out.c_str());
            }
            if (ImGui::TableSetColumnIndex(2))
                ImGui::Text(td.path.c_str());
        }
        ImGui::EndTable();
    }
}

void Level::selectPlacementLayer()
{
    const char *tmpLayers[layers.size()];
    for (int i = 0; i < layers.size(); i++)
        tmpLayers[i] = layers[i]->name.c_str();

    if (ImGui::BeginCombo("Select layer", selectedPlacementLayer))
    {
        for (int n = 0; n < IM_ARRAYSIZE(tmpLayers); n++)
        {
            bool is_selected = (selectedPlacementLayer == tmpLayers[n]);
            if (ImGui::Selectable(tmpLayers[n], is_selected))
                selectedPlacementLayer = tmpLayers[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void Level::selectPlacementObject()
{
    const char *tmpEntities[entities.size()];
    for (int i = 0; i < entities.size(); i++)
        tmpEntities[i] = entities[i]->label.c_str();

    if (ImGui::BeginCombo("Select entity", tmpPlacementEntity))
    {
        for (int n = 0; n < IM_ARRAYSIZE(tmpEntities); n++)
        {
            bool is_selected = (tmpPlacementEntity == tmpEntities[n]);
            if (ImGui::Selectable(tmpEntities[n], is_selected))
            {
                tmpPlacementEntity = tmpEntities[n];
                if (selectedPlacementLayer != "")
                {
                    placementObject = entities[n];
                    scene->addChild(placementObject, std::string(selectedPlacementLayer));
                    canvas->addChild(placementObject);
                }
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void Level::movePlacementObject(glm::vec2 mouse)
{
    glm::vec2 objectScreenPos = placementObject->getScreenPosition(
        scene->camera->getViewMatrix(),
        scene->camera->getProjectionMatrix(true),
        scene->windowWidth,
        scene->windowHeight);
    float dx = (mouse.x - objectScreenPos.x) * scene->camera->zoom;
    float dy = (mouse.y - objectScreenPos.y) * scene->camera->zoom;
    placementObject->move(dx, dy);
}