#include "../Header/CollisionController.h"
#include <algorithm>

bool CollisionController::checkCollision(const AABB& a, const AABB& b) {
    // Check if rectangles overlap on both axes
    bool collisionX = a.getRight() >= b.getLeft() && b.getRight() >= a.getLeft();
    bool collisionY = a.getBottom() >= b.getTop() && b.getBottom() >= a.getTop();

    return collisionX && collisionY;
}

bool CollisionController::pointInAABB(const glm::vec2& point, const AABB& box) {
    return (point.x >= box.getLeft() && point.x <= box.getRight() &&
        point.y >= box.getTop() && point.y <= box.getBottom());
}

bool CollisionController::checkCollisionWithPenetration(const AABB& a, const AABB& b, glm::vec2& penetration) {
    if (!checkCollision(a, b)) {
        penetration = glm::vec2(0.0f);
        return false;
    }

    // Calculate overlap on each axis
    float overlapX = std::min(a.getRight(), b.getRight()) - std::max(a.getLeft(), b.getLeft());
    float overlapY = std::min(a.getBottom(), b.getBottom()) - std::max(a.getTop(), b.getTop());

    // Find the axis with minimum penetration (that's the collision normal)
    if (overlapX < overlapY) {
        // Collision on X axis
        if (a.getCenter().x < b.getCenter().x) {
            penetration = glm::vec2(-overlapX, 0.0f);  // Push left
        }
        else {
            penetration = glm::vec2(overlapX, 0.0f);   // Push right
        }
    }
    else {
        // Collision on Y axis
        if (a.getCenter().y < b.getCenter().y) {
            penetration = glm::vec2(0.0f, -overlapY);  // Push up
        }
        else {
            penetration = glm::vec2(0.0f, overlapY);   // Push down
        }
    }

    return true;
}

void CollisionController::clampToScreen(AABB& box, float screenWidth, float screenHeight) {
    float halfWidth = box.size.x / 2.0f;
    float halfHeight = box.size.y / 2.0f;

    // Clamp to left/right edges
    if (box.position.x < halfWidth) {
        box.position.x = halfWidth;
    }
    if (box.position.x > screenWidth - halfWidth) {
        box.position.x = screenWidth - halfWidth;
    }

    // Clamp to top/bottom edges
    if (box.position.y < halfHeight) {
        box.position.y = halfHeight;
    }
    if (box.position.y > screenHeight - halfHeight) {
        box.position.y = screenHeight - halfHeight;
    }
}

void CollisionController::clampToBox(AABB& box, float minX, float minY, float maxX, float maxY) {
    float halfWidth = box.size.x / 2.0f;
    float halfHeight = box.size.y / 2.0f;

    // Clamp to left/right bounds
    if (box.position.x - halfWidth < minX) {
        box.position.x = minX + halfWidth;
    }
    if (box.position.x + halfWidth > maxX) {
        box.position.x = maxX - halfWidth;
    }

    // Clamp to top/bottom bounds
    if (box.position.y - halfHeight < minY) {
        box.position.y = minY + halfHeight;
    }
    if (box.position.y + halfHeight > maxY) {
        box.position.y = maxY - halfHeight;
    }
}