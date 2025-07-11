#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <chrono>
#include <random>
#include <iostream>
#include "../physics/PhysicsWorld.h"
#include "../renderer/Renderer.h"
#include "../renderer/Camera.h"
#include "../input/InputHandler.h"
#include "../console/Console.h"

/**
 * Game class is the main controller for the 3D physics engine
 * Manages all subsystems and handles the main game loop
 */
class Game {
private:
    // Window and OpenGL context
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    
    // Core systems
    std::unique_ptr<PhysicsWorld> physicsWorld;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<InputHandler> inputHandler;
    std::unique_ptr<Console> console;
    
    // Game state
    bool isRunning;
    bool isPaused;
    
    // Player state
    Ball* heldBall;                    // Ball currently held by player
    float pickupRange;                 // Range for picking up balls
    float throwForce;                  // Force to apply when throwing
    
    // Timing
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime;
    float targetFrameTime;
    
    // Random number generation
    std::random_device randomDevice;
    std::mt19937 randomGenerator;
    
public:
    /**
     * Constructor - initializes the game
     */
    Game() 
        : window(nullptr)
        , windowWidth(1280)
        , windowHeight(720)
        , isRunning(false)
        , isPaused(false)
        , heldBall(nullptr)
        , pickupRange(3.0f)
        , throwForce(15.0f)
        , deltaTime(0.0f)
        , targetFrameTime(1.0f / 60.0f)
        , randomGenerator(randomDevice()) {
    }

    /**
     * Destructor - cleans up resources
     */
    ~Game() {
        shutdown();
    }

    /**
     * Initialize the game engine
     * @return True if initialization successful, false otherwise
     */
    bool Initialize() {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return false;
        }

        // Set OpenGL version and profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        // Create window
        window = glfwCreateWindow(windowWidth, windowHeight, "3D Physics Engine", NULL, NULL);
        if (!window) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return false;
        }

        // Make context current
        glfwMakeContextCurrent(window);
        
        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            return false;
        }

        // Set up viewport
        glViewport(0, 0, windowWidth, windowHeight);
        
        // Enable V-Sync
        glfwSwapInterval(1);
        
        // Initialize core systems
        physicsWorld = std::make_unique<PhysicsWorld>();
        renderer = std::make_unique<Renderer>(windowWidth, windowHeight);
        camera = std::make_unique<Camera>(Vector3(0.0f, 3.0f, 8.0f));
        inputHandler = std::make_unique<InputHandler>(window);
        console = std::make_unique<Console>();
        
        // Initialize renderer
        if (!renderer->initialize()) {
            std::cerr << "Failed to initialize renderer!" << std::endl;
            return false;
        }
        
        // Set up input callbacks
        setupInputCallbacks();
        
        // Register console commands
        setupConsoleCommands();
        
        // Set up initial scene
        setupScene();
        
        // Initialize timing
        lastFrameTime = std::chrono::high_resolution_clock::now();
        
        std::cout << "3D Physics Engine initialized successfully!" << std::endl;
        std::cout << "Press ~ to open console, ESC to exit" << std::endl;
        
        return true;
    }

    /**
     * Run the main game loop
     */
    void Run() {
        isRunning = true;
        
        while (isRunning && !glfwWindowShouldClose(window)) {
            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
            lastFrameTime = currentTime;
            
            // Cap delta time to prevent large jumps
            if (deltaTime > 0.1f) {
                deltaTime = 0.1f;
            }
            
            // Process input
            glfwPollEvents();
            inputHandler->update();
            
            // Update game systems
            update(deltaTime);
            
            // Render the scene
            render();
            
            // Swap buffers
            glfwSwapBuffers(window);
        }
    }

    /**
     * Shutdown the game engine
     */
    void shutdown() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }

