# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/opt/glew/include -I/opt/homebrew/opt/glfw/include -I/opt/homebrew/opt/freeglut/include -I/System/Library/Frameworks/OpenGL.framework/Headers -DGL_SILENCE_DEPRECATION
LDFLAGS = -L/opt/homebrew/opt/glew/lib -L/opt/homebrew/opt/glfw/lib -L/opt/homebrew/opt/freeglut/lib -lglew -lglfw -lglut -framework OpenGL

# Directory structure
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
TARGET = $(BIN_DIR)/Space_simulator

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)
