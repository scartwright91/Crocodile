#include "ParticleEntity.h"

ParticleEntity::ParticleEntity(s2d::Scene *scene, s2d::ParticleGenerator *pg, std::string layer) : scene(scene), pg(pg), layer(layer)
{
}

void ParticleEntity::update()
{
    pg->update(1.f / 60.f);
}