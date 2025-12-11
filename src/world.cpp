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

#include "World.hpp"

#include <gsl/assert>

#include "TileData.hpp"

namespace sfa
{
    World::World()
    {
        m_tiles.fill(TileType::UnderWater);
    }

    TileType World::GetTile(TilePosition pos) const
    {
        Expects(pos.x.Get() >= 0 && pos.x.Get() < WorldWidth);
        Expects(pos.y.Get() >= 0 && pos.y.Get() < WorldHeight);
        return m_tiles[pos.y.Get() * WorldWidth + pos.x.Get()];
    }

    void World::SetTile(TilePosition pos, TileType type)
    {
        Expects(pos.x.Get() >= 0 && pos.x.Get() < WorldWidth);
        Expects(pos.y.Get() >= 0 && pos.y.Get() < WorldHeight);
        m_tiles[pos.y.Get() * WorldWidth + pos.x.Get()] = type;
    }

    bool World::IsTileSolidAt(TilePosition pos) const
    {
        if (pos.x.Get() < 0 || pos.y.Get() < 0 || pos.x.Get() >= WorldWidth || pos.y.Get() >= WorldHeight)
        {
            return true;
        }

        TileType type = GetTile(pos);
        return GetTileData(type).isSolid;
    }
}