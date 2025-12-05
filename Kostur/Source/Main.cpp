#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Header/Util.h"
#include "../Header/Sprite.h"
#include "../Header/CollisionController.h"
#include "../Header/MovementController.h"

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
    unsigned int player_tex = getPreprocessedTexture("Assets/bun.png");
    unsigned int machine_tex = getPreprocessedTexture("Assets/claw_machine.png");

    // Create player sprite
    Sprite* player = new Sprite(player_tex, spriteShader);
    player->setPosition(100, 100);
    player->setSize(64, 64);
    player->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Create obstacle sprite
    Sprite* machine = new Sprite(machine_tex, spriteShader);
    machine->setPosition(400, 400);
    machine->setSize(273*2, 372*2);
    machine->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Create a 1x1 white texture for debug boxes
    unsigned int whiteTexture;
    glGenTextures(1, &whiteTexture);
    glBindTexture(GL_TEXTURE_2D, whiteTexture);
    unsigned char white[] = { 255, 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);

    // Create debug box sprite
    Sprite* debugBox = new Sprite(whiteTexture, spriteShader);

    // Create movement controller for player
    MovementController* playerMovement = new MovementController(player, 200.0f);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // ===== UPDATE (Logic) =====
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Handle input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            playerMovement->moveUp(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            playerMovement->moveDown(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            playerMovement->moveLeft(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            playerMovement->moveRight(deltaTime);
        }

        // Check collision with obstacle
        AABB playerBox = playerMovement->getBoundingBox();

        // Define your movement box
        float boxMinX = 218.0f;
        float boxMinY = 130.0f;
        float boxMaxX = 668.0f;
        float boxMaxY = 450.0f;

        // Clamp player to the restricted box
        CollisionController::clampToBox(playerBox, boxMinX, boxMinY, boxMaxX, boxMaxY);

        // Update player position
        playerMovement->setPosition(playerBox.position);
        // ===== RENDER (Drawing) =====
        glClear(GL_COLOR_BUFFER_BIT);

        machine->draw(WINDOW_WIDTH, WINDOW_HEIGHT);
        player->draw(WINDOW_WIDTH, WINDOW_HEIGHT);

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
    delete playerMovement;
    delete player;
    delete machine;
    delete debugBox;
    glDeleteTextures(1, &whiteTexture);
    glDeleteProgram(spriteShader);
    glDeleteTextures(1, &player_tex);
    glDeleteTextures(1, &machine_tex);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}