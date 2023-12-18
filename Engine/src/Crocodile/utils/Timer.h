#pragma once

#include "../Core.h"

namespace Crocodile
{
    class CROCODILE_API Timer
    {
        public:

            bool active = false;
            bool finished = false;
            float current = 0.0f;
            float total = 0.0f;

            Timer(float total) : total(total)
            {

            }
            ~Timer()
            {

            }

            void activate()
            {
                active = true;
                finished = false;
                current = total;
            }

            void stop()
            {
                active = false;
                finished = true;
            }

            void update(float dt)
            {
                if (!finished)
                {
                    current -= dt;
                    if (current <= 0.0f)
                        finished = true;
                }
            }
    };
}
