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

#include "Renderer.hpp"

#include <cmath>
#include <cstdint>

#include <gsl/gsl>
#include <raylib.h>
#include <rlImGui.h>

#include "assets/sprites/Tiles.hpp"
#include "assets/sprites/Player.hpp"
#include "Coordinate.hpp"
#include "DebugWindow.hpp"
#include "Player.hpp"
#include "TileRange.hpp"
#include "World.hpp"
#include "TileData.hpp"

namespace sfa
{
    Renderer::Renderer(std::int32_t screenWidth, std::int32_t screenHeight)
        : m_screenWidth(screenWidth)
        , m_screenHeight(screenHeight)
        , m_camera{ 0 }
        , m_renderTexture{ LoadRenderTexture(VirtualScreenWidth, VirtualScreenHeight) }
        , m_sourceRec{ GetSourceRec() }
        , m_destRec{ GetDestRec() }
        , m_tilesTexture{ GetTilesTexture() }
        , m_playerTexture{ GetPlayerTexture() }
        , m_currentPlayerRotation{ 0.0f }
    {
        Expects(screenWidth > 0);
        Expects(screenHeight > 0);

        m_camera.zoom = 1.0f;
        m_camera.offset = Vector2{ static_cast<float>(VirtualScreenWidth) * 0.5f, static_cast<float>(VirtualScreenHeight) * 0.5f };

        SetTextureFilter(m_renderTexture.texture, TEXTURE_FILTER_POINT);

        rlImGuiSetup(true);
    }

    float Renderer::GetVirtualRatio() const
    {
        return static_cast<float>(m_screenWidth) / static_cast<float>(VirtualScreenWidth);
    }

    Renderer::~Renderer()
    {
        UnloadRenderTexture(m_renderTexture);
        UnloadTexture(m_tilesTexture);
        UnloadTexture(m_playerTexture);
        rlImGuiShutdown();
    }

    Rectangle Renderer::GetSourceRec() const
    {
        // This is the entire render texture. The height is negative because
        // it needs to be flipped for "opengl reasons".
        return Rectangle{
            0.0f,
            0.0f,
            static_cast<float>(m_renderTexture.texture.width),
            static_cast<float>(-m_renderTexture.texture.height)
        };
    }

    Rectangle Renderer::GetDestRec() const
    {
        // Normally the destination rectangle would be the size of the window itself, but we
        // add "virtual ratio" padding around the edges to avoid artifacts.
        const float virtualRatio = GetVirtualRatio();
        return Rectangle{
            -virtualRatio,
            -virtualRatio,
            static_cast<float>(m_screenWidth) + (virtualRatio * 2),
            static_cast<float>(m_screenHeight) + (virtualRatio * 2)
        };
    }

    void Renderer::DrawFrame(World& world, Player& player)
    {
        const float virtualRatio = GetVirtualRatio();

        // Camera follows player
        m_camera.target = player.GetPosition();
        m_camera.target.x = std::round(m_camera.target.x);
        m_camera.target.y = std::round(m_camera.target.y);

        {
            BeginTextureMode(m_renderTexture);
            auto endTextureModeGuard = gsl::finally(EndTextureMode);

            ClearBackground(RAYWHITE);

            BeginMode2D(m_camera);
            auto endMode2DGuard = gsl::finally(EndMode2D);


            // Draw stuff to texture
            DrawWorld(world);
            DrawPlayer(player);
        }

        {
            BeginDrawing();
            auto endDrawingGuard = gsl::finally(EndDrawing);

            ClearBackground(RED);

            DrawTexturePro(
                m_renderTexture.texture,
                m_sourceRec,
                m_destRec,
                Vector2{ 0.0f, 0.0f },
                0.0f,
                WHITE);
            EndMode2D();

            // More resolution-independent drawing stuff can happen here.

            rlImGuiBegin();
            auto rlImGuiEndGuard = gsl::finally(rlImGuiEnd);
            DrawDebugWindow(world, player);
        }
    }

