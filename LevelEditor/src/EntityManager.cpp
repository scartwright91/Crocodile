#include "EntityManager.h"

EntityManager::EntityManager(s2d::Scene *scene, ResourceManager *rm, Canvas *canvas) : scene(scene), rm(rm), canvas(canvas)
{
}

EntityManager::~EntityManager()
{
    for (Entity *e : placedEntities)
        delete e;
    for (TextEntity *te : placedTextEntities)
        delete te;
    for (ParticleEntity *pe : placedParticleEntities)
        delete pe;
    for (ConnectionEntity *ce : placedConnectionEntities)
        delete ce;
}

void EntityManager::move(float dx, float dy)
{
    for (Entity *e : placedEntities)
        e->obj->move(dx, dy);
    for (TextEntity *te : placedTextEntities)
        te->text->move(dx, dy);
    for (ParticleEntity *pe : placedParticleEntities)
        pe->pg->move(dx, dy);
    for (ConnectionEntity *ce : placedConnectionEntities)
    {
        ce->collision->move(dx, dy);
        ce->spawn->move(dx, dy);
    }
}

void EntityManager::load()
{
    for (Entity *e : placedEntities)
        scene->addChild(e->obj, e->layer);
    for (TextEntity *te : placedTextEntities)
        scene->addChild(te->text, te->layer);
    for (ParticleEntity *pe : placedParticleEntities)
        scene->addChild(pe->pg, pe->layer);
    for (ConnectionEntity *ce : placedConnectionEntities)
    {
        scene->addChild(ce->collision, ce->layer);
        scene->addChild(ce->spawn, ce->layer);
    }
}

void EntityManager::clear()
{
    for (Entity *e : placedEntities)
        scene->removeChild(e->obj, e->layer);
    for (TextEntity *te : placedTextEntities)
        scene->removeChild(te->text, te->layer);
    for (ParticleEntity *pe : placedParticleEntities)
        scene->removeChild(pe->pg, pe->layer);
    for (ConnectionEntity *ce : placedConnectionEntities)
    {
        scene->removeChild(ce->collision, ce->layer);
        scene->removeChild(ce->spawn, ce->layer);
    }
}

