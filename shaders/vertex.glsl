#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;        // Vertex position
layout (location = 1) in vec3 aNormal;     // Vertex normal
layout (location = 2) in vec2 aTexCoord;   // Texture coordinates

// Output to fragment shader
out vec3 FragPos;       // Fragment position in world space
out vec3 Normal;        // Fragment normal in world space
out vec2 TexCoord;      // Texture coordinates

// Uniform matrices
uniform mat4 model;      // Model matrix (object to world)
uniform mat4 view;       // View matrix (world to camera)
uniform mat4 projection; // Projection matrix (camera to screen)

/**
 * Vertex shader main function
 * Transforms vertex position and normal from object space to world space
 * and calculates final screen position
 */
void main()
{
    // Transform vertex position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space (using normal matrix for non-uniform scaling)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass through texture coordinates
    TexCoord = aTexCoord;
    
    // Calculate final vertex position in clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
} 