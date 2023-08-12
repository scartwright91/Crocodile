#include "EntityManager.h"

EntityManager::EntityManager(s2d::Scene *scene, Canvas *canvas) : scene(scene), canvas(canvas)
{
}

EntityManager::~EntityManager()
{
}

void EntityManager::update(float dt)
{
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
    scene->addChild(placementObject, std::string(selectedPlacementLayer));
    s2d::ParticleGenerator *pg = (s2d::ParticleGenerator *)placementObject;
    pg->direction = tmpParticleDirection;
    pg->dispersion = tmpParticleDispersion;
    pg->scale = tmpParticleScale;
    pg->velocity = tmpParticleVelocity;
    ParticleEntity *pge = new ParticleEntity(scene, pg, std::string(selectedPlacementLayer));
    placedParticleEntities.push_back(pge);
}

void EntityManager::selectObject()
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
                    .intersectsPoint(canvas->sceneMousePos))
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
            if (bbox.intersectsPoint(canvas->sceneMousePos))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
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
                scene->camera->getViewMatrix(),
                scene->camera->getProjectionMatrix(true),
                scene->camera->zoom,
                scene->windowWidth,
                scene->windowHeight);
            bbox.inflate(100.f, 100.f, true);
            if (bbox.intersectsPoint(canvas->sceneMousePos))
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
}

void EntityManager::moveObject(s2d::Object *obj)
{
    if (snapToGrid)
        obj->setPosition(canvas->getWorldGridPosition());
    else
        obj->setPosition(canvas->mouseWorldPos);
}
