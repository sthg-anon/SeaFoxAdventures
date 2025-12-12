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

#include <gsl/assert>
#include <raylib.h>

#include "TileData.hpp"
#include "WorldCollision.hpp"
#include "World.hpp"

namespace
{
    static constexpr float Acceleration = 200.0f;
    static constexpr float MaxSpeed = 100.0f;
    static constexpr float Drag = 3.0f;
    static constexpr float CollisionBoxTolerance = 3.0f;

    // This value is used for determining if the player is moving in a cardina direction.
    // The player only drills a tile if they're moving in a cardinal direction. This prevents
    // them from drilling every tile they brush against while moving around.
    // The lower this value, the more the player needs to hit the tile "head on" to drill it.
    static constexpr float CardinalDirectionTolerance = 0.3f;
}

namespace sfa
{
    void Player::ProcessInputs()
    {
        if (m_isDrilling)
        {
            return;
        }

        float deltaTime = GetFrameTime();

        Vector2 input{ 0.f, 0.f };
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
        Vector2 moveDir{ 0.0f, 0.0f };
        if (length > 0.0001f)
        {
            moveDir.x = input.x / length;
            moveDir.y = input.y / length;
        }

        if (length > 0.f)
        {
            m_velocity.x += moveDir.x * Acceleration * deltaTime;
            m_velocity.y += moveDir.y * Acceleration * deltaTime;
        }
        else
        {
            m_velocity.x -= m_velocity.x * Drag * deltaTime;
            m_velocity.y -= m_velocity.y * Drag * deltaTime;
        }

        float speed = std::hypot(m_velocity.x, m_velocity.y);
        if (speed > MaxSpeed)
        {
            float scale = MaxSpeed / speed;
            m_velocity.x *= scale;
            m_velocity.y *= scale;
        }
    }

    void Player::MoveWithCollision(World& world)
    {
        float deltaTime = GetFrameTime();

        if (m_isDrilling)
        {
            PullPlayerThoughTile(world);
            return;
        }

        Vector2 newPos = m_position;

        float dx = m_velocity.x * deltaTime;
        if (dx != 0.0f)
        {
            float testX = newPos.x + dx;
            if (!CheckCollision(world, testX, newPos.y))
            {
                newPos.x = testX;
            }
            else
            {
                auto isDrillingStarted = TryBreakTile(world, testX, newPos.y);
                if (!isDrillingStarted)
                {
                    m_velocity.x = 0.0f;
                }
            }
        }

        float dy = m_velocity.y * deltaTime;
        if (dy != 0.0f)
        {
            float testY = newPos.y + dy;
            if (!CheckCollision(world, newPos.x, testY))
            {
                newPos.y = testY;
            }
            else
            {
                auto isDrillingStarted = TryBreakTile(world, newPos.x, testY);
                if (!isDrillingStarted)
                {
                    m_velocity.y = 0.0f;
                }
            }
        }

        m_position = newPos;
    }

    bool Player::CheckCollision(const World& world, float x, float y) const
    {
        return RectHitsSolid(world, x, y, PlayerHalfSize - CollisionBoxTolerance, PlayerHalfSize - CollisionBoxTolerance);
    }

    bool Player::TryBreakTile(World& world, float x, float y)
    {
        if (!IsMovingInCardinalDirection())
        {
            return false;
        }

        auto halfPlayer = PlayerHalfSize - CollisionBoxTolerance;

        float collisionX = x + (m_velocity.x > 0 ? halfPlayer : -halfPlayer);
        float collisionY = y + (m_velocity.y > 0 ? halfPlayer : -halfPlayer);

        auto tileX = World::PixelToTileCoord(PixelCoord{ collisionX });
        auto tileY = World::PixelToTileCoord(PixelCoord{ collisionY });

        auto pos = TilePosition{ tileX, tileY };

        if (world.IsTileSolidAt(pos) && World::IsInBounds(pos))
        {
            auto tileData = GetTileData(world.GetTile(pos));

            TraceLog(LOG_INFO, "Velocity: %.2f, %.2f", m_velocity.x, m_velocity.y);

            if (m_velocity.x > CardinalDirectionTolerance)
            {
                m_velocity.x = m_drillSpeed;
            }
            else if (m_velocity.x < -CardinalDirectionTolerance)
            {
                m_velocity.x = -m_drillSpeed;
            }
            else
            {
                m_velocity.x = 0.0f;
            }
            
            if (m_velocity.y > CardinalDirectionTolerance)
            {
                m_velocity.y = m_drillSpeed;
            }
            else if (m_velocity.y < -CardinalDirectionTolerance)
            {
                m_velocity.y = -m_drillSpeed;
            }
            else
            {
                m_velocity.y = 0.0f;
            }

            m_velocity.x *= tileData.drill_speed_multiplier;
            m_velocity.y *= tileData.drill_speed_multiplier;

            DrillTile(world, pos);
            return true;
        }

        return false;
    }

    bool Player::IsMovingInCardinalDirection() const
    {
        auto absVelX = std::abs(m_velocity.x);
        auto absVelY = std::abs(m_velocity.y);

        if (absVelX < 0.001 && absVelY < 0.001)
        {
            return false;
        }

        auto maxVel = std::max(absVelX, absVelY);
        auto minVel = std::min(absVelX, absVelY);
        auto cardinalRatio = minVel / maxVel;

        return cardinalRatio < CardinalDirectionTolerance;
    }

    void Player::DrillTile(World& world, TilePosition pos)
    {
        m_isDrilling = true;
        m_drillProgress = WorldTileSizePixels - CollisionBoxTolerance - 1.0f;
    }

    void Player::PullPlayerThoughTile(World& world)
    {
        auto absDx = std::abs(m_velocity.x);
        auto absDy = std::abs(m_velocity.y);

        Expects(absDx > 0 || absDy > 0);

        auto deltaTime = GetFrameTime();
        auto position = m_position;

        float dx = m_velocity.x * deltaTime;
        float dy = m_velocity.y * deltaTime;

        m_position.x += dx;
        m_position.y += dy;

        // This is ok to do because we only drill in cardinal directions.
        auto magnitude = std::max(std::abs(dx), std::abs(dy));
        m_drillProgress -= magnitude;

        float drillProgressRatio = 1.0f - (m_drillProgress / (WorldTileSizePixels - CollisionBoxTolerance - 1.0f));

        if (absDx > 0.5f)
        {
            // We're moving along the x axis
            auto worldY = World::PixelToTileCoord(PixelCoord{ m_position.y });
            auto targetPixelY = World::TileToPixelCoord(worldY).Get();
            m_position.y = position.y + (targetPixelY - position.y) * drillProgressRatio;
        }
        else
        {
            // Because we're moving in a cardinal direction and velocity needs to be > 0,
            // we can assume that if we're not moving on the x axis then we must be moving
            // on the y axis.

            auto worldX = World::PixelToTileCoord(PixelCoord{ m_position.x });
            auto targetPixelX = World::TileToPixelCoord(worldX).Get();
            m_position.x = position.x + (targetPixelX - position.x) * drillProgressRatio;
        }

        if (m_drillProgress <= 0.0f)
        {
            m_isDrilling = false;
            m_velocity.x = 0.0f;
            m_velocity.y = 0.0f;

            auto tileX = World::PixelToTileCoord(PixelCoord{ m_position.x });
            auto tileY = World::PixelToTileCoord(PixelCoord{ m_position.y });

            auto pos = TilePosition{ tileX, tileY };

            world.SetTile(pos, TileType::UnderWater);
        }
    }
}