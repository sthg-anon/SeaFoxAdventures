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

#include "Coordinate.hpp"

namespace sfa
{
    class TileIterator
    {
    private:
        TilePosition m_current;
        TileCoord m_minX;
        TileCoord m_maxX;
        TileCoord m_maxY;

    public:
        TileIterator(TileCoord startX, TileCoord startY, TileCoord maxX, TileCoord maxY)
            : m_current{ startX, startY }
            , m_minX{ startX }
            , m_maxX{ maxX }
            , m_maxY{ maxY }
        {
        }

        const TilePosition& operator*() const
        {
            return m_current;
        }

        TileIterator& operator++()
        {
            m_current.x = TileCoord(m_current.x.Get() + 1);

            if (m_current.x.Get() >= m_maxX.Get())
            {
                m_current.x = m_minX;
                m_current.y = TileCoord{ m_current.y.Get() + 1 };
            }

            return *this;
        }

        bool operator==(const TileIterator& other) const
        {
            return m_current.y.Get() >= other.m_current.y.Get();
        }

        bool operator!=(const TileIterator& other) const
        {
            return !(*this == other);
        }
    };

    class TileRange
    {
    private:
        TileCoord m_minX, m_minY, m_maxX, m_maxY;

    public:
        TileRange(TileCoord minX, TileCoord minY, TileCoord maxX, TileCoord maxY)
            : m_minX{ minX }
            , m_minY{ minY }
            , m_maxX{ maxX }
            , m_maxY{ maxY }
        {
        }

        TileIterator begin() const
        {
            return TileIterator(m_minX, m_minY, m_maxX, m_maxY);
        }

        TileIterator end() const
        {
            return TileIterator(m_minX, m_maxY, m_maxX, m_maxY);
        }
    };
}