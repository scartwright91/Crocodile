#include "Level.h"

Level::Level(LevelData data, s2d::Scene *scene, ResourceManager *rm) : scene(scene), rm(rm), currentPath(fs::current_path())
{
    strcpy(name, data.name.c_str());
    canvas = new s2d::Object();
    canvas->size = data.canvasSize;
    canvasColour = data.canvasColor;
    canvas->color = canvasColour;
    entitiesData = data.entitiesData;
    layers = data.layers;
    for (s2d::Layer *l : layers)
        scene->layerStack->addLayer(l);
    textures = data.textures;
    for (ResourceManager::TextureData td : textures)
        rm->loadTexture(td.path.c_str(), td.name, false);
    scene->addChild(canvas, "canvas");
    initCanvasEdges();
    loadPlacedEntities(data);
}

Level::Level(std::string name, s2d::Scene *scene, ResourceManager *rm, glm::vec2 canvasSize) : scene(scene), rm(rm), currentPath(fs::current_path())
{
    strcpy(this->name, name.c_str());
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
        Entity *e = new Entity(scene, obj, sed->layer);
        e->movementPath = sed->path;
        placedEntities.push_back(e);
    }
    for (s2d::SceneTextEntityData *ted : data.sceneTextEntityData)
    {
        s2d::Text *t = new s2d::Text(ted->text, false);
        t->alpha = ted->alpha;
        t->setPosition(ted->pos);
        t->color = ted->color;
        t->textScale = ted->textScale;
        scene->addChild(t, ted->layer);
        TextEntity *te = new TextEntity(scene, t, ted->layer);
        placedTextEntities.push_back(te);
    }
    for (s2d::SceneParticleEntityData *ped : data.SceneParticleEntityData)
    {
        s2d::ParticleGenerator *pg = new s2d::ParticleGenerator(ped->amount);
        pg->direction = ped->direction;
        pg->dispersion = ped->dispersion;
        pg->scale = ped->scale;
        pg->velocity = ped->velocity;
        pg->setPosition(ped->pos);
        pg->color = ped->color;
        scene->addChild(pg, ped->layer);
        ParticleEntity *pe = new ParticleEntity(scene, pg, ped->layer);
        placedParticleEntities.push_back(pe);
    }
}

void Level::update(glm::vec2 mouse)
{
    sceneMousePos = glm::vec2(mouse.x, scene->window->getMouseScreenPosition().y + (2 * ImGui::GetStyle().FramePadding.y)); // why this works I have no idea
    bool leftclick = scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1);
    bool rightclick = scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2);
    float now = glfwGetTime();
    if (updateLevel)
    {
        // update particles
        for (ParticleEntity *pg : placedParticleEntities)
            pg->update();
    }
    calculateMouseWorldPos();
    selectEdge();
    if (placementObject != nullptr)
    {
        moveObject(placementObject);
        if (leftclick && (now - placementTimer > 0.5f))
        {
            placementTimer = now;
            if (placeMultiple)
                createEntityFromData(selectedEntityData);
            else
                placementObject = nullptr;
        }
        else if (rightclick)
        {
            scene->removeChild(placementObject, std::string(selectedPlacementLayer));
            delete placementObject;
            placementObject = nullptr;
        }
    }
    if (selectedObject != nullptr)
    {
        if (rightclick)
        {
            for (Entity *e : placedEntities)
                if (e->obj == selectedObject)
                {
                    e->deselect();
                    break;
                }
            selectedObject->outline = false;
            selectedObject = nullptr;
        }
        if (moveSelectedObject)
        {
            moveObject(selectedObject);
            if (leftclick)
                moveSelectedObject = false;
        }
        else
        {
            if (scene->window->isKeyPressed(GLFW_KEY_M))
                moveSelectedObject = true;
            createObjectPath();
        }
    }
}

