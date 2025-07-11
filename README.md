# 3D Physics Engine

A comprehensive 3D physics engine built in C++ with OpenGL rendering, featuring realistic ball physics, collision detection, and interactive gameplay.

## Features

### Core Physics
- **Real-time Physics**: Accurate physics simulation with gravity, collision detection, and response
- **Bouncy Balls!**: Highly bouncy balls with realistic material properties
- **Collision Detection**: Sphere-sphere and sphere-boundary collision detection
- **Force Application**: Support for applying forces and impulses to physics bodies
- **Realistic Damping**: Air resistance and friction simulation

### Rendering System
- **3D OpenGL Rendering**: Modern OpenGL 3.3 core profile with custom shaders
- **Phong Lighting**: Realistic lighting with ambient, diffuse, and specular components
- **Sphere Rendering**: Procedurally generated sphere meshes with proper normals
- **Room Environment**: Enclosed 30x30 meter room with walls, floor, and ceiling
- **Dynamic Lighting**: Configurable light sources with realistic shadows

### Player Interaction
- **First-Person Camera**: Smooth camera controls with mouse look and WASD movement
- **Ball Interaction**: Pick up, hold, and throw balls with realistic physics
- **Interactive Range**: Configurable pickup range and throwing force

### Command Console
- **In-Game Console**: Press `~` to open the command console
- **Ball Summoning**: Use `summon <number>` to create multiple balls at once
- **Physics Commands**: Query physics state and clear objects
- **Command History**: Navigate through previous commands with arrow keys

## Controls

| Key | Action |
|-----|--------|
| `W/A/S/D` | Move camera |
| `Mouse` | Look around |
| `Space` | Move camera up |
| `Left Shift` | Move camera down |
| `E` | Pick up/drop nearest ball |
| `F` | Throw held ball |
| `~` | Toggle console |
| `P` | Pause/unpause physics |
| `ESC` | Exit game |

## Console Commands

- `summon <number>` - Create the specified number of balls
- `clear_balls` - Remove all balls from the scene
- `physics_info` - Display physics simulation information
- `help` - Show available commands
- `clear` - Clear console output
- `history` - Show command history

## Architecture

### Physics System
- **Vector3**: 3D vector mathematics with common operations
- **PhysicsBody**: Base class for all physics objects
- **Ball**: Specialized physics body with enhanced bouncing properties
- **PhysicsWorld**: Manages all physics objects and simulations

### Rendering Pipeline
- **Camera**: First-person camera with perspective projection
- **Renderer**: OpenGL rendering manager with shader support
- **Shaders**: Vertex and fragment shaders for 3D rendering with lighting

### Input System
- **InputHandler**: Comprehensive input management with callback support
- **Console**: Command-line interface for game commands

### Game Framework
- **Game**: Main game loop and system coordination
- **Modular Design**: Loosely coupled systems for easy extension

## Building

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15+
- OpenGL 3.3+ support
- GLFW 3.3+

### Build Instructions

1. **Clone the repository**:
```bash
git clone <repository-url>
cd 3d-physicsengine
```

2. **Create build directory**:
```bash
mkdir build
cd build
```

3. **Configure with CMake**:
```bash
cmake ..
```

4. **Build the project**:
```bash
cmake --build .
```

5. **Run the engine**:
```bash
./3DPhysicsEngine
```

### Dependencies
- **GLFW**: Window management and input handling
- **GLAD**: OpenGL function loading (custom minimal implementation)
- **GLM**: Mathematics library for graphics (custom minimal implementation)

## Technical Details

### Physics Implementation
- **Euler Integration**: Stable numerical integration for physics simulation
- **Impulse-Based Collision Resolution**: Realistic collision response
- **Broadphase Collision Detection**: Efficient collision detection for multiple objects
- **Restitution and Friction**: Configurable material properties

### Rendering Features
- **Procedural Sphere Generation**: Runtime sphere mesh generation
- **Instanced Rendering**: Efficient rendering of multiple similar objects
- **Shader-Based Lighting**: GPU-accelerated Phong lighting model
- **Depth Testing**: Proper 3D depth sorting

### Performance Optimizations
- **Fixed Timestep Physics**: Consistent physics simulation regardless of framerate
- **Efficient Collision Detection**: Optimized algorithms for real-time performance
- **Memory Management**: Smart pointer usage for automatic resource cleanup

## Configuration

### World Settings
- **Room Size**: 30x30 meters (configurable in PhysicsWorld)
- **Gravity**: 9.81 m/s² (Earth gravity)
- **Ball Properties**: Mass: 0.5kg, Radius: 0.25m, Restitution: 0.8

### Rendering Settings
- **Resolution**: 1280x720 (configurable)
- **Field of View**: 45° (mouse scroll to adjust)
- **Render Distance**: 1000 units

## Future Enhancements

- **Advanced Physics**: Rotation, angular momentum, and torque
- **Multiple Ball Types**: Different materials and properties
- **Particle Effects**: Visual effects for collisions and impacts
- **Audio System**: Sound effects for collisions and interactions
- **Performance Profiling**: Built-in performance monitoring
- **Save/Load System**: Scene persistence and loading

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- OpenGL community for graphics programming resources
- Physics simulation techniques from real-time rendering literature
- Game development best practices from industry standards 
