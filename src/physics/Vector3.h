#pragma once
#include <cmath>
#include <iostream>
#include <string>

/**
 * 3D Vector class for physics calculations
 * Provides essential vector operations for 3D physics simulation
 */
class Vector3 {
public:
    float x, y, z;

    /**
     * Default constructor - creates zero vector
     */
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

    /**
     * Constructor with individual components
     * @param x X component
     * @param y Y component  
     * @param z Z component
     */
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    /**
     * Copy constructor
     * @param other Vector to copy from
     */
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

    /**
     * Vector addition operator
     * @param other Vector to add
     * @return New vector representing sum
     */
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    /**
     * Vector subtraction operator
     * @param other Vector to subtract
     * @return New vector representing difference
     */
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    /**
     * Scalar multiplication operator
     * @param scalar Scalar value to multiply by
     * @return New vector scaled by scalar
     */
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    /**
     * Scalar division operator
     * @param scalar Scalar value to divide by
     * @return New vector divided by scalar
     */
    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    /**
     * In-place addition operator
     * @param other Vector to add
     * @return Reference to this vector
     */
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    /**
     * In-place subtraction operator
     * @param other Vector to subtract
     * @return Reference to this vector
     */
    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    /**
     * In-place scalar multiplication operator
     * @param scalar Scalar value to multiply by
     * @return Reference to this vector
     */
    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    /**
     * Calculate dot product with another vector
     * @param other Vector to calculate dot product with
     * @return Dot product value
     */
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    /**
     * Calculate cross product with another vector
     * @param other Vector to calculate cross product with
     * @return New vector representing cross product
     */
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    /**
     * Calculate magnitude (length) of vector
     * @return Magnitude of vector
     */
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    /**
     * Calculate squared magnitude (avoids square root for performance)
     * @return Squared magnitude of vector
     */
    float magnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    /**
     * Normalize vector to unit length
     * @return New normalized vector
     */
    Vector3 normalized() const {
        float mag = magnitude();
        if (mag > 0.0001f) {
            return *this / mag;
        }
        return Vector3(0, 0, 0);
    }

    /**
     * Normalize this vector in place
     */
    void normalize() {
        float mag = magnitude();
        if (mag > 0.0001f) {
            *this /= mag;
        }
    }

    /**
     * Check if vector is approximately zero
     * @param epsilon Tolerance for comparison
     * @return True if vector is near zero
     */
    bool isZero(float epsilon = 0.0001f) const {
        return magnitude() < epsilon;
    }

    /**
     * Stream output operator for debugging
     * @param os Output stream
     * @param v Vector to output
     * @return Reference to output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    /**
     * Get string representation of vector
     * @return String representation
     */
    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    // Common vector constants
    static const Vector3 ZERO;
    static const Vector3 UP;
    static const Vector3 DOWN;
    static const Vector3 LEFT;
    static const Vector3 RIGHT;
    static const Vector3 FORWARD;
    static const Vector3 BACK;
};

// Static constant definitions
const Vector3 Vector3::ZERO = Vector3(0, 0, 0);
const Vector3 Vector3::UP = Vector3(0, 1, 0);
const Vector3 Vector3::DOWN = Vector3(0, -1, 0);
const Vector3 Vector3::LEFT = Vector3(-1, 0, 0);
const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
const Vector3 Vector3::FORWARD = Vector3(0, 0, -1);
const Vector3 Vector3::BACK = Vector3(0, 0, 1); 