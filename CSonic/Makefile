ifdef SystemRoot
	LINKS = -lming32 -lSDLmain -lSDL -lopengl32 -lglut
else
	LINKS = -lSDL -lGL -lglut
endif

SYSDIR = ./system
BUILDDIR = ./build
APPNAME = $(BUILDDIR)/csonic
CFLAGS = -W -Wall -O3 -g

all: stack.o linkedlist.o
	mkdir -p $(BUILDDIR)
	gcc -o $(APPNAME) stack.o linkedlist.o palette.c level.c main.c $(LINKS) $(CFLAGS)
	mkdir -p $(BUILDDIR)/data
	mkdir -p $(BUILDDIR)/resources
	cp ./resources/icon.bmp $(BUILDDIR)/resources/icon.bmp

stack.o: $(SYSDIR)/stack.c $(SYSDIR)/stack.h
	gcc -o stack.o -c $(SYSDIR)/stack.c $(CFLAGS)

linkedlist.o: $(SYSDIR)/linkedlist.c $(SYSDIR)/linkedlist.h
	gcc -o linkedlist.o -c $(SYSDIR)/linkedlist.c $(CFLAGS)

clean:
	rm -rf *.o

delete: clean
	rm -rf $(BUILDDIR)
