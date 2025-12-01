#include "tile_data.hpp"

#include <utility>
#include "tile_type.hpp"

namespace sfa
{
    using underlying_t = std::underlying_type<TileType>::type;

    // These need to be in the exact same order that they're declared in TileType.
    constexpr TileData g_tileData[static_cast<underlying_t>(TileType::Count)] =
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
            .texture_x = 4,
            .texture_y = 0,
        }
    };

    // If this fails, that probably means an entry was added to TileType but not to g_tileData.
    static_assert(
        static_cast<underlying_t>(TileType::Count) == sizeof(g_tileData) / sizeof(g_tileData[0]),
        "TileData table size must match TileType::Count"
    );

    constexpr const TileData& getTileData(TileType type)
    {
        auto index = static_cast<std::underlying_type_t<TileType>>(type);
        return g_tileData[index];
    }
}