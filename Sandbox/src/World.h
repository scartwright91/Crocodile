#pragma once

#include "Scene.h"

using namespace Crocodile;

class World
{

    public:
        World(Scene* scene);
        ~World();

        void update(float dt);

    private:
        Scene* scene = nullptr;

};
