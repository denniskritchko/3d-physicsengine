#pragma once

// Basic GLM-like math library for the physics engine
// This is a minimal implementation of common vector and matrix operations

#include <cmath>

namespace glm {
    // Vector2
    struct vec2 {
        float x, y;
        
        vec2() : x(0), y(0) {}
        vec2(float x, float y) : x(x), y(y) {}
        
        vec2 operator+(const vec2& other) const { return vec2(x + other.x, y + other.y); }
        vec2 operator-(const vec2& other) const { return vec2(x - other.x, y - other.y); }
        vec2 operator*(float scalar) const { return vec2(x * scalar, y * scalar); }
    };
    
    // Vector3
    struct vec3 {
        float x, y, z;
        
        vec3() : x(0), y(0), z(0) {}
        vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        
        vec3 operator+(const vec3& other) const { return vec3(x + other.x, y + other.y, z + other.z); }
        vec3 operator-(const vec3& other) const { return vec3(x - other.x, y - other.y, z - other.z); }
        vec3 operator*(float scalar) const { return vec3(x * scalar, y * scalar, z * scalar); }
        
        float length() const { return std::sqrt(x * x + y * y + z * z); }
        vec3 normalize() const { 
            float len = length();
            return len > 0 ? vec3(x / len, y / len, z / len) : vec3();
        }
        
        static vec3 cross(const vec3& a, const vec3& b) {
            return vec3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }
        
        static float dot(const vec3& a, const vec3& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }
    };
    
    // Vector4
    struct vec4 {
        float x, y, z, w;
        
        vec4() : x(0), y(0), z(0), w(0) {}
        vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        vec4(const vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    };
    
    // Matrix4x4
    struct mat4 {
        float m[16];  // Column-major order
        
        mat4() {
            // Identity matrix
            for (int i = 0; i < 16; i++) m[i] = 0;
            m[0] = m[5] = m[10] = m[15] = 1.0f;
        }
        
        mat4(float* data) {
            for (int i = 0; i < 16; i++) m[i] = data[i];
        }
        
        float* value_ptr() { return m; }
        const float* value_ptr() const { return m; }
    };
    
    // Matrix operations
    inline mat4 perspective(float fovy, float aspect, float near, float far) {
        float f = 1.0f / std::tan(fovy / 2.0f);
        float zRange = near - far;
        
        mat4 result;
        result.m[0] = f / aspect;
        result.m[5] = f;
        result.m[10] = (far + near) / zRange;
        result.m[11] = -1.0f;
        result.m[14] = (2.0f * far * near) / zRange;
        result.m[15] = 0.0f;
        
        return result;
    }
    
    inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
        vec3 f = (center - eye).normalize();
        vec3 s = vec3::cross(f, up).normalize();
        vec3 u = vec3::cross(s, f);
        
        mat4 result;
        result.m[0] = s.x;
        result.m[4] = s.y;
        result.m[8] = s.z;
        result.m[12] = -vec3::dot(s, eye);
        
        result.m[1] = u.x;
        result.m[5] = u.y;
        result.m[9] = u.z;
        result.m[13] = -vec3::dot(u, eye);
        
        result.m[2] = -f.x;
        result.m[6] = -f.y;
        result.m[10] = -f.z;
        result.m[14] = vec3::dot(f, eye);
        
        result.m[3] = 0;
        result.m[7] = 0;
        result.m[11] = 0;
        result.m[15] = 1;
        
        return result;
    }
    
    inline mat4 translate(const mat4& m, const vec3& v) {
        mat4 result = m;
        result.m[12] = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
        result.m[13] = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
        result.m[14] = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
        result.m[15] = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15];
        return result;
    }
    
    inline mat4 scale(const mat4& m, const vec3& v) {
        mat4 result = m;
        result.m[0] *= v.x;
        result.m[1] *= v.x;
        result.m[2] *= v.x;
        result.m[3] *= v.x;
        
        result.m[4] *= v.y;
        result.m[5] *= v.y;
        result.m[6] *= v.y;
        result.m[7] *= v.y;
        
        result.m[8] *= v.z;
        result.m[9] *= v.z;
        result.m[10] *= v.z;
        result.m[11] *= v.z;
        
        return result;
    }
    
    // Useful constants
    const float pi = 3.14159265359f;
    
    // Utility functions
    inline float radians(float degrees) {
        return degrees * pi / 180.0f;
    }
    
    inline float degrees(float radians) {
        return radians * 180.0f / pi;
    }
} 