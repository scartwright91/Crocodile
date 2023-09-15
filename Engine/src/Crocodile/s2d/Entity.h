#include "Scene.h"
#include "../audio/SoundManager.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API Entity
        {
        public:
            Entity(Scene *scene, std::string layer, ResourceManager *rm, SoundManager *sm);
            ~Entity();

            Object *sprite = nullptr;

            void add();
            void remove();

        private:
            Scene *scene = nullptr;
            std::string layer = "";
            ResourceManager *rm = nullptr;
            SoundManager *sm = nullptr;
        };
    }
}