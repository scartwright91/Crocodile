#include "Level.h"

Level::Level(LevelData data, s2d::Scene *scene, ResourceManager *rm) : scene(scene), rm(rm), currentPath(fs::current_path())
{
    strcpy(name, data.name.c_str());
    canvas = new Canvas(scene, data.canvasSize, data.canvasColor);
    entityManager = new EntityManager(scene, rm, canvas);
    entitiesData = data.entitiesData;
    layers = data.layers;
    for (s2d::Layer *l : layers)
        scene->layerStack->addLayer(l);
    textures = data.textures;
    for (ResourceManager::TextureData td : textures)
        rm->loadTexture(td.path.c_str(), td.name, false);
    loadPlacedEntities(data);
}

Level::Level(
    std::string name,
    s2d::Scene *scene,
    ResourceManager *rm,
    glm::vec2 canvasPos,
    glm::vec2 canvasSize) : scene(scene), rm(rm), currentPath(fs::current_path())
{
    strcpy(this->name, name.c_str());
    canvas = new Canvas(scene, canvasPos, canvasSize);
    entityManager = new EntityManager(scene, rm, canvas);
}

Level::~Level()
{
    delete canvas;
    delete entityManager;
    for (s2d::Layer *layer : layers)
    {
        scene->layerStack->removeLayer(layer);
        delete layer;
    }
    scene->removeChild(levelName, "canvas");
    delete levelName;
}

void Level::createLevelName()
{
    delete levelName;
    levelName = new s2d::Text();
    levelName->color = glm::vec3(1.f);
    levelName->setScale(glm::vec2(2.f));
    levelName->setText(std::string(name), false);
    levelName->setPosition(canvas->canvas->getCenteredPosition() - levelName->size / 2.f);
    scene->removeChild(levelName, "canvas");
    scene->addChild(levelName, "canvas");
}

void Level::load()
{
    entityManager->load();
    for (s2d::Layer *layer : layers)
        scene->layerStack->addLayer(layer);
    canvas->showGrid = true;
    canvas->grid->show();
    scene->removeChild(levelName, "canvas");
}

void Level::clear()
{
    entityManager->clear();
    for (s2d::Layer *layer : layers)
        scene->layerStack->removeLayer(layer);
    canvas->showGrid = false;
    canvas->grid->hide();
    scene->addChild(levelName, "canvas");
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
        obj->color = sed->color;
        if (sed->texture != "")
            obj->setTexture(rm->getTexture(sed->texture));
        Entity *e = new Entity(scene, obj, sed->layer);
        e->movementPath = sed->path;
        entityManager->placedEntities.push_back(e);
    }
    for (s2d::SceneTextEntityData *ted : data.sceneTextEntityData)
    {
        s2d::Text *t = new s2d::Text(ted->text, false);
        t->alpha = ted->alpha;
        t->setPosition(ted->pos);
        t->color = ted->color;
        t->setScale(ted->textScale);
        TextEntity *te = new TextEntity(scene, t, ted->layer);
        entityManager->placedTextEntities.push_back(te);
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
        if (ped->texture != "")
            pg->setTexture(rm->getTexture(ped->texture));
        ParticleEntity *pe = new ParticleEntity(scene, pg, ped->layer);
        entityManager->placedParticleEntities.push_back(pe);
    }
}

void Level::update(float dt, glm::vec2 mouse)
{
    canvas->update(dt, mouse);
    entityManager->update(dt, updateLevel, selectedPlacementLayer, selectedPlacementObjectType);
}

