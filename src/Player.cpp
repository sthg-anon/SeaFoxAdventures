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
        , m_flipY{ false }
        , m_velocity{ 0.f, 0.f }
        , m_acceleration{ 200.0f }
        , m_drag{ 3.0f }
        , m_maxSpeed{ 100.0f }
    {
    }

    void Player::ProcessInputs()
    {
        float deltaTime = GetFrameTime();

        Vector2 input = { 0.f, 0.f };
        if (IsKeyDown(KEY_W))
        {
            input.y -= 1.f;
        }
        if (IsKeyDown(KEY_S))
        {
            input.y += 1.f;
        }
        if (IsKeyDown(KEY_A))
        {
            input.x -= 1.f;
            m_flipY = true;
        }
        if (IsKeyDown(KEY_D))
        {
            input.x += 1.f;
            m_flipY = false;
        }

        if (std::abs(input.x) > 0.f || std::abs(input.y) > 0.f)
        {
            float angleRad = std::atan2(input.y, input.x);
            float angleDeg = angleRad * 180.f / PI;
            if (angleDeg < 0.f)
            {
                angleDeg += 360.f;
            }

            if (angleDeg >= 60.f && angleDeg < 120.f)
            {
                m_targetRotation = 90.f; // North
            }
            else if (angleDeg >= 120.f && angleDeg < 240.f)
            {
                m_targetRotation = 180.f; // West
            }
            else if (angleDeg >= 240.f && angleDeg < 300.f)
            {
                m_targetRotation = 270.f; // South
            }
            else
            {
                m_targetRotation = 0.f; // East
            }
        }

        float length = std::hypot(input.x, input.y);
        if (length > 0.0001f)
        {
            input.x /= length;
            input.y /= length;
        }

        if (length > 0.f)
        {
            m_velocity.x += input.x * m_acceleration * deltaTime;
            m_velocity.y += input.y * m_acceleration * deltaTime;
        }
        else
        {
            m_velocity.x -= m_velocity.x * m_drag * deltaTime;
            m_velocity.y -= m_velocity.y * m_drag * deltaTime;
        }

        float speed = std::hypot(m_velocity.x, m_velocity.y);
        if (speed > m_maxSpeed)
        {
            float scale = m_maxSpeed / speed;
            m_velocity.x *= scale;
            m_velocity.y *= scale;
        }

        m_position.x += m_velocity.x * deltaTime;
        m_position.y += m_velocity.y * deltaTime;
    }

    Vector2 Player::GetPosition() const
    {
        return m_position;
    }

    float Player::GetTargetRotation() const
    {
        return m_targetRotation;
    }

    bool Player::IsFlippedY() const
    {
        return m_flipY;
    }
}