private:
    /**
     * Update game systems
     * @param dt Delta time since last update
     */
    void update(float dt) {
        if (isPaused) return;
        
        // Update camera based on input
        updateCamera(dt);
        
        // Update player actions
        updatePlayer(dt);
        
        // Update physics simulation
        physicsWorld->update(dt);
        
        // Update held ball position
        if (heldBall) {
            Vector3 cameraPos = camera->getPosition();
            Vector3 cameraFront = camera->getFront();
            heldBall->position = cameraPos + cameraFront * 2.0f;
        }
    }

    /**
     * Render the scene
     */
    void render() {
        renderer->render(*camera, *physicsWorld, deltaTime);
        
        // Render console if visible
        if (console->getVisible()) {
            renderConsole();
        }
    }

    /**
     * Update camera based on input
     * @param dt Delta time
     */
    void updateCamera(float dt) {
        if (console->getActive()) return;  // Don't move camera when console is active
        
        // Camera movement
        if (inputHandler->isKeyPressed(GLFW_KEY_W)) {
            camera->processKeyboard('W', dt);
        }
        if (inputHandler->isKeyPressed(GLFW_KEY_S)) {
            camera->processKeyboard('S', dt);
        }
        if (inputHandler->isKeyPressed(GLFW_KEY_A)) {
            camera->processKeyboard('A', dt);
        }
        if (inputHandler->isKeyPressed(GLFW_KEY_D)) {
            camera->processKeyboard('D', dt);
        }
        if (inputHandler->isKeyPressed(GLFW_KEY_SPACE)) {
            camera->processKeyboard('Q', dt);
        }
        if (inputHandler->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            camera->processKeyboard('E', dt);
        }
        
        // Mouse look
        if (inputHandler->isMouseGrabbed()) {
            double deltaX, deltaY;
            inputHandler->getMouseDelta(deltaX, deltaY);
            camera->processMouseMovement(deltaX, deltaY);
        }
    }

    /**
     * Update player actions
     * @param dt Delta time
     */
    void updatePlayer(float dt) {
        // Pick up/drop balls with E key
        if (inputHandler->wasKeyPressed(GLFW_KEY_E)) {
            if (heldBall) {
                // Drop the held ball
                heldBall->setHeld(false);
                heldBall = nullptr;
            } else {
                // Try to pick up a nearby ball
                Ball* nearestBall = findNearestBall();
                if (nearestBall) {
                    heldBall = nearestBall;
                    heldBall->setHeld(true);
                }
            }
        }
        
        // Throw held ball with F key
        if (inputHandler->wasKeyPressed(GLFW_KEY_F) && heldBall) {
            Vector3 throwVelocity = camera->getFront() * throwForce;
            heldBall->throwBall(throwVelocity);
            heldBall = nullptr;
        }
    }

    /**
     * Find the nearest ball to the player
     * @return Pointer to nearest ball, or nullptr if none in range
     */
    Ball* findNearestBall() {
        Vector3 cameraPos = camera->getPosition();
        std::vector<Ball*> balls = physicsWorld->getBalls();
        
        Ball* nearestBall = nullptr;
        float nearestDistance = pickupRange;
        
        for (Ball* ball : balls) {
            if (ball->isHeld) continue;  // Skip already held balls
            
            float distance = (ball->position - cameraPos).magnitude();
            if (distance < nearestDistance) {
                nearestDistance = distance;
                nearestBall = ball;
            }
        }
        
        return nearestBall;
    }

    /**
     * Set up input callbacks
     */
    void setupInputCallbacks() {
        // Key callback
        inputHandler->setKeyCallback([this](int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS) {
                switch (key) {
                    case GLFW_KEY_ESCAPE:
                        isRunning = false;
                        break;
                    case GLFW_KEY_GRAVE_ACCENT:  // ~ key
                        console->toggle();
                        inputHandler->setMouseGrab(!console->getActive());
                        if (console->getActive()) {
                            inputHandler->resetMouseDelta();
                        }
                        break;
                    case GLFW_KEY_P:
                        isPaused = !isPaused;
                        break;
                }
            }
        });
        
        // Mouse scroll callback
        inputHandler->setScrollCallback([this](double xoffset, double yoffset) {
            if (!console->getActive()) {
                camera->processMouseScroll(yoffset);
            }
        });
        
        // Initially grab mouse
        inputHandler->setMouseGrab(true);
    }

    /**
     * Set up console commands
     */
    void setupConsoleCommands() {
        // Summon command
        console->registerCommand("summon", [this](const std::vector<std::string>& args) {
            if (args.empty()) {
                console->addOutput("Usage: summon <number>");
                return;
            }
            
            try {
                int count = std::stoi(args[0]);
                if (count <= 0) {
                    console->addOutput("Number must be positive");
                    return;
                }
                if (count > 100) {
                    console->addOutput("Maximum 100 balls at once");
                    return;
                }
                
                summonBalls(count);
                console->addOutput("Summoned " + std::to_string(count) + " balls");
                
            } catch (const std::exception& e) {
                console->addOutput("Invalid number: " + args[0]);
            }
        });
        
        // Clear balls command
        console->registerCommand("clear_balls", [this](const std::vector<std::string>& args) {
            physicsWorld->clear();
            heldBall = nullptr;
            console->addOutput("Cleared all balls");
        });
        
        // Physics info command
        console->registerCommand("physics_info", [this](const std::vector<std::string>& args) {
            size_t ballCount = physicsWorld->getBalls().size();
            console->addOutput("Physics Info:");
            console->addOutput("  Balls: " + std::to_string(ballCount));
            console->addOutput("  Held ball: " + (heldBall ? "Yes" : "No"));
        });
    }

    /**
     * Set up the initial scene
     */
    void setupScene() {
        // Create a few initial balls
        summonBalls(5);
    }

    /**
     * Summon balls at random positions
     * @param count Number of balls to summon
     */
    void summonBalls(int count) {
        Vector3 cameraPos = camera->getPosition();
        std::uniform_real_distribution<float> posDistribution(-5.0f, 5.0f);
        std::uniform_real_distribution<float> heightDistribution(2.0f, 8.0f);
        
        for (int i = 0; i < count; i++) {
            Vector3 position(
                cameraPos.x + posDistribution(randomGenerator),
                heightDistribution(randomGenerator),
                cameraPos.z + posDistribution(randomGenerator)
            );
            
            Ball* ball = physicsWorld->createBall(position);
            
            // Give the ball a small random initial velocity
            std::uniform_real_distribution<float> velDistribution(-2.0f, 2.0f);
            ball->velocity = Vector3(
                velDistribution(randomGenerator),
                0.0f,
                velDistribution(randomGenerator)
            );
        }
    }

    /**
     * Render console overlay
     */
    void renderConsole() {
        // This would normally use a text rendering system
        // For now, we'll just output to stdout when console is toggled
        // In a full implementation, you'd render text to the screen
        static bool consoleOutputted = false;
        
        if (!consoleOutputted) {
            std::cout << "Console is now visible (text rendering not implemented in this demo)" << std::endl;
            consoleOutputted = true;
        }
    }
}; 