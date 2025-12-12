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

#include "DebugWindow.hpp"

#include <gsl/gsl>
#include <imgui.h>
#include <raylib.h>

namespace
{
    static constexpr ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings;

    static constexpr float DebugWindowOffset = 10.0f;
    
    static constexpr float DebugWindowHoldTime = 0.4f;
}

namespace sfa
{
    void DebugWindow::Draw(World& world, Player& player)
    {
        if (IsKeyDown(KEY_GRAVE))
        {
            m_showWindowTimer += GetFrameTime();
        }
        else
        {
            m_showWindowTimer = 0.0f;
        }

        if (m_showWindowTimer > DebugWindowHoldTime)
        {
            m_showDebugWindow = true;
        }

        if (!m_showDebugWindow)
        {
            return;
        }

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImGui::SetNextWindowPos(ImVec2(DebugWindowOffset, DebugWindowOffset), ImGuiCond_Always);

        auto windowEndGuard = gsl::finally(ImGui::End);
        if (ImGui::Begin("Debug", &m_showDebugWindow, WindowFlags))
        {
            ImGui::Text("This is a window without decorations, fixed to the top-left corner!");
            ImGui::Separator();
            ImGui::Text("Current Display Size: %.1f x %.1f", displaySize.x, displaySize.y);
        }
    }
}