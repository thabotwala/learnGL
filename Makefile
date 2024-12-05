CFLAGS = -std=c++11 -lglfw -lGL -lX11 -lpthread -lXrandr -ldl
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = /usr/include
all: clean objs run

objs: main glad
	$(CXX) $(CFLAGS) $(OBJDIR)/main.o $(OBJDIR)/glad.o -o $(BINDIR)/window.elf

main: $(SRCDIR)/main.cpp $(INCDIR)/GLFW/glfw3.h
	$(CXX) $(CFLAGS) $(INCDIR)/GLFW/glfw3.h $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o
glad: $(SRCDIR)/glad.c
	$(CXX) $(CFLAGS) -c $(SRCDIR)/glad.c -o $(OBJDIR)/glad.o

clean:
	rm -f $(OBJDIR)/main.o -f $(OBJDIR)/glad.o
	rm -f $(BINDIR)/*.elf
run:
	./$(BINDIR)/window.elf
