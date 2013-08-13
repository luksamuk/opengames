EXECUTABLE_NAME = cplatformer
COMPILER = gcc

ifdef SystemRoot
LIBS = -lmingw32 -lSDLmain -lSDL -lopengl32
REMOVE = del
else
LIBS = -lSDL -lGL
REMOVE = rm -rf
endif

all:
	$(COMPILER) level.c character.c main.c -o $(EXECUTABLE_NAME) $(LIBS)

clean:
	$(REMOVE) $(EXECUTABLE_NAME)
