#version 330 core

// Input from vertex shader
in vec3 FragPos;    // Fragment position in world space
in vec3 Normal;     // Fragment normal in world space
in vec2 TexCoord;   // Texture coordinates

// Output color
out vec4 FragColor;

// Uniform variables
uniform vec3 objectColor;   // Color of the object
uniform vec3 lightPos;      // Position of light source
uniform vec3 lightColor;    // Color of light source
uniform vec3 viewPos;       // Position of camera/viewer
uniform float shininess;    // Shininess factor for specular highlights

/**
 * Fragment shader main function
 * Implements Phong lighting model with ambient, diffuse, and specular components
 */
void main()
{
    // Ambient lighting (constant base lighting)
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting (depends on angle between normal and light direction)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting (depends on viewing angle and reflection)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Combine all lighting components
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    // Apply gamma correction for more realistic lighting
    result = pow(result, vec3(1.0/2.2));
    
    FragColor = vec4(result, 1.0);
} 