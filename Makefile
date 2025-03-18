# Компилятор
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

# Папки
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TEST_DIR = test

# Исходные файлы
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Главный исполняемый файл
EXECUTABLE = differentiator
TEST_EXEC = tests

# Основные цели
all: $(EXECUTABLE)

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Компиляция исполняемого файла
$(EXECUTABLE): $(OBJECTS) $(BUILD_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(BUILD_DIR)/main.o $(OBJECTS)

# Сборка Google Test и тестов
$(TEST_EXEC): $(OBJECTS) $(TEST_DIR)/test.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -Igoogletest/include -o $(TEST_EXEC) $(TEST_DIR)/test.cpp $(OBJECTS) -lgtest -lgtest_main -pthread

# Запуск тестов
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(TEST_EXEC)

# Создание build-папки
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
