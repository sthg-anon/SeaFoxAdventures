#pragma once

#include <cstdint>

#include "tile_type.hpp"

namespace sfa
{
    struct TileData
    {
        float hardness;
        bool isSolid;
        std::uint8_t texture_x;
        std::uint8_t texture_y;
    };

    constexpr const TileData& getTileData(TileType type);
}