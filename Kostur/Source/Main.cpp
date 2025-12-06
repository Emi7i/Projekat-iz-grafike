#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Header/Util.h"
#include "../Header/Sprite.h"
#include "../Header/CollisionController.h"
#include "../Header/MovementController.h"
#include "../Header/ClawController.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sprite Movement & Collision", NULL, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create shader program
    unsigned int spriteShader = createShader("Shaders/sprite.vert", "Shaders/sprite.frag");

    // Load textures
    unsigned int claw_tex = getPreprocessedTexture("Assets/claw.png");
    unsigned int machine_tex = getPreprocessedTexture("Assets/claw_machine.png");

    // Create claw sprite
    Sprite* claw = new Sprite(claw_tex, spriteShader);
    claw->setPosition(100, 100);
    claw->setSize(64, 64);
    claw->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Create obstacle sprite
    Sprite* machine = new Sprite(machine_tex, spriteShader);
    machine->setPosition(400, 400);
    machine->setSize(273 * 2, 372 * 2);
    machine->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Create a 1x1 white texture for debug boxes
    unsigned int whiteTexture;
    glGenTextures(1, &whiteTexture);
    glBindTexture(GL_TEXTURE_2D, whiteTexture);
    unsigned char white[] = { 255, 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);

    // Create debug box sprite
    Sprite* debugBox = new Sprite(whiteTexture, spriteShader);

    // Create movement controller for claw
    ClawController* clawMovement = new ClawController(claw, 200.0f);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float lastTime = glfwGetTime();
    bool downWasPressed = false;  // To detect single press

    while (!glfwWindowShouldClose(window))
    {
        // ===== UPDATE (Logic) =====
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Only allow left/right movement when NOT grabbing
        if (!clawMovement->isGrabbingActive()) {
            // Handle input     
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                clawMovement->moveLeft(deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                clawMovement->moveRight(deltaTime);
            }

            // Check for space press to start grab animation
            if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && !downWasPressed) {
                clawMovement->startGrab(clawMovement->getPosition().y);
                downWasPressed = true;
            }
            if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                downWasPressed = false;
            }
        }

        // Always update grab animation
        clawMovement->grabItem(deltaTime);

        // Define your movement box
        float boxMinX = 218.0f;
        float boxMinY = 130.0f;
        float boxMaxX = 668.0f;
        float boxMaxY = 450.0f;

        // Get claw bounding box and clamp to the restricted box
        AABB clawBox = clawMovement->getBoundingBox();
        CollisionController::clampToBox(clawBox, boxMinX, boxMinY, boxMaxX, boxMaxY);

        // Update claw position
        clawMovement->setPosition(clawBox.position);

        // ===== RENDER (Drawing) =====
        glClear(GL_COLOR_BUFFER_BIT);

        machine->draw(WINDOW_WIDTH, WINDOW_HEIGHT);
        claw->draw(WINDOW_WIDTH, WINDOW_HEIGHT);

        // Draw the restricted box in red
        float boxCenterX = (boxMinX + boxMaxX) / 2.0f;
        float boxCenterY = (boxMinY + boxMaxY) / 2.0f;
        float boxWidth = boxMaxX - boxMinX;
        float boxHeight = boxMaxY - boxMinY;

        debugBox->setPosition(boxCenterX, boxCenterY);
        debugBox->setSize(boxWidth, boxHeight);
        debugBox->setColor(1.0f, 0.0f, 0.0f, 0.3f);  // Red, semi-transparent
        debugBox->draw(WINDOW_WIDTH, WINDOW_HEIGHT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    delete clawMovement;
    delete claw;
    delete machine;
    delete debugBox;
    glDeleteTextures(1, &whiteTexture);
    glDeleteProgram(spriteShader);
    glDeleteTextures(1, &claw_tex);
    glDeleteTextures(1, &machine_tex);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}