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
    unsigned int texture1 = getPreprocessedTexture("Assets/bun.png");
    unsigned int texture2 = getPreprocessedTexture("Assets/cart.png");

    // Create player sprite
    Sprite* player = new Sprite(texture1, spriteShader);
    player->setPosition(100, 100);
    player->setSize(64, 64);
    player->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Create obstacle sprite
    Sprite* obstacle = new Sprite(texture2, spriteShader);
    obstacle->setPosition(400, 400);
    obstacle->setSize(100, 100);
    obstacle->setColor(1.0f, 1.0f, 1.0f, 1.0f);

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

        // Update movement (if using velocity)
        playerMovement->update(deltaTime);

        // Check collision with obstacle
        AABB playerBox = playerMovement->getBoundingBox();
        AABB obstacleBox(obstacle->getPosition(), obstacle->getSize());

        glm::vec2 penetration;
        if (CollisionController::checkCollisionWithPenetration(playerBox, obstacleBox, penetration)) {
            // Push player out of obstacle
            glm::vec2 newPos = playerMovement->getPosition() + penetration;
            playerMovement->setPosition(newPos);

            // Change player color when colliding
            player->setColor(1.0f, 0.0f, 1.0f, 1.0f);  // Magenta
        }
        else {
            player->setColor(1.0f, 1.0f, 1.0f, 1.0f);  // White
        }

        // Keep player on screen
        playerBox = playerMovement->getBoundingBox();
        CollisionController::clampToScreen(playerBox, WINDOW_WIDTH, WINDOW_HEIGHT);
        playerMovement->setPosition(playerBox.position);

        // ===== RENDER (Drawing) =====
        glClear(GL_COLOR_BUFFER_BIT);

        obstacle->draw(WINDOW_WIDTH, WINDOW_HEIGHT);
        player->draw(WINDOW_WIDTH, WINDOW_HEIGHT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    delete playerMovement;
    delete player;
    delete obstacle;
    glDeleteProgram(spriteShader);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}