#pragma once

#include "Crocodile/s3d/Surface.h"

using namespace Crocodile;

class WaterSurface : s3d::Surface
{
    public:
        WaterSurface(s3d::HeightMap heightMap, graphics::Shader* shader);
        ~WaterSurface();

        void customRender();
};
