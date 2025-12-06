#ifndef CLAW_CONTROLLER_H
#define CLAW_CONTROLLER_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "CollisionController.h"

class ClawController {
private:
    Sprite* sprite;
    glm::vec2 velocity;
    float speed;
    float friction;

    // For grab animation
    float grabTimer;
    bool isGrabbing;
    float startY;
    float bottomY;

public:
    ClawController(Sprite* sprite, float speed = 200.0f);

    // Update movement with velocity and friction
    void update(float deltaTime);

    // Movement controls
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    // Grab sequence
    void startGrab(float startYPos, float bottomYPos = 0.0f);  // bottomYPos not used anymore
    void grabItem(float deltaTime);
    bool isGrabbingActive() const { return isGrabbing; }

    // Velocity controls
    void addVelocity(const glm::vec2& vel);
    void setVelocity(const glm::vec2& vel);
    glm::vec2 getVelocity() const { return velocity; }

    // Position controls
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