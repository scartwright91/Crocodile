#include "Scene.h"
#include "../Core.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API Entity
        {
        public:
            Entity(s2d::Scene* scene, ResourceManager* rm);
            ~Entity();

            Object *sprite = nullptr;
            s2d::Scene* scene = nullptr;
            ResourceManager* rm = nullptr;

        };
    }
}