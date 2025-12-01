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

#include <cstdlib>
#include <raylib.h>
#include <gsl/gsl>
#include <rlImGui.h>
#include <imgui.h>

#include "assets/sprites/TestImage.hpp"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    auto cleanup = gsl::finally(CloseWindow);

    SetTargetFPS(60);

    rlImGuiSetup(true);
    auto rlImGuiCleanup = gsl::finally(rlImGuiShutdown);

    Image test_image = LoadImageFromMemory(".png", sfa::TestImage, sfa::TestImage_size);
    auto unloadImage = gsl::finally([&test_image]() { UnloadImage(test_image); });

    Texture2D test_texture = LoadTextureFromImage(test_image);
    auto unloadTexture = gsl::finally([&test_texture]() { UnloadTexture(test_texture); });

    while (!WindowShouldClose())
    {
        BeginDrawing();
        auto endDrawing = gsl::finally(EndDrawing);

        rlImGuiBegin();
        auto rlImGuiEndGuard = gsl::finally(rlImGuiEnd);

        //ImGui::ShowDemoWindow();

        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        DrawTexture(test_texture, screenWidth / 2 - test_texture.width / 2, screenHeight / 2 - test_texture.height / 2, WHITE);
    }

    return EXIT_SUCCESS;
}