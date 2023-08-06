#include "ZoneEntity.h"

ZoneEntity::ZoneEntity(s2d::Scene *scene) : scene(scene)
{
    zone = new s2d::Object();
    zone->color = color;
}

ZoneEntity::~ZoneEntity()
{
}