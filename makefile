# Automatically generated Makefile for project: tic_tac_toe
# Created on  by gaten for sdltutorials.com
# Comments, suggestions and bugs email: gaten DOT net AT gmail DOT com

# Compiler
CC=g++

# Objects
OBJECTS=game.o

# Compiler flags
CPPFLAGS=-Wall -g `sdl-config --cflags`

# Linker flags
LDFLAGS=`sdl-config --libs` -lSDL -lSDL_image -lglut -lGL -lGLU -lSDL_ttf

main: game.cpp
	$(CC) $(CPPFLAGS) -c game.cpp
	$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o test

# cleanup
clean:
	rm -rf $(OBJECTS)

# EOF
