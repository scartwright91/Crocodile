#include "Entity.h"

Entity::Entity(s2d::Object *obj, std::string layer) : obj(obj), layer(layer)
{
}

void Entity::select(s2d::Scene *scene)
{
    selected = true;
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

void Entity::deselect(s2d::Scene *scene)
{
    selected = false;
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
