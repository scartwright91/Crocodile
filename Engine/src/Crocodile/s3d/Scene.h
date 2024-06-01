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
                virtual void render();
                void resize();
                
                void addObject(Object* obj);
                void removeObject(Object* obj);

                CameraController* camera = nullptr;

                // lighting
                float ambientLighting = .4f;
                glm::vec3 lightPosition = glm::vec3(50.f);
                glm::vec3 lightColour = glm::vec3(1.f);

                std::vector<Object*> objects = {};
                std::vector<Surface*> surfaces = {};
                Renderer* renderer = nullptr;

            protected:
                graphics::Window* window = nullptr;
                ResourceManager *resourceManager = nullptr;

            private:
                void init();

        };

    }
}