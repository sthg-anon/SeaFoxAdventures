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

#include "WorldGenerator.hpp"

#include <cstdint>
#include <random>

#include "Coordinate.hpp"
#include "World.hpp"

namespace sfa
{
    namespace
    {
        const std::int32_t WaterDepth = 5;
    }

    void generate_world(World& world)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        /* Sky */
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            for (std::int32_t y = 0; y < SkyHeight; ++y)
            {
                world.SetTile(TileCoord{ x }, TileCoord{ y }, TileType::Sky);
            }
        }

        /* Water surface */
        auto waterSurfaceY = SkyHeight;
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            world.SetTile(TileCoord{ x }, TileCoord{ waterSurfaceY }, TileType::WaterSurface);
        }

        /* Water */
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            for (std::int32_t y = waterSurfaceY + 1; y < waterSurfaceY + WaterDepth; ++y)
            {
                world.SetTile(TileCoord{ x }, TileCoord{ y }, TileType::UnderWater);
            }
        }

        /* Underground */
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            for (std::int32_t y = waterSurfaceY + WaterDepth + 1; y < WorldHeight; ++y)
            {
                auto r = dist(gen);
                if (r < 0.8f)
                {
                    world.SetTile(TileCoord{ x }, TileCoord{ y }, TileType::Earth);
                }
            }
        }

        /* Iron ore */
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            for (std::int32_t y = waterSurfaceY + WaterDepth + 1; y < WorldHeight; ++y)
            {
                auto r = dist(gen);
                if (r < 0.05f)
                {
                    world.SetTile(TileCoord{ x }, TileCoord{ y }, TileType::IronOre);
                }
            }
        }

    }
}