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

                CameraController* m_camera = nullptr;

                // lighting
                float m_ambientLighting = .4f;
                glm::vec3 m_lightPosition = glm::vec3(50.f);
                glm::vec3 m_lightColour = glm::vec3(1.f);

                std::vector<Object*> m_objects = {};
                std::vector<Surface*> m_surfaces = {};
                Renderer* m_renderer = nullptr;

            protected:
                graphics::Window* m_window = nullptr;
                ResourceManager* m_resourceManager = nullptr;

            private:
                void init();

        };

    }
}