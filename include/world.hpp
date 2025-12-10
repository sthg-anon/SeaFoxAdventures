/*
* Copyright (c) 2025 sthg-anon
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <array>
#include <cstdint>

#include "TileType.hpp"

namespace sfa
{
    constexpr std::int32_t WorldWidth = 64;
    constexpr std::int32_t WorldHeight = 1024;
    constexpr std::int32_t SkyHeight = 10;

    constexpr std::int32_t WorldTileSizePixels = 16;

    class World
    {
        std::array<TileType, WorldWidth * WorldHeight> m_tiles;

    public:
        World();
        World(const World&) = delete;
        World& operator=(const World&) = delete;
        World(World&&) = default;
        World& operator=(World&&) = default;

        TileType GetTile(std::int32_t x, std::int32_t y) const;
        void SetTile(std::int32_t x, std::int32_t y, TileType type);

        bool IsTileSolidAt(std::int32_t x, std::int32_t y) const;
    };
}