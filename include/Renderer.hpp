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

#include <raylib.h>

#include "Player.hpp"
#include "world.hpp"

namespace sfa
{
    const std::int32_t VirtualScreenWidth = 320;
    const std::int32_t VirtualScreenHeight = 180;

    class Renderer
    {
    private:
        // The dimensions of the actual window that is being rendered to.
        std::int32_t m_screenWidth;
        std::int32_t m_screenHeight;

        // Game world camera. Used when drawing things to the render texture.
        Camera2D m_worldSpaceCamera;

        // Smoothing camera. Used when drawing the render texture to the screen.
        Camera2D m_screenSpaceCamera;

        // The low-res render texture that the game is drawn to. This is a resource
        // managed by the Renderer and must be cleaned up.
        RenderTexture2D m_renderTexture;

        // Source and destination rectangles for "projecting" the render texture to
        // the screen.
        Rectangle m_sourceRec;
        Rectangle m_destRec;

        // Tiles texture for drawing the world.
        Texture2D m_tilesTexture;

    public:
        Renderer(std::int32_t screenWidth, std::int32_t screenHeight);
        ~Renderer();
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void DrawFrame(World& world, Player& player);

    private:
        float GetVirtualRatio() const;
        Rectangle GetSourceRec() const;
        Rectangle GetDestRec() const;
        void DrawWorld(World& world);
        static Texture2D GetTilesTexture();
    };
}
