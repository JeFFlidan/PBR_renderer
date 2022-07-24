INCLUDE = ./include
LIB = ./lib

DLL = -lglfw3dll -lassimp

CFLAGS = -std=c++17 -I$(INCLUDE) -L$(LIB)

main: src/*.cpp include/*.hpp include/import/*.hpp
	g++ $(CFLAGS) -o main src/*.cpp src/glad.c $(DLL)

.PHONY: test clean

test: main.exe
	.\main.exe

clean:
	rm -f main.exe