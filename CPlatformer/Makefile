EXECUTABLE_NAME = cplatformer
COMPILER = gcc

ifdef SystemRoot
LIBS = -lmingw32 -lSDLmain -lSDL -lopengl32
else
LIBS = -lSDL -lGL
endif

all:
	$(COMPILER) level.c character.c main.c -o $(EXECUTABLE_NAME) $(LIBS)

clean:
	rm -rf $(EXECUTABLE_NAME)
