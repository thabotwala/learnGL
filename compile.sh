clear
rm -f bin/opGL.elf
g++ ./src/hello_square.cpp ./src/glad.c -ldl -lglfw -o bin/opGL.elf
cd bin
./opGL.elf
