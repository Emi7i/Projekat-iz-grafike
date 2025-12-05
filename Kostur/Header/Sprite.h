#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Sprite {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    unsigned int shaderProgram;

    glm::vec2 position;
    glm::vec2 size;
    float rotation;
    glm::vec4 color;

    void setupMesh();

public:
    // Constructor
    Sprite(unsigned int texture, unsigned int shader);

    // Destructor
    ~Sprite();

    // Rendering
    void draw(int windowWidth, int windowHeight);

    // Setters
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setRotation(float degrees);
    void setColor(float r, float g, float b, float a = 1.0f);

    // Getters
    glm::vec2 getPosition() const { return position; }
    glm::vec2 getSize() const { return size; }
    float getRotation() const { return rotation; }
};

#endif