    void Renderer::DrawWorld(World& world)
    {
        for (const auto& pos : TileRange(TileCoord{ 0 }, TileCoord{ 0 }, TileCoord{ WorldWidth }, TileCoord{ WorldHeight }))
        {
            const TileType tileType = world.GetTile(pos);
            const auto tileData = GetTileData(tileType);
            DrawTexturePro(
                m_tilesTexture,
                Rectangle{
                    static_cast<float>(tileData.texture_x * WorldTileSizePixels),
                    static_cast<float>(tileData.texture_y * WorldTileSizePixels),
                    WorldTileSizePixels,
                    WorldTileSizePixels
                },
                Rectangle{
                    static_cast<float>(pos.x.Get() * WorldTileSizePixels),
                    static_cast<float>(pos.y.Get() * WorldTileSizePixels),
                    WorldTileSizePixels,
                    WorldTileSizePixels
                },
                Vector2{ 0.0f, 0.0f },
                0.0f,
                WHITE);
        }
    }

    void Renderer::DrawPlayer(Player& player)
    {
        float deltaTime = GetFrameTime();
        float target = player.GetTargetRotation();
        if (m_currentPlayerRotation < 0.0f)
        {
            m_currentPlayerRotation += 360.0f;
        }
        else if (m_currentPlayerRotation >= 360.0f)
        {
            m_currentPlayerRotation -= 360.0f;
        }
        float diff = target - m_currentPlayerRotation;
        if (diff > 180.0f)
        {
            diff -= 360.0f;
        }
        else if (diff < -180.0f)
        {
            diff += 360.0f;
        }
        m_currentPlayerRotation += diff * PlayerRotationSpeed * deltaTime;

        if (std::fabs(diff) < 0.1f)
        {
            m_currentPlayerRotation = target;
        }

        float flipY = player.IsFlippedY() ? -1.0f : 1.0f;

        // Sea Fox Subarmine Sprite
        DrawTexturePro(
            m_playerTexture,
            Rectangle{
                0,
                0,
                WorldTileSizePixels,
                flipY * WorldTileSizePixels
            },
            Rectangle{
                player.GetPosition().x,
                player.GetPosition().y,
                WorldTileSizePixels,
                WorldTileSizePixels
            },
            Vector2{ PlayerHalfSize, PlayerHalfSize },
            m_currentPlayerRotation,
            WHITE);

        // Drill
        DrawTexturePro(
            m_playerTexture,
            Rectangle{
                16,
                0,
                WorldTileSizePixels,
                flipY * WorldTileSizePixels
            },
            Rectangle{
                player.GetPosition().x,
                player.GetPosition().y,
                WorldTileSizePixels,
                WorldTileSizePixels
            },
            Vector2{ PlayerHalfSize - WorldTileSizePixels, PlayerHalfSize },
            m_currentPlayerRotation,
            WHITE);
    }

    Texture2D Renderer::GetTilesTexture()
    {
        Image tilesImage = LoadImageFromMemory(".png", TilesSprite, static_cast<std::int32_t>(TilesSprite_size));
        auto unloadImageGuard = gsl::finally([&tilesImage]() { UnloadImage(tilesImage); });
        Texture2D tilesTexture = LoadTextureFromImage(tilesImage);
        SetTextureFilter(tilesTexture, TEXTURE_FILTER_POINT);
        return tilesTexture;
    }

    Texture2D Renderer::GetPlayerTexture()
    {
        Image playerImage = LoadImageFromMemory(".png", PlayerSprite, static_cast<std::int32_t>(PlayerSprite_size));
        auto unloadImageGuard = gsl::finally([&playerImage]() { UnloadImage(playerImage); });
        Texture2D playerTexture = LoadTextureFromImage(playerImage);
        SetTextureFilter(playerTexture, TEXTURE_FILTER_POINT);
        return playerTexture;
    }
}