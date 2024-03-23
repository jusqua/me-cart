CXX = clang++
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = obj
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

TARGET = me-cart
CXXFLAGS = -stdlib=libstdc++ -pthread -I./include 
LDFLAGS = -lGL -lGLU -lGLEW -lglfw

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	@$(RM) -rf $(OBJDIR) $(TARGET)
