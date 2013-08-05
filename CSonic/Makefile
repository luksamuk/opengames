ifdef SystemRoot
	FLAGS = -lming32 -lSDLmain -lSDL -lopengl32
else
	FLAGS = -lSDL -lGL
endif

all:
	gcc palette.c level.c main.c -o csonic $(FLAGS)