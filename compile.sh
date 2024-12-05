clear
rm -f bin/opGL.elf
g++ ./src/hello_square.cpp ./src/glad.c -std=c++11 -lglfw -lGL -lX11 -lpthread -lXrandr -o bin/opGL.elf
cd bin
./opGL.elf
