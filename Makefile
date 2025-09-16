CXX = g++
CXXFLAGS = -Wall -g -I./include
LDFLAGS = -lglfw -lGL -ldl -lpthread

SRC = window.cpp glad.c dice.cpp button.cpp
OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

TARGET = window

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
