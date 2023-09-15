// Created by Modar Nasser on 16/11/2020.

#pragma once

#include <array>
#include <string>
#include <vector>

#include "../thirdparty/json_fwd.hpp"
#include "../containers/TagsContainer.hpp"
#include "FieldDef.hpp"
#include "../DataTypes.hpp"
#include "../Tileset.hpp"

namespace ldtk
{

    class Project;

    class EntityDef : public TagsContainer
    {
    public:
        const std::string name;
        const int uid;
        const IntPoint size;
        const Color color;
        const FloatPoint pivot;
        const Tileset *tileset;
        const IntRect texture_rect;
        std::vector<FieldDef> fields;
        NineSliceBorders nine_slice_borders;

        EntityDef(const nlohmann::json &j, Project *p);
    };

}
