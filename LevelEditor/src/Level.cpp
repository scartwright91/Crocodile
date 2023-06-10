#include "Level.h"

Level::Level(LevelData data, s2d::Scene *scene, ResourceManager *rm) : name(data.name), scene(scene), rm(rm)
{
    canvas = new s2d::Object();
    canvas->size = data.canvasSize;
    canvasColour = data.canvasColor;
    canvas->color = canvasColour;
    entitiesData = data.entitiesData;
    layers = data.layers;
    for (s2d::Layer *l : layers)
        scene->layerStack->addLayer(l);
    textures = data.textures;
    scene->addChild(canvas, "canvas");
    initCanvasEdges();
    loadPlacedEntities(data);
}

Level::Level(std::string name, s2d::Scene *scene, ResourceManager *rm, glm::vec2 canvasSize) : name(name), scene(scene), rm(rm)
{
    canvas = new s2d::Object();
    canvas->size = canvasSize;
    canvas->color = canvasColour;
    scene->addChild(canvas, "canvas");
    initCanvasEdges();
}

void Level::loadPlacedEntities(LevelData data)
{
    for (s2d::SceneEntityData *sed : data.sceneEntityData)
    {
        s2d::Object *obj = new s2d::Object();
        obj->label = sed->label;
        obj->rotation = sed->rotation;
        obj->alpha = sed->alpha;
        obj->setPosition(sed->pos);
        obj->size = sed->size;
        obj->setTexture(rm->getTexture(sed->texture));
        scene->addChild(obj, sed->layer);
    }
}

void Level::update()
{
    glm::vec2 mouse = scene->window->getMouseScreenPosition();
    bool leftclick = scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1);
    float now = glfwGetTime();
    calculateMouseWorldPos(mouse);
    selectEdge(mouse);
    if (placementObject != nullptr)
    {
        moveObject(placementObject, mouse);
        if (leftclick && (now - placementTimer > 0.5f))
        {
            placementTimer = now;
            if (placeMultiple)
                createEntityFromData(selectedEntityData);
            else
            {
                scene->removeChild(placementObject, std::string(selectedPlacementLayer));
                delete placementObject;
                placementObject = nullptr;
            }
        }
        else if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2))
        {
            scene->removeChild(placementObject, std::string(selectedPlacementLayer));
            delete placementObject;
            placementObject = nullptr;
        }
    }
    if (selectedObject != nullptr)
    {
        if (moveSelectedObject)
        {
            moveObject(selectedObject, mouse);
            if (leftclick)
                moveSelectedObject = false;
        }
        else
        {
            if (scene->window->isKeyPressed(GLFW_KEY_M))
                moveSelectedObject = true;
        }
    }
}

LevelData Level::serialise()
{
    LevelData ld = {};
    ld.canvasSize = canvas->size;
    ld.canvasColor = canvasColour;
    ld.entitiesData = entitiesData;
    ld.layers = layers;
    ld.textures = textures;
    std::vector<s2d::SceneEntityData *> sceneEntitiesData = {};
    for (std::string layerName : scene->layerStack->getLayerNames())
    {
        if (layerName != "canvas" && layerName != "hud" && layerName != "canvas_edges")
        {
            for (s2d::Object *obj : scene->layerStack->getLayer(layerName)->objects)
            {
                s2d::SceneEntityData *sed = new s2d::SceneEntityData();
                sed->label = obj->label;
                sed->layer = layerName;
                sed->pos = obj->getPosition();
                sed->size = obj->size;
                sed->rotation = obj->rotation;
                sed->alpha = obj->alpha;
                sed->texture = obj->texture.name;
                sceneEntitiesData.push_back(sed);
            }
        }
    }
    ld.sceneEntityData = sceneEntitiesData;
    return ld;
}

void Level::renderImGui()
{
    ImGui::Begin("Manage");
    levelOptions();
    ImGui::End();

    ImGui::Begin("Level");
    levelInfo();
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
        true,
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
    // updateCanvasOrigin();
    updateCanvas();
    updateEdges();
}

