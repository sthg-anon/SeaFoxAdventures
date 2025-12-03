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

#include "assets/sprites/TestImage.hpp"
#include "assets/sprites/Tiles.hpp"
#include "World.hpp"
#include "TileData.hpp"

namespace sfa
{
    Renderer::Renderer(std::int32_t screenWidth, std::int32_t screenHeight)
        : m_screenWidth(screenWidth)
        , m_screenHeight(screenHeight)
        , m_worldSpaceCamera{ 0 }
        , m_screenSpaceCamera{ 0 }
        , m_renderTexture{ LoadRenderTexture(VirtualScreenWidth, VirtualScreenHeight) }
        , m_sourceRec{ GetSourceRec() }
        , m_destRec{ GetDestRec() }
        , m_testTexture{ GetTestTexture() }
        , m_tilesTexture{ GetTilesTexture() }
        , m_cameraX(0.0f)
        , m_cameraY(0.0f)
    {
        Expects(screenWidth > 0);
        Expects(screenHeight > 0);

        m_worldSpaceCamera.zoom = 1.0f;
        m_screenSpaceCamera.zoom = 1.0f;
    }

    float Renderer::GetVirtualRatio() const
    {
        return static_cast<float>(m_screenWidth) / static_cast<float>(VirtualScreenWidth);
    }

    Renderer::~Renderer()
    {
        UnloadRenderTexture(m_renderTexture);
        UnloadTexture(m_testTexture);
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

    void Renderer::DrawFrame(World& world)
    {
        const float virtualRatio = GetVirtualRatio();

        m_screenSpaceCamera.target = Vector2{ m_cameraX, m_cameraY };

        m_worldSpaceCamera.target.x = std::truncf(m_screenSpaceCamera.target.x);
        m_screenSpaceCamera.target.x -= m_worldSpaceCamera.target.x;
        m_screenSpaceCamera.target.x *= virtualRatio;

        m_worldSpaceCamera.target.y = std::truncf(m_screenSpaceCamera.target.y);
        m_screenSpaceCamera.target.y -= m_worldSpaceCamera.target.y;
        m_screenSpaceCamera.target.y *= virtualRatio;

        {
            BeginTextureMode(m_renderTexture);
            auto endTextureModeGuard = gsl::finally(EndTextureMode);

            ClearBackground(RAYWHITE);

            BeginMode2D(m_worldSpaceCamera);
            auto endMode2DGuard = gsl::finally(EndMode2D);

            // Draw stuff to texture
            DrawTexture(m_testTexture, 0, 0, WHITE);

            for (std::int32_t y = 0; y < WorldHeight; ++y)
            {
                for (std::int32_t x = 0; x < WorldWidth; ++x)
                {
                    const TileType tileType = world.getTile(x, y);
                    const auto tileData = getTileData(tileType);
                    DrawTexturePro(
                        m_tilesTexture,
                        Rectangle{
                            static_cast<float>(tileData.texture_x * 16),
                            static_cast<float>(tileData.texture_y * 16),
                            16.0f,
                            16.0f
                        },
                        Rectangle{
                            static_cast<float>(x * 16),
                            static_cast<float>(y * 16),
                            16.0f,
                            16.0f
                        },
                        Vector2{ 0.0f, 0.0f },
                        0.0f,
                        WHITE);
                }
            }
        }

        {
            BeginDrawing();
            auto endDrawingGuard = gsl::finally(EndDrawing);

            ClearBackground(RED);

            BeginMode2D(m_screenSpaceCamera);
            DrawTexturePro(
                m_renderTexture.texture,
                m_sourceRec,
                m_destRec,
                Vector2{ 0.0f, 0.0f },
                0.0f,
                WHITE);
            EndMode2D();

            // More resolution-independent drawing stuff can happen here.
        }
    }

    void Renderer::SetCamera(float x, float y)
    {
        m_cameraX = x;
        m_cameraY = y;
    }

    Texture2D Renderer::GetTestTexture()
    {
        Image testImage = LoadImageFromMemory(".png", TestImage, static_cast<std::int32_t>(TestImage_size));
        auto unloadImageGuard = gsl::finally([&testImage]() { UnloadImage(testImage); });
        Texture2D testTexture = LoadTextureFromImage(testImage);
        return testTexture;
    }

    Texture2D Renderer::GetTilesTexture()
    {
        Image tilesImage = LoadImageFromMemory(".png", TilesSprite, static_cast<std::int32_t>(TilesSprite_size));
        auto unloadImageGuard = gsl::finally([&tilesImage]() { UnloadImage(tilesImage); });
        Texture2D tilesTexture = LoadTextureFromImage(tilesImage);
        return tilesTexture;
    }
}