void EntityManager::update(
    float dt,
    bool updateLevel,
    const char *selectedPlacementLayer,
    const char *selectedPlacementObjectType)
{
    this->selectedPlacementLayer = selectedPlacementLayer;
    this->selectedPlacementObjectType = selectedPlacementObjectType;
    bool leftclick = scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1);
    bool rightclick = scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2);
    float now = glfwGetTime();
    if (updateLevel)
    {
        // update particles
        for (ParticleEntity *pg : placedParticleEntities)
            pg->update();
    }
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
            deleteObject(placementObject);
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
            for (TextEntity *t : placedTextEntities)
                if (t->text == selectedObject)
                {
                    t->deselect();
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

void EntityManager::serialise()
{
    std::cout << "Serialise entities" << std::endl;
}

void EntityManager::createEntityFromData(s2d::EntityData *entityData)
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

void EntityManager::createTextEntity()
{
    placementObject = new s2d::Text(std::string(tmpText), false);
    placementObject->color = tmpTextColor;
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
    s2d::Text *t = (s2d::Text *)placementObject;
    t->setScale(glm::vec2(tmpTextScale));
    TextEntity *te = new TextEntity(scene, t, std::string(selectedPlacementLayer));
    placedTextEntities.push_back(te);
}

void EntityManager::createParticleEntity()
{
    placementObject = new s2d::ParticleGenerator(tmpParticleAmount);
    placementObject->color = tmpParticleColor;
    placementObject->setTexture(rm->getTexture(std::string(tmpParticleTextureName)));
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
    s2d::ParticleGenerator *pg = (s2d::ParticleGenerator *)placementObject;
    pg->direction = tmpParticleDirection;
    pg->dispersion = tmpParticleDispersion;
    pg->scale = tmpParticleScale;
    pg->velocity = tmpParticleVelocity;
    ParticleEntity *pge = new ParticleEntity(scene, pg, std::string(selectedPlacementLayer));
    placedParticleEntities.push_back(pge);
}

void EntityManager::createConnectionEntity()
{
    placementObject = new s2d::Object();
    placementObject->color = glm::vec3(02, 0.2, 0.8);
    placementObject->alpha = 0.5f;
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
    placementObject->size = glm::vec2(tmpConnectionColWidth, tmpConnectionColHeight);
    ConnectionEntity *ce = new ConnectionEntity(
        scene,
        std::string(tmpConnectionLevelName),
        std::string(selectedPlacementLayer),
        placementObject);
    scene->addChild(ce->spawn, ce->layer);
    placedConnectionEntities.push_back(ce);
}

void EntityManager::selectObject()
{
    float now = glfwGetTime();
    hoveredObject = nullptr;
    // regular entities
    for (Entity *e : placedEntities)
        if (e->layer == std::string(selectedPlacementLayer))
        {
            if (e->obj->getScreenBoundingBox(
                          scene->camera->getViewMatrix(1.),
                          scene->camera->getProjectionMatrix(true),
                          scene->camera->zoom,
                          scene->windowWidth,
                          scene->windowHeight,
                          1.0)
                    .intersectsPoint(canvas->sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                e->obj->outline = true;
                hoveredObject = e->obj;
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
                scene->camera->getViewMatrix(1.),
                scene->camera->getProjectionMatrix(true),
                scene->camera->zoom,
                scene->windowWidth,
                scene->windowHeight,
                1.0);
            if (bbox.intersectsPoint(canvas->sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                hoveredObject = te->text;
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1) && (now - tmpTimer > 0.5f))
                {
                    tmpTimer = now;
                    selectedObject = te->text;
                    selectedObjectType = "text";
                    te->select();
                    break;
                }
            }
        }
    // particle entities
    for (ParticleEntity *pe : placedParticleEntities)
        if (pe->layer == std::string(selectedPlacementLayer))
        {
            s2d::col::BoundingBox bbox = pe->pg->getScreenBoundingBox(
                scene->camera->getViewMatrix(1.),
                scene->camera->getProjectionMatrix(true),
                scene->camera->zoom,
                scene->windowWidth,
                scene->windowHeight,
                1.0);
            bbox.inflate(100.f, 100.f, true);
            if (bbox.intersectsPoint(canvas->sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                hoveredObject = pe->pg;
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1) && (now - tmpTimer > 0.5f))
                {
                    tmpTimer = now;
                    selectedObject = pe->pg;
                    selectedObjectType = "particles";
                    break;
                }
            }
        }
    for (ConnectionEntity *ce : placedConnectionEntities)
        if (ce->layer == std::string(selectedPlacementLayer))
        {
            s2d::col::BoundingBox bbox = ce->collision->getScreenBoundingBox(
                scene->camera->getViewMatrix(1.),
                scene->camera->getProjectionMatrix(true),
                scene->camera->zoom,
                scene->windowWidth,
                scene->windowHeight,
                1.0);
            if (bbox.intersectsPoint(canvas->sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                hoveredObject = ce->collision;
                if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1) && (now - tmpTimer > 0.5f))
                {
                    tmpTimer = now;
                    selectedObject = ce->collision;
                    selectedConnectionEntity = ce;
                    selectedObjectType = "connection";
                    break;
                }
            }
        }

    if (selectedObject != NULL)
        selectedObject->outline = true;
    if (hoveredObject != NULL && scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2))
        deleteObject(hoveredObject);
}

void EntityManager::deleteObject(s2d::Object *obj)
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
    else if (selectedObjectType == "connection")
    {
        for (ConnectionEntity *e : placedConnectionEntities)
        {
            if (selectedConnectionEntity == e)
            {
                scene->removeChild(e->collision, e->layer);
                scene->removeChild(e->spawn, e->layer);
                placedConnectionEntities.erase(
                    std::remove(placedConnectionEntities.begin(),
                                placedConnectionEntities.end(), e),
                    placedConnectionEntities.end());
                delete e;
                break;
            }
        }
    }
}

void EntityManager::moveObject(s2d::Object *obj)
{
    obj->setPosition(canvas->getWorldPosition(snapToGrid));
}

void EntityManager::createObjectPath()
{
    float now = glfwGetTime();
    if (scene->window->isKeyPressed(GLFW_KEY_Z) && (now - objectPathTimer > 0.5f))
        for (Entity *e : placedEntities)
            if (selectedObject == e->obj)
            {
                objectPathTimer = now;
                e->addMovementPathPos(canvas->getWorldPosition(false));
                break;
            }
}
