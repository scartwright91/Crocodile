#pragma once


#include "Crocodile/s3d/Scene.h"
#include "Crocodile/s3d/Surface.h"

#include "WaterSurface.h"
#include "EarthSurface.h"

using namespace Crocodile;

class Scene : public s3d::Scene
{

    public:
        Scene(graphics::Window *window, ResourceManager *resourceManager);
        ~Scene();

        std::vector<EarthSurface*> earthSurfaces = {};
        std::vector<WaterSurface*> waterSurfaces = {};

        // water rendering
        graphics::FrameBuffer* reflectionFB = nullptr;
        graphics::FrameBuffer* refractionFB = nullptr;

        void render() override;
        void updateTimer(float dt);

        float timer = 0.0f;

};

