#pragma once


#include "Crocodile/s3d/Scene.h"
#include "Crocodile/s3d/Surface.h"

#include "WaterSurface.h"

using namespace Crocodile;

class Scene : public s3d::Scene
{

    public:
        Scene(graphics::Window *window, ResourceManager *resourceManager);
        ~Scene();

        std::vector<s3d::Surface*> earthSurfaces = {};
        std::vector<WaterSurface*> waterSurfaces = {};

        void render() override;

};