LevelData Level::serialise()
{
    LevelData ld = {};
    ld.name = std::string(name);
    ld.canvasSize = canvas->size;
    ld.canvasColor = canvasColour;
    ld.entitiesData = entitiesData;
    ld.layers = layers;
    ld.textures = textures;
    try
    {
        {
            // entities
            std::vector<s2d::SceneEntityData *> sceneEntitiesData = {};
            for (Entity *ent : placedEntities)
            {
                s2d::SceneEntityData *sed = new s2d::SceneEntityData();
                sed->label = ent->obj->label;
                sed->layer = ent->layer;
                sed->pos = ent->obj->getPosition();
                sed->size = ent->obj->size;
                sed->rotation = ent->obj->rotation;
                sed->alpha = ent->obj->alpha;
                sed->texture = ent->obj->texture.name;
                sed->path = ent->movementPath;
                sceneEntitiesData.push_back(sed);
            }
            ld.sceneEntityData = sceneEntitiesData;
            // text
            std::vector<s2d::SceneTextEntityData *> sceneTextEntitiesData = {};
            for (TextEntity *ent : placedTextEntities)
            {
                s2d::SceneTextEntityData *tsed = new s2d::SceneTextEntityData();
                tsed->text = ent->text->text;
                tsed->layer = ent->layer;
                tsed->pos = ent->text->getPosition();
                tsed->color = ent->text->color;
                tsed->alpha = ent->text->alpha;
                tsed->textScale = ent->text->textScale;
                sceneTextEntitiesData.push_back(tsed);
            }
            ld.sceneTextEntityData = sceneTextEntitiesData;
            // particles
            std::vector<s2d::SceneParticleEntityData *> sceneParticleEntitiesData = {};
            for (ParticleEntity *ent : placedParticleEntities)
            {
                s2d::SceneParticleEntityData *psed = new s2d::SceneParticleEntityData();
                psed->layer = ent->layer;
                psed->pos = ent->pg->getPosition();
                psed->color = ent->pg->color;
                psed->amount = ent->pg->amount;
                psed->alpha = ent->pg->alpha;
                psed->direction = ent->pg->direction;
                psed->dispersion = ent->pg->dispersion;
                psed->scale = ent->pg->scale;
                psed->velocity = ent->pg->velocity;
                sceneParticleEntitiesData.push_back(psed);
            }
            ld.SceneParticleEntityData = sceneParticleEntitiesData;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

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

void Level::selectEdge()
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

void Level::moveEdge()
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

void Level::scaleEdges()
{
    edgeWidth = startEdgeWidth * scene->camera->zoom;
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

void Level::calculateMouseWorldPos()
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
    }
}

void Level::levelOptions()
{
    ImGui::InputText("Level name", name, 64);
    ImGui::Checkbox("Update level:", &updateLevel);
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
    // display mouse position
    std::string mousePos = "Mouse pos: ";
    mousePos += "(" + std::to_string((int)mouseWorldPos.x) + ", " + std::to_string((int)mouseWorldPos.y) + ")";
    ImGui::Text(mousePos.c_str());
    // display camera zoom
    ImGui::SliderFloat("Zoom", &scene->camera->zoom, 1.f, 50.0f);
    // Level size
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
    selectPlacementType();
    if (selectedPlacementType == "entity")
    {
        selectPlacementLayer();
        if (selectedPlacementLayer == "")
            return;
        if (ImGui::Checkbox("Place mode", &placeMode))
            selectedObject = nullptr;
        if (placeMode)
        {
            selectPlacementObjectType();
            if (selectedPlacementObjectType == "entity")
            {
                selectPlacementObject();
                ImGui::Checkbox("Place multiple", &placeMultiple);
                ImGui::SliderFloat("scale", &tmpScale, 0.f, 5.f);
                ImGui::SliderFloat("rotation", &tmpRotation, -3.14f, 3.14f);
                ImGui::SliderFloat("alpha", &tmpAlpha, 0.f, 1.f);
                ImGui::Button("Place");
            }
            else if (selectedPlacementObjectType == "text")
            {
                ImGui::InputText("text", tmpText, 64);
                ImGui::ColorEdit3("color", (float *)&tmpTextColor);
                ImGui::SliderFloat("scale", &tmpTextScale, 0.f, 5.f);
                if (ImGui::Button("Place"))
                {
                    placeMultiple = false;
                    createTextEntity();
                }
            }
            else if (selectedPlacementObjectType == "particles")
            {
                ImGui::SliderInt("number", &tmpParticleAmount, 0, 100);
                ImGui::ColorEdit3("color", (float *)&tmpParticleColor);
                ImGui::SliderFloat("direction", &tmpParticleDirection, -3.14, 3.14);
                ImGui::SliderFloat("dispersion", &tmpParticleDispersion, 0, 3.14);
                ImGui::SliderFloat("scale", &tmpParticleScale, 0, 100.f);
                ImGui::SliderFloat("velocity", &tmpParticleVelocity, 0.f, 10.f);
                if (ImGui::Button("Place"))
                {
                    placeMultiple = false;
                    createParticleEntity();
                }
            }
        }
        else
        {
            selectObject();
            if (selectedObject != NULL)
            {
                if (selectedObjectType == "entity")
                {
                    std::string objName = "Name: " + selectedObject->label;
                    ImGui::Text(objName.c_str());
                    std::string objPosText = "Position: ";
                    glm::vec2 objPos = selectedObject->getPosition();
                    objPosText += "(" + std::to_string((int)objPos.x) + ", " + std::to_string((int)objPos.y) + ")";
                    ImGui::Text(objPosText.c_str());
                    ImGui::SliderFloat("rotation", &selectedObject->rotation, -3.14f, 3.14f);
                    ImGui::SliderFloat("alpha", &selectedObject->alpha, 0.f, 1.f);
                    if (ImGui::Button("delete"))
                    {
                        deleteObject(selectedObject);
                        selectedObject = nullptr;
                    }
                }
                else if (selectedObjectType == "particles")
                {
                    s2d::ParticleGenerator *pg = (s2d::ParticleGenerator *)selectedObject;
                    ImGui::ColorEdit3("color", (float *)&selectedObject->color);
                    ImGui::SliderFloat("direction", &pg->direction, -3.14, 3.14);
                    ImGui::SliderFloat("dispersion", &pg->dispersion, 0, 3.14);
                    ImGui::SliderFloat("scale", &pg->scale, 0, 100.f);
                    ImGui::SliderFloat("velocity", &pg->velocity, 0.f, 10.f);
                    if (ImGui::Button("delete"))
                    {
                        deleteObject(selectedObject);
                        selectedObject = nullptr;
                    }
                }
                else if (selectedObjectType == "text")
                {
                    s2d::Text *te = (s2d::Text *)selectedObject;
                    // ImGui::InputText("text", te->text, 64);
                    ImGui::ColorEdit3("color", (float *)&te->color);
                    // ImGui::SliderFloat("scale", &te->textScale, 0.f, 5.f);
                    if (ImGui::Button("delete"))
                    {
                        deleteObject(selectedObject);
                        selectedObject = nullptr;
                    }
                }
            }
        }
    }
    else if (selectedPlacementType == "camera")
    {
        ImGui::SliderFloat("zoom", &tmpCameraZoom, 0.1f, 10.f);
        ImGui::Button("Place");
    }
    else if (selectedPlacementType == "zone")
    {
        ImGui::Button("Place");
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
    if (ImGui::BeginTable("level_layers", 5, tableFlags, tableSize))
    {
        ImGui::TableSetupColumn("name");
        ImGui::TableSetupColumn("position");
        ImGui::TableSetupColumn("depth");
        ImGui::TableSetupColumn("alpha");
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
                    if (row + 1 < layers.size())
                    {
                        move(layers, row, row + 1);
                        scene->layerStack->moveLayerUp(layers[row]->name);
                    }
                ImGui::SameLine();
                if (ImGui::SmallButton("-"))
                    if (row - 1 >= 0)
                    {
                        move(layers, row, row - 1);
                        scene->layerStack->moveLayerDown(layers[row]->name);
                    }
            }
            if (ImGui::TableSetColumnIndex(2))
                ImGui::SliderFloat("depth", &layers[row]->depth, 0.f, 1.0f);
            if (ImGui::TableSetColumnIndex(3))
                ImGui::SliderFloat("alpha", &layers[row]->alpha, 0.f, 1.0f);
            if (ImGui::TableSetColumnIndex(4))
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
            if (tmpNewTexture != "")
                ed->texture = rm->getTexture(std::string(tmpNewTexture));
            else
            {
                ResourceManager::TextureData td;
                ed->texture = td;
            }
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
            ResourceManager::TextureData td = rm->getTexture(std::string(tmpTextures[n]));
            float h = ImGui::GetTextLineHeight();
            ImGui::Image((void *)(intptr_t)td.textureID, ImVec2(h, h));
            ImGui::SameLine();
            if (ImGui::Selectable(tmpTextures[n], is_selected))
            {
                tmpNewTexture = tmpTextures[n];
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
                out += std::to_string((int)(ed->colour.r * 255)) + ", ";
                out += std::to_string((int)(ed->colour.g * 255)) + ", ";
                out += std::to_string((int)(ed->colour.b * 255));
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
                fs::path relPath = fs::relative(filename, currentPath);
                tmpTexturePath = relPath.generic_string();
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
    if (ImGui::BeginTable("textures_table", 4, tableFlags, tableSize))
    {
        ImGui::TableSetupColumn("image");
        ImGui::TableSetupColumn("name");
        ImGui::TableSetupColumn("size");
        ImGui::TableSetupColumn("path");

        ImGui::TableHeadersRow();

        for (int row = 0; row < textures.size(); row++)
        {
            ImGui::TableNextRow();
            ResourceManager::TextureData td = textures[row];
            if (ImGui::TableSetColumnIndex(0))
            {
                float h = ImGui::GetTextLineHeight();
                ImGui::Image((void *)(intptr_t)td.textureID, ImVec2(h, h));
            }
            if (ImGui::TableSetColumnIndex(1))
                ImGui::Text(td.name.c_str());
            if (ImGui::TableSetColumnIndex(2))
            {
                std::string out = "(";
                out += std::to_string((unsigned int)td.width);
                out += ", ";
                out += std::to_string((unsigned int)td.height);
                out += ")";
                ImGui::Text(out.c_str());
            }
            if (ImGui::TableSetColumnIndex(3))
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
    if (entityData->texture.name != "")
        placementObject->setTexture(entityData->texture);
    placementObject->alpha = tmpAlpha;
    placementObject->rotation = tmpRotation;
    placementObject->size *= tmpScale;
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
    Entity *e = new Entity(scene, placementObject, std::string(selectedPlacementLayer));
    placedEntities.push_back(e);
}

void Level::createTextEntity()
{
    placementObject = new s2d::Text(std::string(tmpText), false);
    placementObject->color = tmpTextColor;
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
    s2d::Text *t = (s2d::Text *)placementObject;
    t->textScale = glm::vec2(tmpTextScale);
    TextEntity *te = new TextEntity(scene, t, std::string(selectedPlacementLayer));
    placedTextEntities.push_back(te);
}

void Level::createParticleEntity()
{
    placementObject = new s2d::ParticleGenerator(tmpParticleAmount);
    placementObject->color = tmpParticleColor;
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
    s2d::ParticleGenerator *pg = (s2d::ParticleGenerator *)placementObject;
    pg->direction = tmpParticleDirection;
    pg->dispersion = tmpParticleDispersion;
    pg->scale = tmpParticleScale;
    pg->velocity = tmpParticleVelocity;
    ParticleEntity *pge = new ParticleEntity(scene, pg, std::string(selectedPlacementLayer));
    placedParticleEntities.push_back(pge);
}

void Level::selectPlacementType()
{
    if (ImGui::BeginCombo("Type", selectedPlacementType))
    {
        for (int n = 0; n < IM_ARRAYSIZE(placementTypes); n++)
        {
            bool is_selected = (selectedPlacementType == placementTypes[n]);
            if (ImGui::Selectable(placementTypes[n], is_selected))
                selectedPlacementType = placementTypes[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
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

void Level::selectPlacementObjectType()
{
    if (ImGui::BeginCombo("Object type", selectedPlacementObjectType))
    {
        for (int n = 0; n < IM_ARRAYSIZE(placementObjectTypes); n++)
        {
            bool is_selected = (selectedPlacementObjectType == placementObjectTypes[n]);
            if (ImGui::Selectable(placementObjectTypes[n], is_selected))
                selectedPlacementObjectType = placementObjectTypes[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void Level::deleteObject(s2d::Object *obj)
{
    if (selectedObjectType == "entity")
    {
        for (Entity *e : placedEntities)
        {
            if (obj == e->obj)
            {
                scene->removeChild(obj, e->layer);
                if (e->selected)
                    e->deselect();
                placedEntities.erase(
                    std::remove(placedEntities.begin(),
                                placedEntities.end(), e),
                    placedEntities.end());
                delete e;
                delete obj;
                break;
            }
        }
    }
    else if (selectedObjectType == "particles")
    {
        for (ParticleEntity *e : placedParticleEntities)
        {
            if (obj == e->pg)
            {
                scene->removeChild(obj, e->layer);
                placedParticleEntities.erase(
                    std::remove(placedParticleEntities.begin(),
                                placedParticleEntities.end(), e),
                    placedParticleEntities.end());
                delete e;
                delete obj;
                break;
            }
        }
    }
    else if (selectedObjectType == "text")
    {
        for (TextEntity *e : placedTextEntities)
        {
            if (obj == e->text)
            {
                scene->removeChild(obj, e->layer);
                placedTextEntities.erase(
                    std::remove(placedTextEntities.begin(),
                                placedTextEntities.end(), e),
                    placedTextEntities.end());
                delete e;
                delete obj;
                break;
            }
        }
    }
}

void Level::moveObject(s2d::Object *obj)
{
    glm::vec2 objectScreenPos = obj->getScreenPosition(
        true,
        scene->camera->getViewMatrix(),
        scene->camera->getProjectionMatrix(true),
        scene->windowWidth,
        scene->windowHeight);
    float dx = (sceneMousePos.x - objectScreenPos.x) * scene->camera->zoom;
    float dy = (sceneMousePos.y - objectScreenPos.y) * scene->camera->zoom;
    obj->move(dx, dy);
}

void Level::selectObject()
{
    float now = glfwGetTime();
    // regular entities
    for (Entity *e : placedEntities)
        if (e->layer == std::string(selectedPlacementLayer))
        {
            if (e->obj->getScreenBoundingBox(
                          scene->camera->getViewMatrix(),
                          scene->camera->getProjectionMatrix(true),
                          scene->camera->zoom,
                          scene->windowWidth,
                          scene->windowHeight)
                    .intersectsPoint(sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                e->obj->outline = true;
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1) && (now - tmpTimer > 0.5f))
                {
                    tmpTimer = now;
                    selectedObject = e->obj;
                    selectedObjectType = "entity";
                    e->select();
                    break;
                }
            }
            else
                e->obj->outline = false;
        }
    // text entities
    for (TextEntity *te : placedTextEntities)
        if (te->layer == std::string(selectedPlacementLayer))
        {
            s2d::col::BoundingBox bbox = te->text->getScreenBoundingBox(
                scene->camera->getViewMatrix(),
                scene->camera->getProjectionMatrix(true),
                scene->camera->zoom,
                scene->windowWidth,
                scene->windowHeight);
            bbox.inflate(100.f, 100.f, true);
            if (bbox.intersectsPoint(sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1) && (now - tmpTimer > 0.5f))
                {
                    tmpTimer = now;
                    selectedObject = te->text;
                    selectedObjectType = "text";
                    break;
                }
            }
        }
    // particle entities
    for (ParticleEntity *pe : placedParticleEntities)
        if (pe->layer == std::string(selectedPlacementLayer))
        {
            s2d::col::BoundingBox bbox = pe->pg->getScreenBoundingBox(
                scene->camera->getViewMatrix(),
                scene->camera->getProjectionMatrix(true),
                scene->camera->zoom,
                scene->windowWidth,
                scene->windowHeight);
            bbox.inflate(100.f, 100.f, true);
            if (bbox.intersectsPoint(sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1) && (now - tmpTimer > 0.5f))
                {
                    tmpTimer = now;
                    selectedObject = pe->pg;
                    selectedObjectType = "particles";
                    break;
                }
            }
        }

    if (selectedObject != NULL)
        selectedObject->outline = true;
}

void Level::createObjectPath()
{
    float now = glfwGetTime();
    if (scene->window->isKeyPressed(GLFW_KEY_A) && (now - objectPathTimer > 0.5f))
        for (Entity *e : placedEntities)
            if (selectedObject == e->obj)
            {
                objectPathTimer = now;
                e->movementPath.push_back(mouseWorldPos);
                break;
            }
}
