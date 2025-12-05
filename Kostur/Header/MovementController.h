#pragma once
#ifndef MOVEMENT_CONTROLLER_H
#define MOVEMENT_CONTROLLER_H

#include <glm/glm.hpp>
#include "../Header/Sprite.h"
#include "CollisionController.h"

class MovementController {
private:
    Sprite* sprite;
    glm::vec2 velocity;
    float speed;
    float friction;  // How quickly sprite slows down (0-1)

public:
    MovementController(Sprite* sprite, float speed = 300.0f);

    // Update position based on velocity and deltaTime
    void update(float deltaTime);

    // Movement controls
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    // Apply velocity-based movement (for physics)
    void addVelocity(const glm::vec2& vel);
    void setVelocity(const glm::vec2& vel);
    glm::vec2 getVelocity() const { return velocity; }

    // Direct position control
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;

    // Settings
    void setSpeed(float spd) { speed = spd; }
    void setFriction(float fric) { friction = fric; }
    float getSpeed() const { return speed; }

    // Get bounding box for collision
    AABB getBoundingBox() const;
};

#endif