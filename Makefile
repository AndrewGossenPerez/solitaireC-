# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I/opt/homebrew/opt/sfml/include
LDFLAGS = -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

# Source and build directories
SRC_DIR = src
OBJ_DIR = obj

# Store each .cpp file in /src and store in SRC 
SRC = $(wildcard $(SRC_DIR)/*.cpp)
# Convert to o path 
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Output binary
TARGET = solitaire

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Build object files inside /obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create OBJ_DIR directory if it doesn't exist already 
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean the build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Run command for convienance sake 
run: $(TARGET)
	./$(TARGET)
