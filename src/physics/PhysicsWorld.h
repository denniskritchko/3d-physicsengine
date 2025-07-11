#pragma once
#include "PhysicsBody.h"
#include "Ball.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>

/**
 * PhysicsWorld class manages all physics bodies and handles collision detection/resolution
 * This is the main physics simulation controller
 */
class PhysicsWorld {
private:
    std::vector<std::unique_ptr<PhysicsBody>> bodies;  // All physics bodies in the world
    Vector3 gravity;                                   // Global gravity vector
    float worldBounds[6];                              // World boundaries [minX, maxX, minY, maxY, minZ, maxZ]
    float timeStep;                                    // Fixed time step for physics simulation
    int maxSubsteps;                                   // Maximum substeps per frame
    
public:
    /**
     * Constructor - creates a physics world with default settings
     */
    PhysicsWorld() 
        : gravity(0, -9.81f, 0)
        , timeStep(1.0f / 60.0f)
        , maxSubsteps(4) {
        
        // Set default world bounds (30x30 room, 10m high)
        worldBounds[0] = -15.0f;  // minX
        worldBounds[1] = 15.0f;   // maxX
        worldBounds[2] = 0.0f;    // minY (floor)
        worldBounds[3] = 10.0f;   // maxY (ceiling)
        worldBounds[4] = -15.0f;  // minZ
        worldBounds[5] = 15.0f;   // maxZ
    }

    /**
     * Add a physics body to the world
     * @param body Unique pointer to the physics body
     * @return Raw pointer to the added body for reference
     */
    PhysicsBody* addBody(std::unique_ptr<PhysicsBody> body) {
        PhysicsBody* rawPtr = body.get();
        bodies.push_back(std::move(body));
        return rawPtr;
    }

    /**
     * Create and add a ball to the world
     * @param position Starting position of the ball
     * @return Pointer to the created ball
     */
    Ball* createBall(const Vector3& position) {
        auto ball = std::make_unique<Ball>(position);
        Ball* ballPtr = ball.get();
        bodies.push_back(std::move(ball));
        return ballPtr;
    }

    /**
     * Remove a physics body from the world
     * @param body Pointer to the body to remove
     */
    void removeBody(PhysicsBody* body) {
        bodies.erase(
            std::remove_if(bodies.begin(), bodies.end(),
                [body](const std::unique_ptr<PhysicsBody>& ptr) {
                    return ptr.get() == body;
                }),
            bodies.end()
        );
    }

