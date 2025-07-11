#include <iostream>
#include <memory>
#include "game/Game.h"

/**
 * Main entry point for the 3D Physics Engine
 * Initializes the game engine and runs the main game loop
 * @return Exit code (0 for success, non-zero for error)
 */
int main() {
    try {
        // Create game instance
        auto game = std::make_unique<Game>();
        
        // Initialize the game engine
        if (!game->Initialize()) {
            std::cerr << "Failed to initialize game engine!" << std::endl;
            return -1;
        }
        
        // Run the main game loop
        game->Run();
        
        // Cleanup is handled by destructors
        std::cout << "Game exited successfully." << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }
} 