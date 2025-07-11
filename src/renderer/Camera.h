#pragma once
#define _USE_MATH_DEFINES
#include "../physics/Vector3.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Camera class for 3D perspective rendering
 * Provides first-person camera controls and view/projection matrix calculations
 */
class Camera {
private:
    Vector3 position;        // Camera position in world space
    Vector3 front;           // Camera front direction (normalized)
    Vector3 up;              // Camera up direction (normalized)
    Vector3 right;           // Camera right direction (normalized)
    Vector3 worldUp;         // World up direction (constant)
    
    float yaw;               // Horizontal rotation (degrees)
    float pitch;             // Vertical rotation (degrees)
    float fov;               // Field of view (degrees)
    float nearPlane;         // Near clipping plane
    float farPlane;          // Far clipping plane
    
    // Camera movement speeds
    float movementSpeed;     // Movement speed
    float mouseSensitivity;  // Mouse sensitivity for looking around
    
    // Constraint values
    float maxPitch;          // Maximum pitch angle (degrees)
    float minPitch;          // Minimum pitch angle (degrees)
    
public:
    /**
     * Constructor with default values
     * @param pos Initial camera position
     * @param worldUp World up direction
     * @param yaw Initial yaw angle
     * @param pitch Initial pitch angle
     */
    Camera(Vector3 pos = Vector3(0.0f, 2.0f, 10.0f), 
           Vector3 worldUp = Vector3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, 
           float pitch = 0.0f)
        : position(pos)
        , worldUp(worldUp)
        , yaw(yaw)
        , pitch(pitch)
        , fov(45.0f)
        , nearPlane(0.1f)
        , farPlane(1000.0f)
        , movementSpeed(5.0f)
        , mouseSensitivity(0.1f)
        , maxPitch(89.0f)
        , minPitch(-89.0f) {
        updateCameraVectors();
    }

    /**
     * Get the view matrix for rendering
     * @return 4x4 view matrix as float array (column-major order)
     */
    void getViewMatrix(float* matrix) const {
        // Create look-at matrix
        Vector3 target = position + front;
        
        // Calculate camera basis vectors
        Vector3 zAxis = (position - target).normalized();  // Points towards camera
        Vector3 xAxis = up.cross(zAxis).normalized();      // Points to the right
        Vector3 yAxis = zAxis.cross(xAxis);                // Points up
        
        // Create view matrix (inverse of camera transform)
        matrix[0] = xAxis.x;    matrix[4] = xAxis.y;    matrix[8] = xAxis.z;     matrix[12] = -xAxis.dot(position);
        matrix[1] = yAxis.x;    matrix[5] = yAxis.y;    matrix[9] = yAxis.z;     matrix[13] = -yAxis.dot(position);
        matrix[2] = zAxis.x;    matrix[6] = zAxis.y;    matrix[10] = zAxis.z;    matrix[14] = -zAxis.dot(position);
        matrix[3] = 0.0f;       matrix[7] = 0.0f;       matrix[11] = 0.0f;       matrix[15] = 1.0f;
    }

    /**
     * Get the projection matrix for rendering
     * @param aspectRatio Aspect ratio of the viewport
     * @return 4x4 projection matrix as float array (column-major order)
     */
    void getProjectionMatrix(float aspectRatio, float* matrix) const {
        // Create perspective projection matrix
        float fovRad = fov * M_PI / 180.0f;
        float f = 1.0f / tan(fovRad / 2.0f);
        float zRange = nearPlane - farPlane;
        
        // Initialize to zero
        for (int i = 0; i < 16; i++) {
            matrix[i] = 0.0f;
        }
        
        // Set perspective projection values
        matrix[0] = f / aspectRatio;
        matrix[5] = f;
        matrix[10] = (farPlane + nearPlane) / zRange;
        matrix[11] = -1.0f;
        matrix[14] = (2.0f * farPlane * nearPlane) / zRange;
    }

