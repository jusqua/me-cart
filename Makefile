CXX = clang++
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = obj
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

TARGET = me-cart
LDFLAGS = -lGL -lGLU -lGLEW -lglfw -I./include

$(TARGET): $(OBJECTS)
	@$(CXX) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) -c $(LDFLAGS) $< -o $@

clean:
	@$(RM) -rf $(OBJDIR) $(TARGET)