LevelData Level::serialise()
{
    LevelData ld = {};
    ld.name = std::string(name);
    ld.canvasPos = canvas->canvas->getPosition();
    ld.canvasSize = canvas->canvas->size;
    ld.canvasColor = canvas->canvasColour;
    ld.entitiesData = entitiesData;
    ld.layers = layers;
    ld.textures = textures;
    try
    {
        {
            // entities
            std::vector<s2d::SceneEntityData *> sceneEntitiesData = {};
            for (Entity *ent : entityManager->placedEntities)
            {
                s2d::SceneEntityData *sed = new s2d::SceneEntityData();
                sed->label = ent->obj->label;
                sed->layer = ent->layer;
                sed->pos = ent->obj->getPosition();
                sed->size = ent->obj->size;
                sed->color = ent->obj->color;
                sed->rotation = ent->obj->rotation;
                sed->alpha = ent->obj->alpha;
                sed->texture = ent->obj->texture.name;
                sed->path = ent->movementPath;
                sceneEntitiesData.push_back(sed);
            }
            ld.sceneEntityData = sceneEntitiesData;
            // text
            std::vector<s2d::SceneTextEntityData *> sceneTextEntitiesData = {};
            for (TextEntity *ent : entityManager->placedTextEntities)
            {
                s2d::SceneTextEntityData *tsed = new s2d::SceneTextEntityData();
                tsed->text = ent->text->text;
                tsed->layer = ent->layer;
                tsed->pos = ent->text->getPosition();
                tsed->color = ent->text->color;
                tsed->alpha = ent->text->alpha;
                tsed->textScale = ent->text->getScale();
                sceneTextEntitiesData.push_back(tsed);
            }
            ld.sceneTextEntityData = sceneTextEntitiesData;
            // particles
            std::vector<s2d::SceneParticleEntityData *> sceneParticleEntitiesData = {};
            for (ParticleEntity *ent : entityManager->placedParticleEntities)
            {
                s2d::SceneParticleEntityData *psed = new s2d::SceneParticleEntityData();
                psed->layer = ent->layer;
                psed->pos = ent->pg->getPosition();
                psed->color = ent->pg->color;
                psed->texture = ent->pg->texture.name;
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

void Level::levelOptions()
{
    ImGui::Checkbox("Update level:", &updateLevel);
    canvas->renderImGui();
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
    mousePos += "(" + std::to_string((int)canvas->levelMousePos.x) + ", " + std::to_string((int)canvas->levelMousePos.y) + ")";
    ImGui::Text(mousePos.c_str());
    // grid position
    std::string gridPos = "Grid pos: ";
    gridPos += "(" + std::to_string((int)canvas->levelMousePosGrid.x) + ", " + std::to_string((int)canvas->levelMousePosGrid.y) + ")";
    ImGui::Text(gridPos.c_str());
    // display camera zoom
    ImGui::SliderFloat("Zoom", &scene->camera->zoom, 1.f, 50.0f);
    // Level size
    glm::vec2 size = canvas->canvas->size;
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
    selectPlacementLayer();
    if (entityManager->selectedPlacementLayer == "")
        return;
    if (ImGui::Checkbox("Place mode", &placeMode))
        entityManager->selectedObject = nullptr;
    if (placeMode)
    {
        selectPlacementObjectType();
        ImGui::Checkbox("Snap to grid", &entityManager->snapToGrid);
        if (entityManager->selectedPlacementObjectType == "entity")
        {
            selectPlacementObject();
            ImGui::Checkbox("Place multiple", &entityManager->placeMultiple);
            ImGui::SliderFloat("scale", &entityManager->tmpScale, 0.f, 5.f);
            ImGui::SliderFloat("rotation", &entityManager->tmpRotation, -3.14f, 3.14f);
            ImGui::SliderFloat("alpha", &entityManager->tmpAlpha, 0.f, 1.f);
            ImGui::Button("Place");
        }
        else if (entityManager->selectedPlacementObjectType == "text")
        {
            ImGui::InputText("text", entityManager->tmpText, 64);
            ImGui::ColorEdit3("color", (float *)&entityManager->tmpTextColor);
            ImGui::SliderFloat("scale", &entityManager->tmpTextScale, 0.f, 20.f);
            if (ImGui::Button("Place"))
            {
                entityManager->placeMultiple = false;
                entityManager->createTextEntity();
            }
        }
        else if (entityManager->selectedPlacementObjectType == "particles")
        {
            ImGui::SliderInt("number", &entityManager->tmpParticleAmount, 0, 100);
            ImGui::ColorEdit3("color", (float *)&entityManager->tmpParticleColor);
            selectParticleTexture();
            ImGui::SliderFloat("direction", &entityManager->tmpParticleDirection, -3.14, 3.14);
            ImGui::SliderFloat("dispersion", &entityManager->tmpParticleDispersion, 0, 3.14);
            ImGui::SliderFloat("scale", &entityManager->tmpParticleScale, 0, 100.f);
            ImGui::SliderFloat("velocity", &entityManager->tmpParticleVelocity, 0.f, 10.f);
            if (ImGui::Button("Place"))
            {
                entityManager->placeMultiple = false;
                entityManager->createParticleEntity();
            }
        }
        else if (entityManager->selectedPlacementObjectType == "light")
        {
            ImGui::ColorEdit3("color", (float *)&entityManager->tmpParticleColor);
            ImGui::SliderFloat("radius", &entityManager->tmpParticleDirection, -3.14, 3.14);
            ImGui::Button("Place");
        }
        else if (entityManager->selectedPlacementObjectType == "zone")
        {
            ImGui::Text("Zones are created by dragging and dropping");
            ImGui::InputText("Name", entityManager->tmpText, 64);
            ImGui::ColorEdit3("color", (float *)&entityManager->tmpParticleColor);
            ImGui::SliderFloat("alpha", &entityManager->tmpParticleDirection, -3.14, 3.14);
        }
    }
    else
    {
        if (entityManager->selectedObject != NULL)
        {
            if (entityManager->selectedObjectType == "entity")
            {
                std::string objName = "Name: " + entityManager->selectedObject->label;
                ImGui::Text(objName.c_str());
                std::string objPosText = "Position: ";
                glm::vec2 objPos = entityManager->selectedObject->getPosition();
                objPosText += "(" + std::to_string((int)objPos.x) + ", " + std::to_string((int)objPos.y) + ")";
                ImGui::Text(objPosText.c_str());
                ImGui::SliderFloat("rotation", &entityManager->selectedObject->rotation, -3.14f, 3.14f);
                ImGui::SliderFloat("alpha", &entityManager->selectedObject->alpha, 0.f, 1.f);
                if (ImGui::Button("delete"))
                {
                    entityManager->deleteObject(entityManager->selectedObject);
                    entityManager->selectedObject = nullptr;
                }
            }
            else if (entityManager->selectedObjectType == "particles")
            {
                s2d::ParticleGenerator *pg = (s2d::ParticleGenerator *)entityManager->selectedObject;
                ImGui::ColorEdit3("color", (float *)&entityManager->selectedObject->color);
                selectParticleTexture();
                ImGui::SliderFloat("direction", &pg->direction, -3.14, 3.14);
                ImGui::SliderFloat("dispersion", &pg->dispersion, 0, 3.14);
                ImGui::SliderFloat("scale", &pg->scale, 0, 100.f);
                ImGui::SliderFloat("velocity", &pg->velocity, 0.f, 10.f);
                if (ImGui::Button("delete"))
                {
                    entityManager->deleteObject(entityManager->selectedObject);
                    entityManager->selectedObject = nullptr;
                }
            }
            else if (entityManager->selectedObjectType == "text")
            {
                s2d::Text *te = (s2d::Text *)entityManager->selectedObject;
                // std::string t{te->text};
                // ImGui::InputText("text", &t, 64);
                ImGui::ColorEdit3("color", (float *)&te->color);
                float ts = te->getScale().x;
                ImGui::SliderFloat("scale", &ts, 0.f, 20.f);
                te->setScale(glm::vec2(ts));
                if (ImGui::Button("delete"))
                {
                    entityManager->deleteObject(entityManager->selectedObject);
                    entityManager->selectedObject = nullptr;
                }
            }
        }
        else
        {
            entityManager->selectObject();
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
                ImGui::SliderFloat("depth", &layers[row]->depth, -1.f, 1.0f);
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
    std::vector<const char *> tmpTextures = {};

    for (int i = 0; i < textures.size(); i++)
        tmpTextures.push_back(textures[i].name.c_str());

    if (ImGui::BeginCombo("##combo", tmpNewTexture))
    {
        for (int n = 0; n < tmpTextures.size(); n++)
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

void Level::selectParticleTexture()
{
    std::vector<const char *> tmpTextures = {};

    for (int i = 0; i < textures.size(); i++)
        tmpTextures.push_back(textures[i].name.c_str());

    if (ImGui::BeginCombo("##combo", entityManager->tmpParticleTextureName))
    {
        for (int n = 0; n < tmpTextures.size(); n++)
        {
            bool is_selected = (entityManager->tmpParticleTextureName == tmpTextures[n]);
            ResourceManager::TextureData td = rm->getTexture(std::string(tmpTextures[n]));
            float h = ImGui::GetTextLineHeight();
            ImGui::Image((void *)(intptr_t)td.textureID, ImVec2(h, h));
            ImGui::SameLine();
            if (ImGui::Selectable(tmpTextures[n], is_selected))
            {
                entityManager->tmpParticleTextureName = tmpTextures[n];
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

void Level::selectPlacementLayer()
{
    std::vector<const char *> tmpLayers = {};
    for (int i = 0; i < layers.size(); i++)
        tmpLayers.push_back(layers[i]->name.c_str());

    if (ImGui::BeginCombo("Select layer", selectedPlacementLayer))
    {
        for (int n = 0; n < tmpLayers.size(); n++)
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
    std::vector<const char *> tmpEntities = {};
    for (int i = 0; i < entitiesData.size(); i++)
        tmpEntities.push_back(entitiesData[i]->label.c_str());

    if (ImGui::BeginCombo("Select entity", tmpPlacementEntity))
    {
        for (int n = 0; n < tmpEntities.size(); n++)
        {
            bool is_selected = (tmpPlacementEntity == tmpEntities[n]);
            if (ImGui::Selectable(tmpEntities[n], is_selected))
            {
                tmpPlacementEntity = tmpEntities[n];
                if (selectedPlacementLayer != "")
                {
                    entityManager->selectedEntityData = entitiesData[n];
                    entityManager->createEntityFromData(entityManager->selectedEntityData);
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
