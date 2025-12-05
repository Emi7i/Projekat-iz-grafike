#include "MovementController.h"
#include "CollisionController.h"

MovementController::MovementController(Sprite* sprite, float speed)
    : sprite(sprite), velocity(0.0f), speed(speed), friction(0.9f) {
}

void MovementController::update(float deltaTime) {
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

void MovementController::moveUp(float deltaTime) {
    glm::vec2 currentPos = sprite->getPosition();
    currentPos.y -= speed * deltaTime;
    sprite->setPosition(currentPos.x, currentPos.y);
}

void MovementController::moveDown(float deltaTime) {
    glm::vec2 currentPos = sprite->getPosition();
    currentPos.y += speed * deltaTime;
    sprite->setPosition(currentPos.x, currentPos.y);
}

void MovementController::moveLeft(float deltaTime) {
    glm::vec2 currentPos = sprite->getPosition();
    currentPos.x -= speed * deltaTime;
    sprite->setPosition(currentPos.x, currentPos.y);
}

void MovementController::moveRight(float deltaTime) {
    glm::vec2 currentPos = sprite->getPosition();
    currentPos.x += speed * deltaTime;
    sprite->setPosition(currentPos.x, currentPos.y);
}

void MovementController::addVelocity(const glm::vec2& vel) {
    velocity += vel;
}

void MovementController::setVelocity(const glm::vec2& vel) {
    velocity = vel;
}

void MovementController::setPosition(const glm::vec2& pos) {
    sprite->setPosition(pos.x, pos.y);
}

glm::vec2 MovementController::getPosition() const {
    return sprite->getPosition();
}

AABB MovementController::getBoundingBox() const {
    return AABB(sprite->getPosition(), sprite->getSize());
}