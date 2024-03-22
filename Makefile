SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = obj
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

TARGET = me-cart
CFLAGS = -lGL -lGLU -lGLEW -lglfw -I./include

$(TARGET): $(OBJECTS)
	@$(CXX) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@$(RM) -rfv $(OBJDIR)
