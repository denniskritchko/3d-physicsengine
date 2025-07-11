#pragma once
#include "Vector3.h"

/**
 * Physics Body class representing a physical object in the simulation
 * Handles position, velocity, forces, and basic physics properties
 */
class PhysicsBody {
public:
    Vector3 position;       // Current position in world space
    Vector3 velocity;       // Current velocity vector
    Vector3 acceleration;   // Current acceleration vector
    Vector3 force;          // Net force acting on the body
    
    float mass;             // Mass of the object (kg)
    float restitution;      // Bounciness coefficient (0 = no bounce, 1 = perfect bounce)
    float friction;         // Friction coefficient
    float radius;           // Radius for sphere collision (simplified collision)
    
    bool isStatic;          // If true, object doesn't move (infinite mass)
    bool isActive;          // If false, object is not updated
    
    /**
     * Default constructor - creates a default physics body
     */
    PhysicsBody() 
        : position(0, 0, 0)
        , velocity(0, 0, 0)
        , acceleration(0, 0, 0)
        , force(0, 0, 0)
        , mass(1.0f)
        , restitution(0.7f)
        , friction(0.5f)
        , radius(0.5f)
        , isStatic(false)
        , isActive(true) {}

    /**
     * Constructor with position and mass
     * @param pos Initial position
     * @param m Mass of the object
     * @param r Radius of the object
     */
    PhysicsBody(const Vector3& pos, float m, float r)
        : position(pos)
        , velocity(0, 0, 0)
        , acceleration(0, 0, 0)
        , force(0, 0, 0)
        , mass(m)
        , restitution(0.7f)
        , friction(0.5f)
        , radius(r)
        , isStatic(false)
        , isActive(true) {}

    /**
     * Apply a force to the physics body
     * @param f Force vector to apply
     */
    void applyForce(const Vector3& f) {
        if (!isStatic) {
            force += f;
        }
    }

    /**
     * Apply an impulse (instantaneous change in momentum)
     * @param impulse Impulse vector to apply
     */
    void applyImpulse(const Vector3& impulse) {
        if (!isStatic) {
            velocity += impulse / mass;
        }
    }

    /**
     * Update physics body for one time step using Euler integration
     * @param deltaTime Time step in seconds
     */
    virtual void update(float deltaTime) {
        if (!isActive || isStatic) {
            return;
        }

        // Calculate acceleration from force (F = ma -> a = F/m)
        acceleration = force / mass;
        
        // Apply gravity
        acceleration += Vector3(0, -9.81f, 0);
        
        // Update velocity using acceleration
        velocity += acceleration * deltaTime;
        
        // Apply air resistance (simple drag)
        velocity *= 0.999f;
        
        // Update position using velocity
        position += velocity * deltaTime;
        
        // Reset forces for next frame
        force = Vector3::ZERO;
    }

    /**
     * Get the inverse mass (used for collision response)
     * @return Inverse mass (0 for static objects)
     */
    float getInverseMass() const {
        return isStatic ? 0.0f : 1.0f / mass;
    }

    /**
     * Check if this body is colliding with another body
     * @param other Other physics body to check collision with
     * @return True if bodies are colliding
     */
    bool isCollidingWith(const PhysicsBody& other) const {
        if (!isActive || !other.isActive) {
            return false;
        }
        
        float distance = (position - other.position).magnitude();
        return distance < (radius + other.radius);
    }

    /**
     * Get the distance between this body and another
     * @param other Other physics body
     * @return Distance between centers
     */
    float getDistanceTo(const PhysicsBody& other) const {
        return (position - other.position).magnitude();
    }

    /**
     * Set the body as static (immovable)
     * @param static_state True to make static, false to make dynamic
     */
    void setStatic(bool static_state) {
        isStatic = static_state;
        if (isStatic) {
            velocity = Vector3::ZERO;
            acceleration = Vector3::ZERO;
            force = Vector3::ZERO;
        }
    }

    /**
     * Set the restitution (bounciness) of the body
     * @param rest Restitution value (0.0 to 1.0)
     */
    void setRestitution(float rest) {
        restitution = std::max(0.0f, std::min(1.0f, rest));
    }

    /**
     * Set the friction coefficient of the body
     * @param frict Friction value (0.0 to 1.0)
     */
    void setFriction(float frict) {
        friction = std::max(0.0f, std::min(1.0f, frict));
    }
}; 