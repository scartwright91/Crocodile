#pragma once

#include "../Core.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Scene
        {
            public:
                Scene();
                ~Scene();

                void update(float dt);
                void render();
            private:

        };

    }
}