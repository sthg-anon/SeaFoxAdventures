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

#include "Coordinate.hpp"
#include "DebugWindow.hpp"
#include "Player.hpp"
#include "world.hpp"

namespace sfa
{
    class Renderer
    {
    private:
        // The dimensions of the actual window that is being rendered to.
        ScreenDimensions m_screenDimensions;

        Camera2D m_camera;

        // The low-res render texture that the game is drawn to. This is a resource
        // managed by the Renderer and must be cleaned up.
        RenderTexture2D m_renderTexture;

        // Source and destination rectangles for "projecting" the render texture to
        // the screen.
        Rectangle m_sourceRec;
        Rectangle m_destRec;

        // Tiles texture for drawing the world.
        Texture2D m_tilesTexture;

        // Player texture
        Texture2D m_playerTexture;

        // The player's current rotation. Used for linear interpolation.
        float m_currentPlayerRotation;

        DebugWindow m_debugWindow;

        Font m_font;

    public:
        Renderer(ScreenDimensions dimensions);
        ~Renderer();
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void DrawFrame(World& world, Player& player);

        static ScreenDimensions GetScreenDimensions(std::int32_t scale);

    private:
        float GetVirtualRatio() const;
        Rectangle GetSourceRec() const;
        Rectangle GetDestRec() const;
        void DrawWorld(World& world);
        void DrawPlayer(Player& player);
        static Texture2D GetTilesTexture();
        static Texture2D GetPlayerTexture();
        static Font LoadFont();
    };
}
