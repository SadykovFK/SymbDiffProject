CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude

SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
MAIN_OBJ = $(BUILD_DIR)/main.o
TEST_OBJ = $(BUILD_DIR)/test.o

EXECUTABLE = differentiator
TEST_EXECUTABLE = tests

all: $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MAIN_OBJ): main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

$(TEST_OBJ): $(TEST_DIR)/test.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/test.cpp -o $@

$(EXECUTABLE): $(OBJECTS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_EXECUTABLE): $(OBJECTS) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(TEST_EXECUTABLE)