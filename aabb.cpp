#include "aabb.hpp"
#include "Camera.hpp"

bool Engine::AABB::intersect(const Tmpl8::vec2& p0, const Tmpl8::vec2& p1) const
{
    // Find min and max X for the segment
    float minX = p0.x;
    float maxX = p1.x;

    if (p0.x > p1.x)
    {
        minX = p1.x;
        maxX = p0.x;
    }

    // Find the intersection of the segment's and rectangle's x-projections
    if (maxX > max.x)
    {
        maxX = max.x;
    }

    if (minX < min.x)
    {
        minX = min.x;
    }

    if (minX > maxX) // If their projections do not intersect return false
    {
        return false;
    }

    // Find corresponding min and max Y for min and max X we found before
    float minY = p0.y;
    float maxY = p1.y;

    float dx = p1.x - p0.x;

    if (std::abs(dx) > 0.0000001f)
    {
        float a = (p1.y - p0.y) / dx;
        float b = p0.y - a * p0.x;
        minY = a * minX + b;
        maxY = a * maxX + b;
    }

    if (minY > maxY)
    {
        std::swap(minY, maxY);
    }

    // Find the intersection of the segment's and rectangle's y-projections
    if (maxY > max.y)
    {
        maxY = max.y;
    }

    if (minY < min.y)
    {
        minY = min.y;
    }

    if (minY > maxY) // If Y-projections do not intersect return false
    {
        return false;
    }

    return true;
}


void Engine::AABB::draw(Tmpl8::Surface& screen, Tmpl8::Pixel color) const
{
    screen.Box(
        static_cast<int>(min.x), static_cast<int>(min.y),
        static_cast<int>(max.x), static_cast<int>(max.y),
        color
    );
}

void Engine::AABB::draw(Camera& c, Tmpl8::Pixel color) const
{
    c.drawBoxWorldSpace(min, max, color);
}
