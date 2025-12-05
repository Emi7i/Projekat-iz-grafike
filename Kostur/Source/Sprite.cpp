#include "../Header/Sprite.h"
#include <glm/gtc/type_ptr.hpp>

Sprite::Sprite(unsigned int texture, unsigned int shader)
    : texture(texture), shaderProgram(shader),
    position(0.0f, 0.0f), size(100.0f, 100.0f),
    rotation(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {
    setupMesh();
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Sprite::setupMesh() {
    // Vertex data: position (x, y) and texture coordinates (u, v)
    float vertices[] = {
        // positions   // texture coords
        0.5f,  0.5f,   1.0f, 1.0f,  // top right
        0.5f, -0.5f,   1.0f, 0.0f,  // bottom right
       -0.5f, -0.5f,   0.0f, 0.0f,  // bottom left
       -0.5f,  0.5f,   0.0f, 1.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sprite::draw(int windowWidth, int windowHeight) {
    glUseProgram(shaderProgram);

    // Create transformation matrix
    glm::mat4 model = glm::mat4(1.0f);

    // Translate to position
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // Rotate around center
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // Scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // Create orthographic projection matrix (pixel coordinates)
    glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth,
        (float)windowHeight, 0.0f,
        -1.0f, 1.0f);

    // Send matrices to shader
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "spriteColor");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw sprite
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sprite::setPosition(float x, float y) {
    position = glm::vec2(x, y);
}

void Sprite::setSize(float width, float height) {
    size = glm::vec2(width, height);
}

void Sprite::setRotation(float degrees) {
    rotation = degrees;
}

void Sprite::setColor(float r, float g, float b, float a) {
    color = glm::vec4(r, g, b, a);
}