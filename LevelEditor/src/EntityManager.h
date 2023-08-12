#pragma once

#include "Crocodile/s2d/Scene.h"

#include "Canvas.h"
#include "entities/Entity.h"
#include "entities/ParticleEntity.h"
#include "entities/TextEntity.h"

using namespace Crocodile;

class EntityManager
{
public:
    EntityManager(s2d::Scene *scene, Canvas *canvas);
    ~EntityManager();

    void update(float dt, bool updateLevel);
    void serialise();
    void selectObject();
    void deleteObject(s2d::Object *obj);
    void moveObject(s2d::Object *obj);

    // placement
    float placementTimer = glfwGetTime();
    const char *selectedPlacementObjectType = "entity";
    const char *selectedPlacementLayer = "";
    const char *tmpPlacementEntity = "";
    bool placeMultiple = true;
    bool snapToGrid = true;

    // creating entities
    s2d::Object *placementObject = nullptr;
    s2d::EntityData *selectedEntityData;
    void createEntityFromData(s2d::EntityData *entityData);
    void createTextEntity();
    void createParticleEntity();

    // selecting entities
    s2d::Object *selectedObject = nullptr;
    std::string selectedObjectType = "entity";
    bool moveSelectedObject = false;

    std::vector<Entity *> placedEntities = {};
    std::vector<TextEntity *> placedTextEntities = {};
    std::vector<ParticleEntity *> placedParticleEntities = {};

private:
    s2d::Scene *scene = nullptr;
    Canvas *canvas = nullptr;

    float tmpTimer = 0.f;

    // ----- Parameters -----

    // entity inputs
    float tmpScale = 1.f;
    float tmpAlpha = 1.f;
    float tmpRotation = 0.f;
    // text inputs
    char tmpText[64] = "";
    glm::vec3 tmpTextColor = glm::vec3(1.f);
    float tmpTextScale = 1.f;
    // particle inputs
    int tmpParticleAmount = 10;
    glm::vec3 tmpParticleColor = glm::vec3(1.f);
    float tmpParticleDirection = 0.f;
    float tmpParticleDispersion = 0.1f;
    float tmpParticleScale = 10.f;
    float tmpParticleVelocity = 1.f;

    float objectPathTimer = 0.f;
    std::vector<s2d::Object *> objectPath = {};
    void createObjectPath();
};