    /**
     * Update the physics world for one frame
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime) {
        // Use fixed time step with substeps for stability
        float remainingTime = deltaTime;
        int substeps = 0;
        
        while (remainingTime > 0.0f && substeps < maxSubsteps) {
            float currentStep = std::min(remainingTime, timeStep);
            
            // Update all physics bodies
            for (auto& body : bodies) {
                body->update(currentStep);
            }
            
            // Handle collisions
            handleCollisions();
            
            // Handle world boundary collisions
            handleWorldBoundaries();
            
            remainingTime -= currentStep;
            substeps++;
        }
    }

    /**
     * Handle collision detection and resolution between all bodies
     */
    void handleCollisions() {
        // Simple O(n²) collision detection - can be optimized with spatial partitioning
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                PhysicsBody* bodyA = bodies[i].get();
                PhysicsBody* bodyB = bodies[j].get();
                
                if (bodyA->isCollidingWith(*bodyB)) {
                    resolveCollision(bodyA, bodyB);
                }
            }
        }
    }

    /**
     * Resolve collision between two physics bodies
     * @param bodyA First colliding body
     * @param bodyB Second colliding body
     */
    void resolveCollision(PhysicsBody* bodyA, PhysicsBody* bodyB) {
        // Calculate collision normal
        Vector3 normal = (bodyA->position - bodyB->position).normalized();
        
        // Calculate relative velocity
        Vector3 relativeVelocity = bodyA->velocity - bodyB->velocity;
        
        // Calculate relative velocity along normal
        float velocityAlongNormal = relativeVelocity.dot(normal);
        
        // Don't resolve if objects are separating
        if (velocityAlongNormal > 0) {
            return;
        }
        
        // Calculate restitution (bounciness)
        float restitution = std::min(bodyA->restitution, bodyB->restitution);
        
        // Calculate impulse scalar
        float impulseScalar = -(1 + restitution) * velocityAlongNormal;
        impulseScalar /= bodyA->getInverseMass() + bodyB->getInverseMass();
        
        // Apply impulse
        Vector3 impulse = normal * impulseScalar;
        bodyA->applyImpulse(impulse * bodyA->getInverseMass());
        bodyB->applyImpulse(impulse * -bodyB->getInverseMass());
        
        // Position correction to prevent sinking
        float penetrationDepth = (bodyA->radius + bodyB->radius) - (bodyA->position - bodyB->position).magnitude();
        if (penetrationDepth > 0) {
            float correctionPercent = 0.8f;  // How much to correct
            float correctionSlop = 0.01f;    // Penetration allowance
            
            Vector3 correction = normal * (correctionPercent * 
                std::max(penetrationDepth - correctionSlop, 0.0f) / 
                (bodyA->getInverseMass() + bodyB->getInverseMass()));
            
            bodyA->position += correction * bodyA->getInverseMass();
            bodyB->position -= correction * bodyB->getInverseMass();
        }
        
        // Special handling for ball-to-ball collisions
        Ball* ballA = dynamic_cast<Ball*>(bodyA);
        Ball* ballB = dynamic_cast<Ball*>(bodyB);
        
        if (ballA && ballB) {
            // Add slight randomness to ball-ball collisions for more interesting behavior
            float randomFactor = 0.1f;
            Vector3 randomOffset(
                ((float)rand() / RAND_MAX - 0.5f) * randomFactor,
                ((float)rand() / RAND_MAX - 0.5f) * randomFactor,
                ((float)rand() / RAND_MAX - 0.5f) * randomFactor
            );
            ballA->velocity += randomOffset;
            ballB->velocity -= randomOffset;
        }
    }

    /**
     * Handle collisions with world boundaries (walls, floor, ceiling)
     */
    void handleWorldBoundaries() {
        for (auto& body : bodies) {
            // Skip static bodies
            if (body->isStatic) {
                continue;
            }
            
            bool collided = false;
            
            // Check X boundaries (left/right walls)
            if (body->position.x - body->radius < worldBounds[0]) {
                body->position.x = worldBounds[0] + body->radius;
                if (body->velocity.x < 0) {
                    body->velocity.x = -body->velocity.x * body->restitution;
                    collided = true;
                }
            } else if (body->position.x + body->radius > worldBounds[1]) {
                body->position.x = worldBounds[1] - body->radius;
                if (body->velocity.x > 0) {
                    body->velocity.x = -body->velocity.x * body->restitution;
                    collided = true;
                }
            }
            
            // Check Y boundaries (floor/ceiling)
            if (body->position.y - body->radius < worldBounds[2]) {
                body->position.y = worldBounds[2] + body->radius;
                if (body->velocity.y < 0) {
                    body->velocity.y = -body->velocity.y * body->restitution;
                    collided = true;
                }
            } else if (body->position.y + body->radius > worldBounds[3]) {
                body->position.y = worldBounds[3] - body->radius;
                if (body->velocity.y > 0) {
                    body->velocity.y = -body->velocity.y * body->restitution;
                    collided = true;
                }
            }
            
            // Check Z boundaries (front/back walls)
            if (body->position.z - body->radius < worldBounds[4]) {
                body->position.z = worldBounds[4] + body->radius;
                if (body->velocity.z < 0) {
                    body->velocity.z = -body->velocity.z * body->restitution;
                    collided = true;
                }
            } else if (body->position.z + body->radius > worldBounds[5]) {
                body->position.z = worldBounds[5] - body->radius;
                if (body->velocity.z > 0) {
                    body->velocity.z = -body->velocity.z * body->restitution;
                    collided = true;
                }
            }
            
            // Apply friction for ground contact
            if (body->position.y <= worldBounds[2] + body->radius + 0.1f && collided) {
                body->velocity.x *= (1.0f - body->friction);
                body->velocity.z *= (1.0f - body->friction);
            }
        }
    }

    /**
     * Get all physics bodies in the world
     * @return Vector of physics body pointers
     */
    std::vector<PhysicsBody*> getBodies() const {
        std::vector<PhysicsBody*> result;
        for (const auto& body : bodies) {
            result.push_back(body.get());
        }
        return result;
    }

    /**
     * Get all balls in the world
     * @return Vector of ball pointers
     */
    std::vector<Ball*> getBalls() const {
        std::vector<Ball*> result;
        for (const auto& body : bodies) {
            Ball* ball = dynamic_cast<Ball*>(body.get());
            if (ball) {
                result.push_back(ball);
            }
        }
        return result;
    }

    /**
     * Set world boundaries
     * @param minX Minimum X coordinate
     * @param maxX Maximum X coordinate
     * @param minY Minimum Y coordinate (floor)
     * @param maxY Maximum Y coordinate (ceiling)
     * @param minZ Minimum Z coordinate
     * @param maxZ Maximum Z coordinate
     */
    void setWorldBounds(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
        worldBounds[0] = minX;
        worldBounds[1] = maxX;
        worldBounds[2] = minY;
        worldBounds[3] = maxY;
        worldBounds[4] = minZ;
        worldBounds[5] = maxZ;
    }

    /**
     * Get world boundaries
     * @return Array of 6 floats [minX, maxX, minY, maxY, minZ, maxZ]
     */
    const float* getWorldBounds() const {
        return worldBounds;
    }

    /**
     * Clear all physics bodies from the world
     */
    void clear() {
        bodies.clear();
    }

    /**
     * Get the number of physics bodies in the world
     * @return Number of bodies
     */
    size_t getBodyCount() const {
        return bodies.size();
    }

    /**
     * Set gravity for the world
     * @param g Gravity vector
     */
    void setGravity(const Vector3& g) {
        gravity = g;
    }

    /**
     * Get current gravity
     * @return Gravity vector
     */
    const Vector3& getGravity() const {
        return gravity;
    }
}; 