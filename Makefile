# Compiler and flags
CXX         = g++
CXXFLAGS    = -Wall -Wextra -std=c++11 -I./pdcurses/include

# Library directories and linking
LDFLAGS     = -L./pdcurses/lib
# Note: linking -lmingw32 before -lpdcurses is important
LIBS        = -lmingw32 -lpdcurses

# Source and object files
SRCS        = Main.cpp Classes.cpp Randomness.cpp
OBJS        = $(SRCS:.cpp=.o)

# The name of the final executable
TARGET      = myproject

# Default target: build the executable
all: $(TARGET)

# Linking rule: combine object files and libraries into executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

# Pattern rule: compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target: remove object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
