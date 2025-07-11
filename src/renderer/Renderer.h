#pragma once
#define _USE_MATH_DEFINES
#include "Camera.h"
#include "../physics/PhysicsWorld.h"
#include <GL/gl.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Renderer class manages all OpenGL rendering operations
 * Handles shader compilation, mesh generation, and 3D object rendering
 */
class Renderer {
private:
    // OpenGL objects
    unsigned int shaderProgram;     // Compiled shader program
    unsigned int sphereVAO;         // Vertex Array Object for sphere
    unsigned int sphereVBO;         // Vertex Buffer Object for sphere
    unsigned int sphereEBO;         // Element Buffer Object for sphere
    unsigned int cubeVAO;           // VAO for room walls
    unsigned int cubeVBO;           // VBO for room walls
    unsigned int cubeEBO;           // EBO for room walls
    
    // Sphere mesh data
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    int sphereIndexCount;
    
    // Cube mesh data (for room walls)
    std::vector<float> cubeVertices;
    std::vector<unsigned int> cubeIndices;
    int cubeIndexCount;
    
    // Lighting properties
    Vector3 lightPos;
    Vector3 lightColor;
    
    // Window properties
    int windowWidth;
    int windowHeight;
    
public:
    /**
     * Constructor - initializes the renderer
     * @param width Window width
     * @param height Window height
     */
    Renderer(int width, int height) 
        : shaderProgram(0)
        , sphereVAO(0)
        , sphereVBO(0)
        , sphereEBO(0)
        , cubeVAO(0)
        , cubeVBO(0)
        , cubeEBO(0)
        , sphereIndexCount(0)
        , cubeIndexCount(0)
        , lightPos(0.0f, 8.0f, 0.0f)
        , lightColor(1.0f, 1.0f, 1.0f)
        , windowWidth(width)
        , windowHeight(height) {
    }

    /**
     * Destructor - cleans up OpenGL resources
     */
    ~Renderer() {
        cleanup();
    }

    /**
     * Initialize the renderer - must be called after OpenGL context is created
     * @return True if initialization successful, false otherwise
     */
    bool initialize() {
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        // Enable face culling for better performance
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        
        // Set clear color to a nice sky blue
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        
        // Load and compile shaders
        if (!loadShaders()) {
            std::cerr << "Failed to load shaders!" << std::endl;
            return false;
        }
        
        // Generate sphere mesh
        generateSphere(1.0f, 32, 16);
        
        // Generate cube mesh for room walls
        generateCube();
        
        return true;
    }

    /**
     * Render the entire scene
     * @param camera Camera for view/projection matrices
     * @param physicsWorld Physics world containing all objects to render
     * @param deltaTime Time elapsed since last frame
     */
    void render(const Camera& camera, const PhysicsWorld& physicsWorld, float deltaTime) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader program
        glUseProgram(shaderProgram);
        
        // Set up view and projection matrices
        float viewMatrix[16];
        float projMatrix[16];
        camera.getViewMatrix(viewMatrix);
        camera.getProjectionMatrix((float)windowWidth / (float)windowHeight, projMatrix);
        
        // Set matrix uniforms
        setMatrix4("view", viewMatrix);
        setMatrix4("projection", projMatrix);
        
        // Set lighting uniforms
        setVector3("lightPos", lightPos);
        setVector3("lightColor", lightColor);
        setVector3("viewPos", camera.getPosition());
        setFloat("shininess", 32.0f);
        
        // Render room walls
        renderRoom(physicsWorld);
        
        // Render all balls
        renderBalls(physicsWorld);
        
        // Render crosshair/reticle
        renderCrosshair();
    }

    /**
     * Update renderer settings when window is resized
     * @param width New window width
     * @param height New window height
     */
    void updateWindowSize(int width, int height) {
        windowWidth = width;
        windowHeight = height;
        glViewport(0, 0, width, height);
    }

    /**
     * Set the position of the light source
     * @param pos New light position
     */
    void setLightPosition(const Vector3& pos) {
        lightPos = pos;
    }

    /**
     * Set the color of the light source
     * @param color New light color
     */
    void setLightColor(const Vector3& color) {
        lightColor = color;
    }