    /**
     * Process keyboard input for camera movement
     * @param direction Movement direction (WASD)
     * @param deltaTime Time elapsed since last frame
     */
    void processKeyboard(char direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        
        switch (direction) {
            case 'W': // Forward
                position += front * velocity;
                break;
            case 'S': // Backward
                position -= front * velocity;
                break;
            case 'A': // Left
                position -= right * velocity;
                break;
            case 'D': // Right
                position += right * velocity;
                break;
            case 'Q': // Up
                position += worldUp * velocity;
                break;
            case 'E': // Down
                position -= worldUp * velocity;
                break;
        }
    }

    /**
     * Process mouse movement for camera rotation
     * @param xOffset Mouse X offset
     * @param yOffset Mouse Y offset
     * @param constrainPitch Whether to constrain pitch angle
     */
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;
        
        yaw += xOffset;
        pitch += yOffset;
        
        // Constrain pitch to prevent screen flipping
        if (constrainPitch) {
            if (pitch > maxPitch) {
                pitch = maxPitch;
            }
            if (pitch < minPitch) {
                pitch = minPitch;
            }
        }
        
        // Update camera vectors
        updateCameraVectors();
    }

    /**
     * Process mouse scroll for zoom (FOV changes)
     * @param yOffset Scroll wheel offset
     */
    void processMouseScroll(float yOffset) {
        fov -= yOffset;
        
        // Constrain FOV
        if (fov < 1.0f) {
            fov = 1.0f;
        }
        if (fov > 120.0f) {
            fov = 120.0f;
        }
    }

    /**
     * Get camera position
     * @return Current camera position
     */
    Vector3 getPosition() const {
        return position;
    }

    /**
     * Get camera front direction
     * @return Camera front direction vector
     */
    Vector3 getFront() const {
        return front;
    }

    /**
     * Get camera up direction
     * @return Camera up direction vector
     */
    Vector3 getUp() const {
        return up;
    }

    /**
     * Get camera right direction
     * @return Camera right direction vector
     */
    Vector3 getRight() const {
        return right;
    }

    /**
     * Set camera position
     * @param pos New camera position
     */
    void setPosition(const Vector3& pos) {
        position = pos;
    }

    /**
     * Set camera movement speed
     * @param speed New movement speed
     */
    void setMovementSpeed(float speed) {
        movementSpeed = speed;
    }

    /**
     * Set mouse sensitivity
     * @param sensitivity New mouse sensitivity
     */
    void setMouseSensitivity(float sensitivity) {
        mouseSensitivity = sensitivity;
    }

    /**
     * Get current field of view
     * @return FOV in degrees
     */
    float getFOV() const {
        return fov;
    }

    /**
     * Set field of view
     * @param newFOV New FOV in degrees
     */
    void setFOV(float newFOV) {
        fov = newFOV;
        
        // Constrain FOV
        if (fov < 1.0f) {
            fov = 1.0f;
        }
        if (fov > 120.0f) {
            fov = 120.0f;
        }
    }

    /**
     * Get camera orientation as yaw/pitch angles
     * @param outYaw Output yaw angle
     * @param outPitch Output pitch angle
     */
    void getOrientation(float& outYaw, float& outPitch) const {
        outYaw = yaw;
        outPitch = pitch;
    }

    /**
     * Set camera orientation
     * @param newYaw New yaw angle
     * @param newPitch New pitch angle
     */
    void setOrientation(float newYaw, float newPitch) {
        yaw = newYaw;
        pitch = newPitch;
        
        // Constrain pitch
        if (pitch > maxPitch) {
            pitch = maxPitch;
        }
        if (pitch < minPitch) {
            pitch = minPitch;
        }
        
        updateCameraVectors();
    }

private:
    /**
     * Update camera direction vectors based on yaw and pitch
     */
    void updateCameraVectors() {
        // Calculate new front vector
        Vector3 newFront;
        newFront.x = cos(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
        newFront.y = sin(pitch * M_PI / 180.0f);
        newFront.z = sin(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
        
        front = newFront.normalized();
        
        // Calculate right and up vectors
        right = front.cross(worldUp).normalized();
        up = right.cross(front).normalized();
    }
}; 