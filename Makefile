CXX = g++
CXXFLAGS = -Wall -g -I./include
LDFLAGS = -lglfw -lGL -ldl -lpthread

# Source files
SRC_ROOT = window.cpp
SRC_SRC  = src/glad.c src/dice.cpp src/button.cpp src/shader.cpp
SRC      = $(SRC_ROOT) $(SRC_SRC)

# Build directory
OBJDIR = build
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.cpp=.o)))
OBJ := $(OBJ:.c=.o)

TARGET = window

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Rules for root-level C++ source
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rules for root-level C source
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rules for src/ C++ source
$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rules for src/ C source
$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it does not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)
