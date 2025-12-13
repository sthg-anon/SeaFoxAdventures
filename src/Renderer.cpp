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

#include "assets/fonts/Console.hpp"
#include "assets/sprites/Tiles.hpp"
#include "assets/sprites/Player.hpp"
#include "Constants.hpp"
#include "Coordinate.hpp"
#include "DebugWindow.hpp"
#include "Player.hpp"
#include "TileRange.hpp"
#include "World.hpp"
#include "TileData.hpp"

namespace
{
    constexpr std::int32_t VirtualScreenWidth = 320;
    constexpr std::int32_t VirtualScreenHeight = 180;
    constexpr float PlayerRotationSpeed = 10.0f;
}

namespace sfa
{
    Renderer::Renderer(ScreenDimensions dimensions)
        : m_screenDimensions{ dimensions }
        , m_camera{ 0 }
        , m_renderTexture{ LoadRenderTexture(VirtualScreenWidth, VirtualScreenHeight) }
        , m_tilesTexture{ GetTilesTexture() }
        , m_playerTexture{ GetPlayerTexture() }
        , m_currentPlayerRotation{ 0.0f }
        , m_font{ LoadFont() }
    {
        Expects(m_screenDimensions.width.Get() > 0);
        Expects(m_screenDimensions.height.Get() > 0);

        m_camera.zoom = 1.0f;
        m_camera.offset = Vector2{ static_cast<float>(VirtualScreenWidth) * 0.5f, static_cast<float>(VirtualScreenHeight) * 0.5f };

        SetTextureFilter(m_renderTexture.texture, TEXTURE_FILTER_POINT);

        rlImGuiSetup(true);
    }

    float Renderer::GetVirtualRatio() const
    {
        return static_cast<float>(m_screenDimensions.width.Get()) / static_cast<float>(VirtualScreenWidth);
    }

    Renderer::~Renderer()
    {
        UnloadRenderTexture(m_renderTexture);
        UnloadTexture(m_tilesTexture);
        UnloadTexture(m_playerTexture);
        UnloadFont(m_font);
        rlImGuiShutdown();
    }

    void Renderer::DrawFrame(World& world, Player& player)
    {
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

            DrawTextEx(
                m_font,
                "Sea Fox Adventures",
                Vector2{ 50.0f, 50.0f },
                9,
                2,
                BLACK);
        }

        {
            BeginDrawing();
            auto endDrawingGuard = gsl::finally(EndDrawing);

            ClearBackground(RED);

            DrawTexturePro(
                m_renderTexture.texture,
                Rectangle{
                    0.0f,
                    0.0f,
                    static_cast<float>(m_renderTexture.texture.width),
                    static_cast<float>(-m_renderTexture.texture.height)
                },
                Rectangle{
                    0.0f,
                    0.0f,
                    static_cast<float>(m_screenDimensions.width.Get()),
                    static_cast<float>(m_screenDimensions.height.Get())
                },
                Vector2{ 0.0f, 0.0f },
                0.0f,
                WHITE);
            EndMode2D();

            // More resolution-independent drawing stuff can happen here.

            rlImGuiBegin();
            auto rlImGuiEndGuard = gsl::finally(rlImGuiEnd);
            m_debugWindow.Draw(world, player);
        }
    }

    void Renderer::DrawWorld(World& world)
    {
        for (const auto& pos : TileRange(TileCoord{ 0 }, TileCoord{ 0 }, TileCoord{ Constants::WorldWidth }, TileCoord{ Constants::WorldHeight }))
        {
            const TileType tileType = world.GetTile(pos);
            const auto tileData = GetTileData(tileType);
            DrawTexturePro(
                m_tilesTexture,
                Rectangle{
                    static_cast<float>(tileData.texture_x * Constants::WorldTileSizePixels),
                    static_cast<float>(tileData.texture_y * Constants::WorldTileSizePixels),
                    Constants::WorldTileSizePixels,
                    Constants::WorldTileSizePixels
                },
                Rectangle{
                    static_cast<float>(pos.x.Get() * Constants::WorldTileSizePixels),
                    static_cast<float>(pos.y.Get() * Constants::WorldTileSizePixels),
                    Constants::WorldTileSizePixels,
                    Constants::WorldTileSizePixels
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
                Constants::WorldTileSizePixels,
                flipY * Constants::WorldTileSizePixels
            },
            Rectangle{
                player.GetPosition().x,
                player.GetPosition().y,
                Constants::WorldTileSizePixels,
                Constants::WorldTileSizePixels
            },
            Vector2{ Constants::PlayerHalfSize, Constants::PlayerHalfSize },
            m_currentPlayerRotation,
            WHITE);

        // Drill
        DrawTexturePro(
            m_playerTexture,
            Rectangle{
                Constants::WorldTileSizePixels,
                0,
                Constants::WorldTileSizePixels,
                flipY * Constants::WorldTileSizePixels
            },
            Rectangle{
                player.GetPosition().x,
                player.GetPosition().y,
                Constants::WorldTileSizePixels,
                Constants::WorldTileSizePixels
            },
            Vector2{ Constants::PlayerHalfSize - Constants::WorldTileSizePixels, Constants::PlayerHalfSize },
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

    Font Renderer::LoadFont()
    {
        return LoadFontFromMemory(".ttf", ConsoleFontTTF, static_cast<int>(ConsoleFontTTF_size), 9, nullptr, 0);
    }

    ScreenDimensions Renderer::GetScreenDimensions(std::int32_t scale)
    {
        return ScreenDimensions{ ScreenCoord{VirtualScreenWidth * scale}, ScreenCoord{VirtualScreenHeight * scale} };
    }
}