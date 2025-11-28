#include <cstdlib>
#include <raylib.h>
#include <gsl/gsl>
#include <rlImGui.h>
#include <imgui.h>

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    auto cleanup = gsl::finally(CloseWindow);

    SetTargetFPS(60);

    rlImGuiSetup(true);
    auto rlImGuiCleanup = gsl::finally(rlImGuiShutdown);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        auto endDrawing = gsl::finally(EndDrawing);

        rlImGuiBegin();
        auto rlImGuiEndGuard = gsl::finally(rlImGuiEnd);

        ImGui::ShowDemoWindow();

        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    }

    return EXIT_SUCCESS;
}