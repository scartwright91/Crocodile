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

        std::vector<EarthSurface*> m_earthSurfaces = {};
        std::vector<WaterSurface*> m_waterSurfaces = {};

        // water rendering
        graphics::FrameBuffer* m_reflectionFB = nullptr;
        graphics::FrameBuffer* m_refractionFB = nullptr;

        void render() override;
        void updateTimer(float dt);

        float m_timer = 0.0f;

};

