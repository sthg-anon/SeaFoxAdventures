#include "World.hpp"

#include <gsl/assert>

#include <cstdint>

namespace sfa
{
    World::World()
    {
        m_tiles.fill(TileType::UnderWater);
    }

    TileType World::getTile(std::int32_t x, std::int32_t y) const
    {
        Expects(x >= 0 && x < WorldWidth);
        Expects(y >= 0 && y < WorldHeight);
        return m_tiles[y * WorldWidth + x];
    }

    void World::setTile(std::int32_t x, std::int32_t y, TileType type)
    {
        Expects(x >= 0 && x < WorldWidth);
        Expects(y >= 0 && y < WorldHeight);
        m_tiles[y * WorldWidth + x] = type;
    }
}