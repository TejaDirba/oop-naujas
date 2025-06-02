CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

INCLUDES = -Iinclude
SRC = src/Student.cpp src/FileManager.cpp src/kodas.cpp
OBJ = Student.o FileManager.o kodas.o
TARGET = studentai_v1.0

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
