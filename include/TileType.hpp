#pragma once

#include <cstdint>

namespace sfa
{
    enum class TileType : std::uint8_t
    {
        Sky,
        WaterSurface,
        UnderWater,
        Earth,
        IronOre,

        Count
    };
}