#pragma once
#include "PhysicsBody.h"
#include <random>
#include <string>

/**
 * Ball class representing a bouncy sphere in the physics simulation
 * Extends PhysicsBody with ball-specific properties and behaviors
 */
class Ball : public PhysicsBody {
public:
    Vector3 color;          // RGB color of the ball (0.0 to 1.0)
    bool isHeld;            // True if ball is being held by player
    float spinDamping;      // How quickly the ball stops spinning
    static int ballCount;   // Static counter for total balls created
    int ballId;             // Unique identifier for this ball

    /**
     * Default constructor - creates a ball with random color
     */
    Ball() : PhysicsBody(), isHeld(false), spinDamping(0.95f) {
        ballId = ++ballCount;
        generateRandomColor();
        
        // Set ball-specific physics properties
        mass = 0.5f;            // 500 grams
        radius = 0.25f;         // 25 cm radius
        restitution = 0.8f;     // Very bouncy
        friction = 0.3f;        // Low friction for rolling
    }

    /**
     * Constructor with position
     * @param pos Initial position of the ball
     */
    Ball(const Vector3& pos) : PhysicsBody(pos, 0.5f, 0.25f), isHeld(false), spinDamping(0.95f) {
        ballId = ++ballCount;
        generateRandomColor();
        
        // Set ball-specific physics properties
        restitution = 0.8f;     // Very bouncy
        friction = 0.3f;        // Low friction for rolling
    }

    /**
     * Constructor with position and color
     * @param pos Initial position of the ball
     * @param col RGB color of the ball
     */
    Ball(const Vector3& pos, const Vector3& col) 
        : PhysicsBody(pos, 0.5f, 0.25f), color(col), isHeld(false), spinDamping(0.95f) {
        ballId = ++ballCount;
        
        // Set ball-specific physics properties
        restitution = 0.8f;     // Very bouncy
        friction = 0.3f;        // Low friction for rolling
    }

    /**
     * Generate a random bright color for the ball
     */
    void generateRandomColor() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(0.3f, 1.0f);
        
        color = Vector3(dis(gen), dis(gen), dis(gen));
    }

    /**
     * Make the ball bounce with enhanced bounciness
     * @param normal Surface normal to bounce off
     * @param impactVelocity Velocity at impact
     */
    void bounce(const Vector3& normal, float impactVelocity) {
        // Calculate bounce direction
        Vector3 bounceDirection = velocity - normal * (2.0f * velocity.dot(normal));
        
        // Apply restitution with some randomness for realistic bounce
        float bounceStrength = restitution * impactVelocity;
        
        // Add slight randomness to make bounces more interesting
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> randomness(-0.1f, 0.1f);
        
        Vector3 randomVector(randomness(gen), randomness(gen), randomness(gen));
        bounceDirection += randomVector;
        
        velocity = bounceDirection.normalized() * bounceStrength;
    }

    /**
     * Set the ball as held by the player
     * @param held True if ball is being held
     */
    void setHeld(bool held) {
        isHeld = held;
        if (held) {
            // Stop the ball when picked up
            velocity = Vector3::ZERO;
            acceleration = Vector3::ZERO;
            force = Vector3::ZERO;
        }
    }

    /**
     * Throw the ball with given velocity
     * @param throwVelocity Velocity to throw the ball with
     */
    void throwBall(const Vector3& throwVelocity) {
        if (isHeld) {
            isHeld = false;
            velocity = throwVelocity;
            
            // Add slight upward component to make throwing feel natural
            velocity.y += 2.0f;
        }
    }

    /**
     * Update ball physics with ball-specific behaviors
     * @param deltaTime Time step in seconds
     */
    void update(float deltaTime) override {
        if (isHeld) {
            // Ball is being held, don't update physics
            return;
        }
        
        // Call parent update for basic physics
        PhysicsBody::update(deltaTime);
        
        // Apply spin damping
        velocity *= spinDamping;
        
        // Ensure ball doesn't fall through floor (basic floor collision)
        if (position.y < radius) {
            position.y = radius;
            if (velocity.y < 0) {
                velocity.y = -velocity.y * restitution;
                
                // Add bounce sound trigger or effect here if needed
                if (velocity.y > 1.0f) {
                    // Significant bounce - could trigger sound effect
                }
            }
        }
    }

    /**
     * Check if ball is moving (has significant velocity)
     * @return True if ball is moving
     */
    bool isMoving() const {
        return velocity.magnitude() > 0.1f;
    }

    /**
     * Get the kinetic energy of the ball
     * @return Kinetic energy (0.5 * mass * velocity^2)
     */
    float getKineticEnergy() const {
        return 0.5f * mass * velocity.magnitudeSquared();
    }

    /**
     * Reset ball to a safe position (used when ball gets stuck)
     * @param safePosition Position to reset to
     */
    void resetToPosition(const Vector3& safePosition) {
        position = safePosition;
        velocity = Vector3::ZERO;
        acceleration = Vector3::ZERO;
        force = Vector3::ZERO;
        isHeld = false;
    }

    /**
     * Get a string representation of the ball for debugging
     * @return String with ball information
     */
    std::string toString() const {
        return "Ball " + std::to_string(ballId) + 
               " at " + position.toString() + 
               " with velocity " + velocity.toString() +
               (isHeld ? " (HELD)" : "");
    }
};

// Initialize static counter
int Ball::ballCount = 0; 