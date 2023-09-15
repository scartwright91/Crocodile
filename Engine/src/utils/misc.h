#pragma once

#include "../Crocodile/Core.h"

namespace Crocodile
{
    template <typename t>
    void CROCODILE_API moveVectorElement(std::vector<t> &v, size_t oldIndex, size_t newIndex)
    {
        if (oldIndex > newIndex)
            std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
        else
            std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
    };
}