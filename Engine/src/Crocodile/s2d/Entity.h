#include "Object.h"
#include "../Core.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API Entity
        {
        public:
            Entity(std::string name);
            ~Entity();

            std::string name;
            Object *sprite = nullptr;

        };
    }
}