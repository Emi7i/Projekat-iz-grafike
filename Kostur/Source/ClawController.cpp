#include "../Header/ClawController.h"
#include "../Header/MovementController.h"
#include <algorithm>

ClawController::ClawController(Sprite* sprite, float speed)
    : sprite(sprite), velocity(0.0f), speed(speed), friction(0.9f),
    grabTimer(0.0f), isGrabbing(false), startY(0.0f), bottomY(0.0f) {
}

void ClawController::startGrab(float startYPos, float bottomYPos) {
    if (!isGrabbing) {  // Only start if not already grabbing
        isGrabbing = true;
        grabTimer = 0.0f;
        startY = startYPos;
        bottomY = bottomYPos;
    }
}

void ClawController::grabItem(float deltaTime) {
    if (!isGrabbing) return;

    grabTimer += deltaTime;

    float totalDuration = 3.1f;
    float halfDuration = totalDuration / 2.0f;
    float moveDistance = 250.0f; 

    if (grabTimer < halfDuration) {
        // First half: Move DOWN
        float descendProgress = grabTimer / halfDuration;  // 0 to 1
        float currentY = startY + (moveDistance * descendProgress);
        sprite->setPosition(sprite->getPosition().x, currentY);
    }
    else if (grabTimer < totalDuration) {
        // Second half: Move UP 
        float ascendProgress = (grabTimer - halfDuration) / halfDuration;  // 0 to 1
        float currentY = (startY + moveDistance) - (moveDistance * ascendProgress);
        sprite->setPosition(sprite->getPosition().x, currentY);
    }
    else {
        // Animation complete
        isGrabbing = false;
        grabTimer = 0.0f;
        sprite->setPosition(sprite->getPosition().x, startY);  // Ensure we're at start
    }
}

void ClawController::update(float deltaTime) {
    // Apply velocity to position
    glm::vec2 currentPos = sprite->getPosition();
    currentPos += velocity * deltaTime;
    sprite->setPosition(currentPos.x, currentPos.y);

    // Apply friction to slow down velocity
    velocity *= friction;

    // Stop completely if moving very slowly
    if (glm::length(velocity) < 1.0f) {
        velocity = glm::vec2(0.0f);
    }
}

void ClawController::moveLeft(float deltaTime) {
    glm::vec2 currentPos = sprite->getPosition();
    currentPos.x -= speed * deltaTime;
    sprite->setPosition(currentPos.x, currentPos.y);
}

void ClawController::moveRight(float deltaTime) {
    glm::vec2 currentPos = sprite->getPosition();
    currentPos.x += speed * deltaTime;
    sprite->setPosition(currentPos.x, currentPos.y);
}

void ClawController::addVelocity(const glm::vec2& vel) {
    velocity += vel;
}

void ClawController::setVelocity(const glm::vec2& vel) {
    velocity = vel;
}

void ClawController::setPosition(const glm::vec2& pos) {
    sprite->setPosition(pos.x, pos.y);
}

glm::vec2 ClawController::getPosition() const {
    return sprite->getPosition();
}

AABB ClawController::getBoundingBox() const {
    return AABB(sprite->getPosition(), sprite->getSize());
}
