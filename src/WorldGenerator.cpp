#include "WorldGenerator.hpp"

#include <cstdint>
#include <random>

#include "World.hpp"

namespace sfa
{
    namespace
    {
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
                world.setTile(x, y, TileType::Sky);
            }
        }

        /* Water surface */
        auto waterSurfaceY = SkyHeight;
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            world.setTile(x, waterSurfaceY, TileType::WaterSurface);
        }

        /* Underground */
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            for (std::int32_t y = waterSurfaceY + 1; y < WorldHeight; ++y)
            {
                auto r = dist(gen);
                if (r < 0.9f)
                {
                    world.setTile(x, y, TileType::Earth);
                }
            }
        }

        /* Iron ore */
        for (std::int32_t x = 0; x < WorldWidth; ++x)
        {
            for (std::int32_t y = waterSurfaceY + 1; y < WorldHeight; ++y)
            {
                auto r = dist(gen);
                if (r < 0.05f)
                {
                    world.setTile(x, y, TileType::IronOre);
                }
            }
        }

    }
}