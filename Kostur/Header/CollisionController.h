#pragma once
#ifndef COLLISION_CONTROLLER_H
#define COLLISION_CONTROLLER_H

#include <glm/glm.hpp>

// Simple axis-aligned bounding box
struct AABB {
    glm::vec2 position;  // Top-left corner
    glm::vec2 size;      // Width and height

    AABB() : position(0.0f), size(0.0f) {}
    AABB(glm::vec2 pos, glm::vec2 sz) : position(pos), size(sz) {}

    glm::vec2 getCenter() const { return position; } // position IS the center

    // Get min/max bounds
    float getLeft() const { return position.x - size.x / 2.0f; }
    float getRight() const { return position.x + size.x / 2.0f; }
    float getTop() const { return position.y - size.y / 2.0f; }
    float getBottom() const { return position.y + size.y / 2.0f; }
};

class CollisionController {
public:
    // Check if two AABBs overlap
    static bool checkCollision(const AABB& a, const AABB& b);

    // Check if point is inside AABB
    static bool pointInAABB(const glm::vec2& point, const AABB& box);

    // Check collision and get penetration depth
    static bool checkCollisionWithPenetration(const AABB& a, const AABB& b, glm::vec2& penetration);

    // Clamp AABB within bounds (useful for keeping sprite on screen)
    static void clampToScreen(AABB& box, float screenWidth, float screenHeight);

    static void clampToBox(AABB& box, float minX, float minY, float maxX, float maxY);
};

#endif