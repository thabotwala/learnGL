CFLAGS = -std=c++11 -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
SRCDIR = src
OBJDIR = obj
BINDIR = bin
all: clean objs run

objs: main
	$(CXX) $(CFLAGS) $(OBJDIR)/main.o -o $(BINDIR)/window.elf

main: $(SRCDIR)/main.cpp $(SRCDIR)/glad.c
	$(CXX) $(CFLAGS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

clean:
	rm -f $(OBJDIR)/main.o -f $(OBJDIR)/glad.o
	rm -f $(BINDIR)/*.elf
run:
	./$(BINDIR)/window.elf
