#pragma once

#include <array>
#include <cstdint>

#include "TileType.hpp"

namespace sfa
{
    constexpr std::int32_t WorldWidth = 64;
    constexpr std::int32_t WorldHeight = 1024;
    constexpr std::int32_t SkyHeight = 10;

    class World
    {
        std::array<TileType, WorldWidth * WorldHeight> m_tiles;

    public:
        World();
        World(const World&) = delete;
        World& operator=(const World&) = delete;
        World(World&&) = default;
        World& operator=(World&&) = default;

        TileType getTile(std::int32_t x, std::int32_t y) const;
        void setTile(std::int32_t x, std::int32_t y, TileType type);
    };
}