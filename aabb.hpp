#pragma once

#include "template.h"
#include "surface.h"
#include <algorithm>




namespace Engine
{
    // forward declaration
    class Camera;

    struct AABB
    {
        // Construct AABB from 2 points.
        AABB(const Tmpl8::vec2& a, const Tmpl8::vec2& b) noexcept
            : min{ std::min(a.x, b.x), std::min(a.y, b.y) }
            , max{ std::max<float>(a.x, b.x), std::max<float>(a.y, b.y) }
        {}

        // Translate AABB.
        AABB operator+(const Tmpl8::vec2& rhs) const noexcept
        {
            return { min + rhs, max + rhs };
        }

        // Translate AABB.
        AABB& operator+=(const Tmpl8::vec2& rhs) noexcept
        {
            min += rhs;
            max += rhs;

            return *this;
        }

        // Translate AABB.
        AABB operator-(const Tmpl8::vec2& rhs) const noexcept
        {
            return { min - rhs, max - rhs };
        }

        // Translate AABB.
        AABB& operator-=(const Tmpl8::vec2& rhs) noexcept
        {
            min -= rhs;
            max -= rhs;

            return *this;
        }

        // Return the left-most point of the AABB.
        float left() const noexcept
        {
            return min.x;
        }

        // Return the right-most point of the AABB.
        float right() const noexcept
        {
            return max.x;
        }

        // Return the top-most point of the AABB.
        float top() const noexcept
        {
            return min.y;
        }

        // Return the bottom-most point of the AABB.
        float bottom() const noexcept
        {
            return max.y;
        }

        // Compute the center point of the AABB.
        Tmpl8::vec2 center() const noexcept
        {
            return (min + max) * 0.5f;
        }

        // Get the width of the AABB.
        float width() const noexcept
        {
            return max.x - min.x;
        }

        // Get the height of the AABB.
        float height() const noexcept
        {
            return max.y - min.y;
        }

        // Get the area of the AABB.
        float area() const noexcept
        {
            return width() * height();
        }

        // Get the size (the diagonal from min -> max) of the AABB.
        Tmpl8::vec2 size() const noexcept
        {
            return max - min;
        }

        // The extent of the AABB is 1/2 of the size of the AABB.
        Tmpl8::vec2 extent() const noexcept
        {
            return size() * 0.5f;
        }

        // Check if a point is inside the AABB.
        bool contains(const Tmpl8::vec2& point) const noexcept
        {
            return (point.x >= min.x && point.x <= max.x) &&
                (point.y >= min.y && point.y <= max.y);
        }

        // Check if another AABB is intersecting with this one.
        bool intersect(const AABB& other) const noexcept
        {
            return (min.x <= other.max.x && min.y <= other.max.y) &&
                (max.x >= other.min.x && max.y >= other.min.y);
        }

        // Check to see if the line {p0, p1} intersects the AABB
        bool intersect(const Tmpl8::vec2& p0, const Tmpl8::vec2& p1) const;


        // Draw the AABB to the screen.
        void draw(Tmpl8::Surface& screen, Tmpl8::Pixel color) const;
        void draw(Camera& c, Tmpl8::Pixel color) const;

        AABB() noexcept = default;
        AABB(const AABB&) noexcept = default;
        AABB(AABB&&) noexcept = default;
        ~AABB() noexcept = default;
        AABB& operator=(const AABB&) noexcept = default;
        AABB& operator=(AABB&&) noexcept = default;

        Tmpl8::vec2 min{ std::numeric_limits<float>::max() };
        Tmpl8::vec2 max{ std::numeric_limits<float>::lowest() };
    };
}