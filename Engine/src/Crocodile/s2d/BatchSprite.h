#pragma once

#include "../Core.h"

#include "Object.h"
#include "rendering/BatchRenderer.h"
#include "../ResourceManager.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API BatchSprite : public Object
        {
        public:
            BatchSprite(
                graphics::Shader *shader,
                std::vector<glm::vec2> positions,
                std::vector<glm::vec2> tilesetPositions,
                glm::vec2 size,
                float tilesetWidth,
                float tilesetHeight);
            ~BatchSprite();

            void render(
                glm::vec2 viewportScale,
                glm::mat4 view,
                glm::mat4 projection,
                TextureData texture,
                float alpha,
                float ambientLighting,
                std::vector<Light *> lights
                );

            void createRenderer(graphics::Shader *shader);

            BatchRenderer* m_renderer = nullptr;
            std::vector<glm::vec2> m_positions = {};
            std::vector<glm::vec2> m_tilesetPositions = {};
            float m_tilesetWidth;
            float m_tilesetHeight;
        };
    }
}