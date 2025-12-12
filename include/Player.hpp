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

#include <raylib.h>

namespace sfa
{
    class World;

    static constexpr float PlayerHalfSize = 8.0f;

    class Player
    {
        // Player position
        Vector2 m_position{ 504.0f, 200.0f };

        // Where the player wants to face
        float m_targetRotation{ 0.0f };

        // Whether or not the sprite needs to be flipped on the y axis.
        bool m_flipY{ false };

        Vector2 m_velocity{ 0.0f, 0.0f };
        
        bool CheckCollision(const World& world, float x, float y) const;
        void TryBreakTile(World& world, float x, float y);

    public:
        Player() = default;

        void ProcessInputs();
        void MoveWithCollision(World& world);

        bool IsMovingInCardinalDirection() const;

        Vector2 GetPosition() const { return m_position; }
        float GetTargetRotation() const { return m_targetRotation; }
        bool IsFlippedY() const { return m_flipY; }
    };
}