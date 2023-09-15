#include "Entity.h"

Entity::Entity(s2d::Scene *scene, s2d::Object *obj, std::string layer) : scene(scene), obj(obj), layer(layer)
{
    label = new s2d::Text(obj->label, false);
    label->setPosition(obj->getPosition());
    label->color = glm::vec3(1.f);
    label->alpha = 0.f;
}

void Entity::select()
{
    selected = true;
    label->setPosition(obj->getPosition());
    obj->showBoundingBox = true;
    label->alpha = 1.f;
    for (glm::vec2 p : movementPath)
    {
        s2d::Object *obj = new s2d::Object();
        obj->setPosition(p);
        obj->size = glm::vec2(25.f);
        obj->color = glm::vec3(1.f, 0.f, 0.f);
        scene->addChild(obj, layer);
        tmpMarkers.push_back(obj);
    }
    for (int i = 1; i < movementPath.size(); i++)
    {
        s2d::shapes::Line *line = new s2d::shapes::Line(movementPath[i - 1], movementPath[i]);
        line->color = glm::vec3(1.f, 0.f, 0.f);
        scene->addChild(line, layer);
        tmpLines.push_back(line);
    }
}

void Entity::deselect()
{
    selected = false;
    obj->showBoundingBox = false;
    label->alpha = 0.f;
    for (s2d::Object *obj : tmpMarkers)
    {
        scene->removeChild(obj, layer);
        delete obj;
    }
    tmpMarkers.clear();
    for (s2d::shapes::Line *line : tmpLines)
    {
        scene->removeChild(line, layer);
        delete line;
    }
    tmpLines.clear();
}

void Entity::addMovementPathPos(glm::vec2 pos)
{
    movementPath.push_back(pos);

    // Add new marker
    s2d::Object *obj = new s2d::Object();
    obj->setPosition(pos);
    obj->size = glm::vec2(25.f);
    obj->color = glm::vec3(1.f, 0.f, 0.f);
    scene->addChild(obj, layer);
    tmpMarkers.push_back(obj);

    // add new line
    unsigned int n = movementPath.size() - 1;
    if (n >= 1)
    {
        s2d::shapes::Line *line = new s2d::shapes::Line(movementPath[n - 1], movementPath[n]);
        line->color = glm::vec3(1.f, 0.f, 0.f);
        scene->addChild(line, layer);
        tmpLines.push_back(line);
    }
}
