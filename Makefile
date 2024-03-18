TARGET=me-cart
CXX=clang++
SOURCEDIR=./src/
LDFLAGS=-lGL -lGLU -lGLEW -lglfw -I./include/
SOURCES=$(shell find $(SOURCEDIR) -name '*.cpp')

build:
	@$(CXX) $(SOURCES) $(LDFLAGS) -o $(TARGET)

run: build
	@./$(TARGET)
