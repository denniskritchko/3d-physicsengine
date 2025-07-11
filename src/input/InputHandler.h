#pragma once
#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <set>

/**
 * InputHandler class manages keyboard and mouse input
 * Provides callback system for handling input events
 */
class InputHandler {
private:
    GLFWwindow* window;                                    // GLFW window handle
    std::map<int, bool> keyStates;                        // Current key states
    std::map<int, bool> previousKeyStates;                // Previous frame key states
    std::set<int> keysPressed;                            // Keys pressed this frame
    std::set<int> keysReleased;                           // Keys released this frame
    
    // Mouse state
    double mouseX, mouseY;                                // Current mouse position
    double lastMouseX, lastMouseY;                        // Previous mouse position
    double mouseDeltaX, mouseDeltaY;                      // Mouse movement delta
    bool firstMouse;                                      // First mouse movement flag
    bool mouseGrabbed;                                    // Whether mouse is grabbed
    
    // Callback functions
    std::function<void(int, int, int, int)> keyCallback;           // Key callback
    std::function<void(double, double)> mouseCallback;            // Mouse movement callback
    std::function<void(int, int, int)> mouseButtonCallback;       // Mouse button callback
    std::function<void(double, double)> scrollCallback;           // Scroll callback
    
public:
    /**
     * Constructor - initializes input handler
     * @param win GLFW window handle
     */
    InputHandler(GLFWwindow* win) 
        : window(win)
        , mouseX(0.0)
        , mouseY(0.0)
        , lastMouseX(0.0)
        , lastMouseY(0.0)
        , mouseDeltaX(0.0)
        , mouseDeltaY(0.0)
        , firstMouse(true)
        , mouseGrabbed(false) {
        
        // Set up GLFW callbacks
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, keyCallbackStatic);
        glfwSetCursorPosCallback(window, mouseCallbackStatic);
        glfwSetMouseButtonCallback(window, mouseButtonCallbackStatic);
        glfwSetScrollCallback(window, scrollCallbackStatic);
    }

    /**
     * Update input state - call this every frame
     * Processes key press/release events and updates mouse delta
     */
    void update() {
        // Update previous key states
        previousKeyStates = keyStates;
        
        // Clear frame-specific sets
        keysPressed.clear();
        keysReleased.clear();
        
        // Check for key state changes
        for (auto& pair : keyStates) {
            int key = pair.first;
            bool currentState = pair.second;
            bool previousState = previousKeyStates[key];
            
            if (currentState && !previousState) {
                keysPressed.insert(key);
            } else if (!currentState && previousState) {
                keysReleased.insert(key);
            }
        }
        
        // Update mouse delta
        if (mouseGrabbed) {
            mouseDeltaX = mouseX - lastMouseX;
            mouseDeltaY = lastMouseY - mouseY;  // Reversed since y-coordinates go from bottom to top
        } else {
            mouseDeltaX = 0.0;
            mouseDeltaY = 0.0;
        }
        
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }

    /**
     * Check if a key is currently pressed
     * @param key GLFW key code
     * @return True if key is pressed
     */
    bool isKeyPressed(int key) const {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    /**
     * Check if a key was just pressed this frame
     * @param key GLFW key code
     * @return True if key was just pressed
     */
    bool wasKeyPressed(int key) const {
        return keysPressed.count(key) > 0;
    }

    /**
     * Check if a key was just released this frame
     * @param key GLFW key code
     * @return True if key was just released
     */
    bool wasKeyReleased(int key) const {
        return keysReleased.count(key) > 0;
    }

    /**
     * Get mouse position
     * @param x Output X position
     * @param y Output Y position
     */
    void getMousePosition(double& x, double& y) const {
        x = mouseX;
        y = mouseY;
    }

    /**
     * Get mouse movement delta
     * @param deltaX Output X delta
     * @param deltaY Output Y delta
     */
    void getMouseDelta(double& deltaX, double& deltaY) const {
        deltaX = mouseDeltaX;
        deltaY = mouseDeltaY;
    }

    /**
     * Set whether the mouse cursor should be grabbed/hidden
     * @param grab True to grab mouse, false to release
     */
    void setMouseGrab(bool grab) {
        mouseGrabbed = grab;
        if (grab) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    /**
     * Check if mouse is currently grabbed
     * @return True if mouse is grabbed
     */
    bool isMouseGrabbed() const {
        return mouseGrabbed;
    }

    /**
     * Set key callback function
     * @param callback Function to call on key events
     */
    void setKeyCallback(std::function<void(int, int, int, int)> callback) {
        keyCallback = callback;
    }

    /**
     * Set mouse movement callback function
     * @param callback Function to call on mouse movement
     */
    void setMouseCallback(std::function<void(double, double)> callback) {
        mouseCallback = callback;
    }

    /**
     * Set mouse button callback function
     * @param callback Function to call on mouse button events
     */
    void setMouseButtonCallback(std::function<void(int, int, int)> callback) {
        mouseButtonCallback = callback;
    }

    /**
     * Set scroll callback function
     * @param callback Function to call on scroll events
     */
    void setScrollCallback(std::function<void(double, double)> callback) {
        scrollCallback = callback;
    }

    /**
     * Toggle mouse grab state
     */
    void toggleMouseGrab() {
        setMouseGrab(!mouseGrabbed);
    }

    /**
     * Reset mouse delta (useful when first grabbing mouse)
     */
    void resetMouseDelta() {
        mouseDeltaX = 0.0;
        mouseDeltaY = 0.0;
        firstMouse = true;
    }

private:
    /**
     * Static GLFW key callback
     * @param window GLFW window
     * @param key Key code
     * @param scancode System scancode
     * @param action Key action (press/release/repeat)
     * @param mods Modifier keys
     */
    static void keyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods) {
        InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->keyCallbackImpl(key, scancode, action, mods);
        }
    }

    /**
     * Static GLFW mouse callback
     * @param window GLFW window
     * @param xpos Mouse X position
     * @param ypos Mouse Y position
     */
    static void mouseCallbackStatic(GLFWwindow* window, double xpos, double ypos) {
        InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->mouseCallbackImpl(xpos, ypos);
        }
    }

    /**
     * Static GLFW mouse button callback
     * @param window GLFW window
     * @param button Mouse button
     * @param action Button action (press/release)
     * @param mods Modifier keys
     */
    static void mouseButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods) {
        InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->mouseButtonCallbackImpl(button, action, mods);
        }
    }

    /**
     * Static GLFW scroll callback
     * @param window GLFW window
     * @param xoffset Scroll X offset
     * @param yoffset Scroll Y offset
     */
    static void scrollCallbackStatic(GLFWwindow* window, double xoffset, double yoffset) {
        InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (handler) {
            handler->scrollCallbackImpl(xoffset, yoffset);
        }
    }

    /**
     * Implementation of key callback
     * @param key Key code
     * @param scancode System scancode
     * @param action Key action
     * @param mods Modifier keys
     */
    void keyCallbackImpl(int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            keyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            keyStates[key] = false;
        }
        
        // Call user callback if set
        if (keyCallback) {
            keyCallback(key, scancode, action, mods);
        }
    }

    /**
     * Implementation of mouse callback
     * @param xpos Mouse X position
     * @param ypos Mouse Y position
     */
    void mouseCallbackImpl(double xpos, double ypos) {
        mouseX = xpos;
        mouseY = ypos;
        
        if (firstMouse) {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }
        
        // Call user callback if set
        if (mouseCallback) {
            mouseCallback(xpos, ypos);
        }
    }

    /**
     * Implementation of mouse button callback
     * @param button Mouse button
     * @param action Button action
     * @param mods Modifier keys
     */
    void mouseButtonCallbackImpl(int button, int action, int mods) {
        // Call user callback if set
        if (mouseButtonCallback) {
            mouseButtonCallback(button, action, mods);
        }
    }

    /**
     * Implementation of scroll callback
     * @param xoffset Scroll X offset
     * @param yoffset Scroll Y offset
     */
    void scrollCallbackImpl(double xoffset, double yoffset) {
        // Call user callback if set
        if (scrollCallback) {
            scrollCallback(xoffset, yoffset);
        }
    }
}; 