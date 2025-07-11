# Makefile for 3D Physics Engine
# Alternative build system for systems without CMake

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
INCLUDES = -Isrc -Iexternal/glad/include -Iexternal/glm

# Libraries
LIBS = -lglfw -lGL -ldl -pthread

# Source files
SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp \
          external/glad/src/glad.c

# Object files
OBJDIR = build
OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/glad.o

# Target executable
TARGET = 3DPhysicsEngine

# Default target
all: directories $(TARGET)

# Create build directory
directories:
	@mkdir -p $(OBJDIR)

# Link executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "Build complete! Run ./$(TARGET) to start the engine."

# Compile main.cpp
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile glad.c
$(OBJDIR)/glad.o: external/glad/src/glad.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	@echo "Clean complete."

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential libglfw3-dev libgl1-mesa-dev

# Install dependencies (for macOS with Homebrew)
install-deps-mac:
	brew install glfw

# Run the engine
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the engine (default)"
	@echo "  clean        - Remove build files"
	@echo "  run          - Build and run the engine"
	@echo "  debug        - Build with debug symbols"
	@echo "  install-deps - Install dependencies (Ubuntu/Debian)"
	@echo "  install-deps-mac - Install dependencies (macOS)"
	@echo "  help         - Show this help message"

.PHONY: all clean run debug help directories install-deps install-deps-mac 