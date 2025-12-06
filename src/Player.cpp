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

#include "Player.hpp"

#include <cmath>

#include <raylib.h>

namespace sfa
{
    Player::Player()
        : m_position{ 0.f, 0.f }
        , m_speed{ BaseSpeed }
        , m_targetRotation{ 0.0f }
        , m_flipX{ false }
    {
    }

    void Player::ProcessInputs()
    {
        float deltaTime = GetFrameTime();

        Vector2 movement = { 0.f, 0.f };
        if (IsKeyDown(KEY_W))
        {
            movement.y -= 1.f;
        }
        if (IsKeyDown(KEY_S))
        {
            movement.y += 1.f;
        }
        if (IsKeyDown(KEY_A))
        {
            movement.x -= 1.f;
        }
        if (IsKeyDown(KEY_D))
        {
            movement.x += 1.f;
        }

        if (std::abs(movement.x) > 0.f || std::abs(movement.y) > 0.f)
        {
            float angleRad = std::atan2(movement.y, movement.x);
            float angleDeg = angleRad * 180.f / PI;
            if (angleDeg < 0.f)
            {
                angleDeg += 360.f;
            }

            if (angleDeg >= 60.f && angleDeg < 120.f)
            {
                m_targetRotation = 90.f; // North
                m_flipX = false;
            }
            else if (angleDeg >= 120.f && angleDeg < 240.f)
            {
                m_targetRotation = 0.0f; // West
                m_flipX = true;
            }
            else if (angleDeg >= 240.f && angleDeg < 300.f)
            {
                m_targetRotation = 270.f; // South
                m_flipX = false;
            }
            else
            {
                m_targetRotation = 0.f; // East
                m_flipX = false;
            }
        }

        //float length = std::hypot(movement.x, movement.y);

        //if (length > 0.0001f)
        //{
        //    movement.x /= length;
        //    movement.y /= length;
        //}

        float diff_x = movement.x * m_speed * deltaTime;
        float diff_y = movement.y * m_speed * deltaTime;

        m_position.x += diff_x;
        m_position.y += diff_y;
    }

    Vector2 Player::GetPosition() const
    {
        return m_position;
    }

    float Player::GetTargetRotation() const
    {
        return m_targetRotation;
    }

    bool Player::IsFlippedX() const
    {
        return m_flipX;
    }
}