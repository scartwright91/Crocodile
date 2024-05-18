#pragma once

#include <vector>
#include "../Core.h"
#include "../graphics/Window.h"
#include "../ResourceManager.h"

#include "Object.h"
#include "Surface.h"
#include "rendering/Renderer.h"
#include "Camera.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Scene
        {
            public:
                Scene(graphics::Window *window, ResourceManager *resourceManager);
                ~Scene();

                void update(float dt);
                void render();
                
                void addObject(Object* obj);
                void removeObject(Object* obj);

                Camera* camera = nullptr;

                std::vector<Object*> objects = {};
                std::vector<Surface*> surfaces = {};
                Renderer* renderer = nullptr;

            private:

                graphics::Window* window = nullptr;
                ResourceManager *resourceManager = nullptr;

                void init();

        };

    }
}