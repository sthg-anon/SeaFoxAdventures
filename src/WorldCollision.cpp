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

#include "WorldCollision.hpp"

#include <cmath>

#include "Coordinate.hpp"
#include "World.hpp"

namespace sfa
{
    bool RectHitsSolid(const World& world, float centerX, float centerY, float halfWidth, float halfHeight)
    {
        float left = centerX - halfWidth;
        float right = centerX + halfWidth;
        float top = centerY - halfHeight;
        float bottom = centerY + halfHeight;

        auto minX = static_cast<std::int32_t>(std::floor(left) / WorldTileSizePixels);
        auto maxX = static_cast<std::int32_t>(std::floor(right) / WorldTileSizePixels);
        auto minY = static_cast<std::int32_t>(std::floor(top) / WorldTileSizePixels);
        auto maxY = static_cast<std::int32_t>(std::floor(bottom) / WorldTileSizePixels);

        for (std::int32_t y = minY; y <= maxY; ++y)
        {
            for (std::int32_t x = minX; x <= maxX; ++x)
            {
                if (world.IsTileSolidAt(TileCoord{ x }, TileCoord{ y }))
                {
                    return true;
                }
            }
        }

        return false;
    }
}