private:
    /**
     * Load and compile vertex and fragment shaders
     * @return True if successful, false otherwise
     */
    bool loadShaders() {
        // Load vertex shader source
        std::string vertexSource = loadShaderFile("shaders/vertex.glsl");
        if (vertexSource.empty()) {
            std::cerr << "Failed to load vertex shader!" << std::endl;
            return false;
        }
        
        // Load fragment shader source
        std::string fragmentSource = loadShaderFile("shaders/fragment.glsl");
        if (fragmentSource.empty()) {
            std::cerr << "Failed to load fragment shader!" << std::endl;
            return false;
        }
        
        // Compile vertex shader
        unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        if (vertexShader == 0) {
            return false;
        }
        
        // Compile fragment shader
        unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
        if (fragmentShader == 0) {
            glDeleteShader(vertexShader);
            return false;
        }
        
        // Create shader program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        // Check for linking errors
        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "Shader program linking failed: " << infoLog << std::endl;
            
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(shaderProgram);
            return false;
        }
        
        // Clean up shader objects
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        return true;
    }

    /**
     * Load shader source code from file
     * @param filePath Path to shader file
     * @return Shader source code as string
     */
    std::string loadShaderFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open shader file: " << filePath << std::endl;
            return "";
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    /**
     * Compile a shader from source code
     * @param source Shader source code
     * @param type Shader type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
     * @return Shader object ID, or 0 if compilation failed
     */
    unsigned int compileShader(const std::string& source, unsigned int type) {
        unsigned int shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        
        // Check for compilation errors
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "Shader compilation failed: " << infoLog << std::endl;
            glDeleteShader(shader);
            return 0;
        }
        
        return shader;
    }

    /**
     * Generate a sphere mesh with given parameters
     * @param radius Sphere radius
     * @param rings Number of horizontal rings
     * @param sectors Number of vertical sectors
     */
    void generateSphere(float radius, int rings, int sectors) {
        sphereVertices.clear();
        sphereIndices.clear();
        
        // Generate vertices
        for (int ring = 0; ring <= rings; ring++) {
            float phi = M_PI * ring / rings;
            for (int sector = 0; sector <= sectors; sector++) {
                float theta = 2.0f * M_PI * sector / sectors;
                
                // Calculate position
                float x = radius * sin(phi) * cos(theta);
                float y = radius * cos(phi);
                float z = radius * sin(phi) * sin(theta);
                
                // Calculate normal (same as position for unit sphere)
                float nx = x / radius;
                float ny = y / radius;
                float nz = z / radius;
                
                // Calculate texture coordinates
                float u = (float)sector / sectors;
                float v = (float)ring / rings;
                
                // Add vertex data (position, normal, texture coords)
                sphereVertices.push_back(x);
                sphereVertices.push_back(y);
                sphereVertices.push_back(z);
                sphereVertices.push_back(nx);
                sphereVertices.push_back(ny);
                sphereVertices.push_back(nz);
                sphereVertices.push_back(u);
                sphereVertices.push_back(v);
            }
        }
        
        // Generate indices
        for (int ring = 0; ring < rings; ring++) {
            for (int sector = 0; sector < sectors; sector++) {
                int first = ring * (sectors + 1) + sector;
                int second = first + sectors + 1;
                
                // First triangle
                sphereIndices.push_back(first);
                sphereIndices.push_back(second);
                sphereIndices.push_back(first + 1);
                
                // Second triangle
                sphereIndices.push_back(second);
                sphereIndices.push_back(second + 1);
                sphereIndices.push_back(first + 1);
            }
        }
        
        sphereIndexCount = sphereIndices.size();
        
        // Create OpenGL objects
        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);
        
        glBindVertexArray(sphereVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), 
                     sphereVertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), 
                     sphereIndices.data(), GL_STATIC_DRAW);
        
        // Set vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }

    /**
     * Generate a cube mesh for room walls
     */
    void generateCube() {
        // Cube vertices with positions, normals, and texture coordinates
        cubeVertices = {
            // Front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            
            // Back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            
            // Left face
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            
            // Right face
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            
            // Top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            
            // Bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f
        };
        
        // Cube indices
        cubeIndices = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Left face
            8, 9, 10, 10, 11, 8,
            // Right face
            12, 13, 14, 14, 15, 12,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            20, 21, 22, 22, 23, 20
        };
        
        cubeIndexCount = cubeIndices.size();
        
        // Create OpenGL objects
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);
        
        glBindVertexArray(cubeVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), 
                     cubeVertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned int), 
                     cubeIndices.data(), GL_STATIC_DRAW);
        
        // Set vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }

    /**
     * Render all balls in the physics world
     * @param physicsWorld Physics world containing balls
     */
    void renderBalls(const PhysicsWorld& physicsWorld) {
        std::vector<Ball*> balls = physicsWorld.getBalls();
        
        glBindVertexArray(sphereVAO);
        
        for (Ball* ball : balls) {
            if (!ball->isActive) continue;
            
            // Create model matrix for this ball
            float modelMatrix[16];
            createModelMatrix(ball->position, Vector3(ball->radius, ball->radius, ball->radius), modelMatrix);
            
            // Set uniforms
            setMatrix4("model", modelMatrix);
            setVector3("objectColor", ball->color);
            
            // Render the sphere
            glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
        }
        
        glBindVertexArray(0);
    }

    /**
     * Render the room walls
     * @param physicsWorld Physics world for boundary information
     */
    void renderRoom(const PhysicsWorld& physicsWorld) {
        const float* bounds = physicsWorld.getWorldBounds();
        
        glBindVertexArray(cubeVAO);
        
        // Set room wall color (light gray)
        setVector3("objectColor", Vector3(0.8f, 0.8f, 0.8f));
        
        // Render floor
        float floorMatrix[16];
        createModelMatrix(Vector3(0, bounds[2] - 0.1f, 0), Vector3(bounds[1] - bounds[0], 0.1f, bounds[5] - bounds[4]), floorMatrix);
        setMatrix4("model", floorMatrix);
        glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
        
        // Render ceiling
        float ceilingMatrix[16];
        createModelMatrix(Vector3(0, bounds[3] + 0.1f, 0), Vector3(bounds[1] - bounds[0], 0.1f, bounds[5] - bounds[4]), ceilingMatrix);
        setMatrix4("model", ceilingMatrix);
        glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
        
        // Render walls
        float wallThickness = 0.1f;
        
        // Left wall
        float leftWallMatrix[16];
        createModelMatrix(Vector3(bounds[0] - wallThickness, (bounds[2] + bounds[3]) / 2, 0), 
                         Vector3(wallThickness, bounds[3] - bounds[2], bounds[5] - bounds[4]), leftWallMatrix);
        setMatrix4("model", leftWallMatrix);
        glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
        
        // Right wall
        float rightWallMatrix[16];
        createModelMatrix(Vector3(bounds[1] + wallThickness, (bounds[2] + bounds[3]) / 2, 0), 
                         Vector3(wallThickness, bounds[3] - bounds[2], bounds[5] - bounds[4]), rightWallMatrix);
        setMatrix4("model", rightWallMatrix);
        glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
        
        // Back wall
        float backWallMatrix[16];
        createModelMatrix(Vector3(0, (bounds[2] + bounds[3]) / 2, bounds[4] - wallThickness), 
                         Vector3(bounds[1] - bounds[0], bounds[3] - bounds[2], wallThickness), backWallMatrix);
        setMatrix4("model", backWallMatrix);
        glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
        
        // Front wall
        float frontWallMatrix[16];
        createModelMatrix(Vector3(0, (bounds[2] + bounds[3]) / 2, bounds[5] + wallThickness), 
                         Vector3(bounds[1] - bounds[0], bounds[3] - bounds[2], wallThickness), frontWallMatrix);
        setMatrix4("model", frontWallMatrix);
        glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
    }

    /**
     * Render a simple crosshair in the center of the screen
     */
    void renderCrosshair() {
        // Switch to 2D rendering mode
        glDisable(GL_DEPTH_TEST);
        
        // Simple crosshair using OpenGL immediate mode (for simplicity)
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        
        float centerX = windowWidth / 2.0f;
        float centerY = windowHeight / 2.0f;
        float size = 10.0f;
        
        glBegin(GL_LINES);
        // Horizontal line
        glVertex2f(centerX - size, centerY);
        glVertex2f(centerX + size, centerY);
        // Vertical line
        glVertex2f(centerX, centerY - size);
        glVertex2f(centerX, centerY + size);
        glEnd();
        
        // Restore matrices
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        
        // Re-enable depth testing
        glEnable(GL_DEPTH_TEST);
    }

    /**
     * Create a model matrix for positioning and scaling an object
     * @param position Object position
     * @param scale Object scale
     * @param matrix Output matrix array
     */
    void createModelMatrix(const Vector3& position, const Vector3& scale, float* matrix) {
        // Initialize to identity matrix
        for (int i = 0; i < 16; i++) {
            matrix[i] = 0.0f;
        }
        matrix[0] = scale.x;
        matrix[5] = scale.y;
        matrix[10] = scale.z;
        matrix[15] = 1.0f;
        
        // Set translation
        matrix[12] = position.x;
        matrix[13] = position.y;
        matrix[14] = position.z;
    }

    /**
     * Set a 4x4 matrix uniform in the shader
     * @param name Uniform name
     * @param matrix Matrix data
     */
    void setMatrix4(const std::string& name, const float* matrix) {
        int location = glGetUniformLocation(shaderProgram, name.c_str());
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
        }
    }

    /**
     * Set a Vector3 uniform in the shader
     * @param name Uniform name
     * @param vector Vector data
     */
    void setVector3(const std::string& name, const Vector3& vector) {
        int location = glGetUniformLocation(shaderProgram, name.c_str());
        if (location != -1) {
            glUniform3f(location, vector.x, vector.y, vector.z);
        }
    }

    /**
     * Set a float uniform in the shader
     * @param name Uniform name
     * @param value Float value
     */
    void setFloat(const std::string& name, float value) {
        int location = glGetUniformLocation(shaderProgram, name.c_str());
        if (location != -1) {
            glUniform1f(location, value);
        }
    }

    /**
     * Clean up OpenGL resources
     */
    void cleanup() {
        if (sphereVAO != 0) {
            glDeleteVertexArrays(1, &sphereVAO);
            sphereVAO = 0;
        }
        if (sphereVBO != 0) {
            glDeleteBuffers(1, &sphereVBO);
            sphereVBO = 0;
        }
        if (sphereEBO != 0) {
            glDeleteBuffers(1, &sphereEBO);
            sphereEBO = 0;
        }
        if (cubeVAO != 0) {
            glDeleteVertexArrays(1, &cubeVAO);
            cubeVAO = 0;
        }
        if (cubeVBO != 0) {
            glDeleteBuffers(1, &cubeVBO);
            cubeVBO = 0;
        }
        if (cubeEBO != 0) {
            glDeleteBuffers(1, &cubeEBO);
            cubeEBO = 0;
        }
        if (shaderProgram != 0) {
            glDeleteProgram(shaderProgram);
            shaderProgram = 0;
        }
    }
}; 