void Level::updateCanvas()
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

void Level::updateEdges()
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
}

void Level::updateCanvasOrigin()
{
    glm::vec2 pos = canvas->getPosition();
    if (pos.x != 0.f || pos.y != 0.f)
    {
        canvas->move(-pos.x, -pos.y);
        for (std::string layerName : scene->layerStack->getLayerNames())
        {
            s2d::Layer *layer = scene->layerStack->getLayer(layerName);
            if (layer->cameraScroll)
                for (s2d::Object *obj : layer->objects)
                    obj->move(-pos.x, -pos.y);
        }
    }
}

void Level::scaleEdges(float v)
{
    edgeWidth = startEdgeWidth * v;
    updateEdges();
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

void Level::calculateMouseWorldPos(glm::vec2 mouse)
{
    s2d::col::BoundingBox bbox = canvas->getScreenBoundingBox(
        scene->camera->getViewMatrix(),
        scene->camera->getProjectionMatrix(true),
        scene->camera->zoom,
        scene->windowWidth,
        scene->windowHeight);
    if (bbox.intersectsPoint(mouse))
    {
        glm::vec2 size = canvas->size;
        float dx = size.x * (mouse.x - bbox.x) / bbox.width;
        float dy = size.y * (mouse.y - bbox.y) / bbox.height;
        mouseWorldPos = glm::vec2(dx, dy);
    }
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

void Level::levelInfo()
{
    std::string mousePos = "Mouse pos: ";
    mousePos += "(" + std::to_string((int)mouseWorldPos.x) + ", " + std::to_string((int)mouseWorldPos.y) + ")";
    ImGui::Text(mousePos.c_str());
    glm::vec2 size = canvas->size;
    std::string cs = "Level size: ";
    cs += "(" + std::to_string((int)size.x) + ", " + std::to_string((int)size.y) + ")";
    ImGui::Text(cs.c_str());
}

void Level::sceneTree()
{
    if (ImGui::TreeNode("Scene"))
    {
        for (s2d::Layer *layer : layers)
        {
            if (ImGui::TreeNode(layer->name.c_str()))
            {
                for (s2d::Object *obj : scene->layerStack->getLayer(layer->name)->objects)
                {
                    if (ImGui::TreeNode(obj->label.c_str()))
                    {
                        ImGui::Text(obj->label.c_str());
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

void Level::placementUI()
{
    selectPlacementLayer();
    if (selectedPlacementLayer == "")
        return;
    if (ImGui::Checkbox("Place mode", &placeMode))
        selectedObject = nullptr;
    if (placeMode)
    {
        selectPlacementObject();
        ImGui::Checkbox("Place multiple", &placeMultiple);
        ImGui::SliderFloat("scale", &tmpScale, 0.f, 5.f);
        ImGui::SliderFloat("rotation", &tmpRotation, -3.14f, 3.14f);
        ImGui::SliderFloat("alpha", &tmpAlpha, 0.f, 1.f);
    }
    else
    {
        selectObject();
        if (selectedObject != NULL)
        {
            std::string objPosText = "Object pos: ";
            glm::vec2 objPos = selectedObject->getPosition();
            objPosText += "(" + std::to_string((int)objPos.x) + ", " + std::to_string((int)objPos.y) + ")";
            ImGui::Text(objPosText.c_str());
            ImGui::SliderFloat("rotation", &selectedObject->rotation, -3.14f, 3.14f);
            ImGui::SliderFloat("alpha", &selectedObject->alpha, 0.f, 1.f);
            if (ImGui::Button("delete"))
            {
                scene->removeChild(selectedObject, selectedPlacementLayer);
                selectedObject = nullptr;
            }
        }
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
                    if (row - 1 >= 0)
                    {
                        move(layers, row, row - 1);
                        scene->layerStack->moveLayerDown(layers[row]->name);
                    }
                ImGui::SameLine();
                if (ImGui::SmallButton("-"))
                    if (row + 1 < layers.size())
                    {
                        move(layers, row, row + 1);
                        scene->layerStack->moveLayerUp(layers[row]->name);
                    }
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
            s2d::EntityData *ed = new s2d::EntityData();
            ed->label = entityName;
            ed->size = glm::vec2((float)tmpWidth, (float)tmpHeight);
            ed->colour = tmpEntityColour;
            ed->texture = rm->getTexture(std::string(tmpNewTexture));
            entitiesData.push_back(ed);
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
                entitiesData[entityRowSelection[0]]->label = tmpEntityName;
                entitiesData[entityRowSelection[0]]->colour = tmpEntityColour;
                entitiesData[entityRowSelection[0]]->size = glm::vec2((float)tmpWidth, (float)tmpHeight);
                entitiesData[entityRowSelection[0]]->texture = rm->getTexture(std::string(tmpNewTexture));
            }
            ImGui::TreePop();
        }
    }
}

void Level::removeEntity()
{
    if (entityRowSelection.size() > 0)
        if (ImGui::Button("Remove entity"))
            entitiesData.erase(entitiesData.begin() + entityRowSelection[0]);
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

        for (int row = 0; row < entitiesData.size(); row++)
        {
            ImGui::TableNextRow();
            const bool item_is_selected = entityRowSelection.contains(row);
            ImGui::PushID(row);
            s2d::EntityData *ed = entitiesData[row];

            if (ImGui::TableSetColumnIndex(0))
            {
                if (ImGui::Selectable(ed->label.c_str(), item_is_selected))
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
                glm::vec2 size = ed->size;
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
                out += std::to_string((int)ed->colour.r * 255) + ", ";
                out += std::to_string((int)ed->colour.g * 255) + ", ";
                out += std::to_string((int)ed->colour.b * 255);
                out += ")";
                ImGui::Text(out.c_str());
            }
            if (ImGui::TableSetColumnIndex(3))
                ImGui::Text(ed->texture.name.c_str());
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

void Level::createEntityFromData(s2d::EntityData *entityData)
{
    placementObject = new s2d::Object();
    placementObject->label = entityData->label;
    placementObject->size = entityData->size;
    placementObject->color = entityData->colour;
    placementObject->setTexture(entityData->texture);
    placementObject->alpha = tmpAlpha;
    placementObject->rotation = tmpRotation;
    placementObject->size *= tmpScale;
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
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
    const char *tmpEntities[entitiesData.size()];
    for (int i = 0; i < entitiesData.size(); i++)
        tmpEntities[i] = entitiesData[i]->label.c_str();

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
                    selectedEntityData = entitiesData[n];
                    createEntityFromData(selectedEntityData);
                }
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void Level::moveObject(s2d::Object *obj, glm::vec2 mouse)
{
    glm::vec2 objectScreenPos = obj->getScreenPosition(
        true,
        scene->camera->getViewMatrix(),
        scene->camera->getProjectionMatrix(true),
        scene->windowWidth,
        scene->windowHeight);
    float dx = (mouse.x - objectScreenPos.x) * scene->camera->zoom;
    float dy = (mouse.y - objectScreenPos.y) * scene->camera->zoom;
    obj->move(dx, dy);
}

void Level::selectObject()
{
    glm::vec2 mouse = scene->window->getMouseScreenPosition();
    for (s2d::Object *obj : scene->layerStack->getLayer(std::string(selectedPlacementLayer))->objects)
    {
        if (obj->getScreenBoundingBox(
                   scene->camera->getViewMatrix(),
                   scene->camera->getProjectionMatrix(true),
                   scene->camera->zoom,
                   scene->windowWidth,
                   scene->windowHeight)
                .intersectsPoint(mouse))
        {
            obj->outline = true;
            if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
                selectedObject = obj;
            break;
        }
        else
            obj->outline = false;
    }
    if (selectedObject != NULL)
        selectedObject->outline = true;
}
