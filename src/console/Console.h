#pragma once
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <sstream>
#include <iostream>

/**
 * Console class provides a command-line interface for game commands
 * Supports commands like "summon <number>" to create balls
 */
class Console {
private:
    std::vector<std::string> history;                           // Command history
    std::string currentInput;                                   // Current input buffer
    bool isVisible;                                             // Whether console is visible
    bool isActive;                                              // Whether console is accepting input
    int maxHistorySize;                                         // Maximum history entries
    int historyIndex;                                           // Current history position
    
    // Command callbacks
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands;
    
    // Output messages
    std::vector<std::string> outputMessages;
    int maxOutputMessages;
    
public:
    /**
     * Constructor - initializes console with default settings
     */
    Console() 
        : isVisible(false)
        , isActive(false)
        , maxHistorySize(100)
        , historyIndex(-1)
        , maxOutputMessages(50) {
        
        // Register built-in commands
        registerCommand("help", [this](const std::vector<std::string>& args) {
            showHelp();
        });
        
        registerCommand("clear", [this](const std::vector<std::string>& args) {
            clearOutput();
        });
        
        registerCommand("history", [this](const std::vector<std::string>& args) {
            showHistory();
        });
        
        // Welcome message
        addOutput("Physics Engine Console");
        addOutput("Type 'help' for available commands");
        addOutput("Press ~ to toggle console");
    }

    /**
     * Register a command with the console
     * @param command Command name
     * @param callback Function to call when command is executed
     */
    void registerCommand(const std::string& command, std::function<void(const std::vector<std::string>&)> callback) {
        commands[command] = callback;
    }

    /**
     * Process a character input
     * @param c Character to process
     */
    void processChar(char c) {
        if (!isActive) return;
        
        if (c == '\b' || c == 127) {  // Backspace
            if (!currentInput.empty()) {
                currentInput.pop_back();
            }
        } else if (c == '\r' || c == '\n') {  // Enter
            executeCommand(currentInput);
            currentInput.clear();
            historyIndex = -1;
        } else if (c >= 32 && c <= 126) {  // Printable characters
            currentInput += c;
            historyIndex = -1;
        }
    }

    /**
     * Process special key input
     * @param key Key code
     */
    void processSpecialKey(int key) {
        if (!isActive) return;
        
        // Handle arrow keys for history navigation
        if (key == 265) {  // Up arrow
            navigateHistory(-1);
        } else if (key == 264) {  // Down arrow
            navigateHistory(1);
        }
    }

    /**
     * Toggle console visibility and activity
     */
    void toggle() {
        isVisible = !isVisible;
        isActive = isVisible;
        
        if (isActive) {
            currentInput.clear();
            historyIndex = -1;
        }
    }

    /**
     * Set console visibility
     * @param visible True to show console, false to hide
     */
    void setVisible(bool visible) {
        isVisible = visible;
        isActive = visible;
        
        if (isActive) {
            currentInput.clear();
            historyIndex = -1;
        }
    }

    /**
     * Check if console is visible
     * @return True if console is visible
     */
    bool getVisible() const {
        return isVisible;
    }

    /**
     * Check if console is active (accepting input)
     * @return True if console is active
     */
    bool getActive() const {
        return isActive;
    }

    /**
     * Get current input buffer
     * @return Current input string
     */
    const std::string& getCurrentInput() const {
        return currentInput;
    }

    /**
     * Get output messages for rendering
     * @return Vector of output messages
     */
    const std::vector<std::string>& getOutputMessages() const {
        return outputMessages;
    }

    /**
     * Add a message to the output
     * @param message Message to add
     */
    void addOutput(const std::string& message) {
        outputMessages.push_back(message);
        
        // Limit output message count
        if (outputMessages.size() > maxOutputMessages) {
            outputMessages.erase(outputMessages.begin());
        }
    }

    /**
     * Clear all output messages
     */
    void clearOutput() {
        outputMessages.clear();
        addOutput("Console cleared");
    }

    /**
     * Execute a command string
     * @param commandLine Full command line to execute
     */
    void executeCommand(const std::string& commandLine) {
        if (commandLine.empty()) return;
        
        // Add to history
        if (history.empty() || history.back() != commandLine) {
            history.push_back(commandLine);
            if (history.size() > maxHistorySize) {
                history.erase(history.begin());
            }
        }
        
        // Echo command
        addOutput("> " + commandLine);
        
        // Parse command
        std::vector<std::string> tokens = parseCommand(commandLine);
        if (tokens.empty()) return;
        
        std::string command = tokens[0];
        std::vector<std::string> args(tokens.begin() + 1, tokens.end());
        
        // Execute command
        auto it = commands.find(command);
        if (it != commands.end()) {
            try {
                it->second(args);
            } catch (const std::exception& e) {
                addOutput("Error executing command: " + std::string(e.what()));
            }
        } else {
            addOutput("Unknown command: " + command);
            addOutput("Type 'help' for available commands");
        }
    }

    /**
     * Get command history
     * @return Vector of command history
     */
    const std::vector<std::string>& getHistory() const {
        return history;
    }

private:
    /**
     * Parse a command line into tokens
     * @param commandLine Command line to parse
     * @return Vector of tokens
     */
    std::vector<std::string> parseCommand(const std::string& commandLine) {
        std::vector<std::string> tokens;
        std::istringstream iss(commandLine);
        std::string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        return tokens;
    }

    /**
     * Navigate through command history
     * @param direction Direction to navigate (-1 for up, 1 for down)
     */
    void navigateHistory(int direction) {
        if (history.empty()) return;
        
        if (direction < 0) {  // Up
            if (historyIndex < 0) {
                historyIndex = history.size() - 1;
            } else if (historyIndex > 0) {
                historyIndex--;
            }
        } else {  // Down
            if (historyIndex >= 0) {
                historyIndex++;
                if (historyIndex >= history.size()) {
                    historyIndex = -1;
                    currentInput.clear();
                    return;
                }
            }
        }
        
        if (historyIndex >= 0 && historyIndex < history.size()) {
            currentInput = history[historyIndex];
        }
    }

    /**
     * Show help message
     */
    void showHelp() {
        addOutput("Available commands:");
        addOutput("  summon <number> - Summon the specified number of balls");
        addOutput("  clear - Clear console output");
        addOutput("  help - Show this help message");
        addOutput("  history - Show command history");
        addOutput("");
        addOutput("Controls:");
        addOutput("  WASD - Move camera");
        addOutput("  Mouse - Look around");
        addOutput("  E - Pick up/drop balls");
        addOutput("  F - Throw held ball");
        addOutput("  ~ - Toggle console");
        addOutput("  ESC - Exit game");
    }

    /**
     * Show command history
     */
    void showHistory() {
        if (history.empty()) {
            addOutput("No command history");
            return;
        }
        
        addOutput("Command history:");
        for (size_t i = 0; i < history.size(); i++) {
            addOutput("  " + std::to_string(i + 1) + ": " + history[i]);
        }
    }
}; 