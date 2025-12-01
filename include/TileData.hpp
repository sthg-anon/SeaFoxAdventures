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

#include <cstdint>

#include "TileType.hpp"

namespace sfa
{
    struct TileData
    {
        float hardness;
        bool isSolid;
        std::uint8_t texture_x;
        std::uint8_t texture_y;
    };

    inline constexpr TileData g_tileData[static_cast<std::uint8_t>(TileType::Count)] =
    {
        // Sky
        TileData{
            .hardness = 0.0f,
            .isSolid = false,
            .texture_x = 0,
            .texture_y = 0,
        },

        // WaterSurface
        TileData{
            .hardness = 0.0f,
            .isSolid = false,
            .texture_x = 1,
            .texture_y = 0,
        },

        // UnderWater
        TileData{
            .hardness = 0.0f,
            .isSolid = false,
            .texture_x = 2,
            .texture_y = 0,
        },

        // Earth
        TileData{
            .hardness = 1.0f,
            .isSolid = true,
            .texture_x = 3,
            .texture_y = 0,
        },

        // IronOre
        TileData{
            .hardness = 1.5f,
            .isSolid = true,
            .texture_x = 0,
            .texture_y = 1,
        }
    };

    constexpr const TileData& getTileData(TileType type)
    {
        auto index = static_cast<std::uint8_t>(type);
        return g_tileData[index];
    }
}