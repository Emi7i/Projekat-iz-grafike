#include "CollisionController.h"
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
    // Clamp to left/right edges
    if (box.getLeft() < 0.0f) {
        box.position.x = 0.0f;
    }
    if (box.getRight() > screenWidth) {
        box.position.x = screenWidth - box.size.x;
    }

    // Clamp to top/bottom edges
    if (box.getTop() < 0.0f) {
        box.position.y = 0.0f;
    }
    if (box.getBottom() > screenHeight) {
        box.position.y = screenHeight - box.size.y;
    }
}