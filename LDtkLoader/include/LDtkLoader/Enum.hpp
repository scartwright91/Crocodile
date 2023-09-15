// Created by Modar Nasser on 13/11/2020.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "thirdparty/json_fwd.hpp"
#include "containers/TagsContainer.hpp"
#include "DataTypes.hpp"

namespace ldtk
{

    class Enum;
    class Tileset;

    struct EnumValue
    {
        const std::string name;
        const Color color;
        const Enum &type;

        auto hasIcon() const -> bool;
        auto getIconTileset() const -> const Tileset &;
        auto getIconTexturePos() const -> IntPoint;

    private:
        friend Enum;
        friend auto operator==(const EnumValue &l, const EnumValue &r) -> bool;
        EnumValue(std::string name, int id, int tile_id, const Color &color, const Enum &enum_type);
        const int id;
        const int tile_id;
    };

    auto operator==(const EnumValue &l, const EnumValue &r) -> bool;
    auto operator!=(const EnumValue &l, const EnumValue &r) -> bool;

    class Enum : public TagsContainer
    {
        friend class Project;

    public:
        Enum(const Enum &) = delete;
        Enum(Enum &&) = default;
        auto operator=(const Enum &) -> Enum & = delete;

        const std::string name;
        const int uid;

        auto operator[](const std::string &val_name) const -> const EnumValue &;

        auto hasIcons() const -> bool;
        auto getIconsTileset() const -> const Tileset &;

        explicit Enum(const nlohmann::json &j);

    private:
        const int m_tileset_id;
        const Tileset *m_tileset = nullptr;
        std::unordered_map<std::string, EnumValue> m_values;
    };

}

auto operator<<(std::ostream &os, const ldtk::EnumValue &enum_value) -> std::ostream &;
