ifdef SystemRoot
	LINKS = -lming32 -lSDLmain -lSDL -lopengl32 -lglut
	COPY = copy
	MAKEDIR = mkdir
	REMOVE = del
else
	LINKS = -lSDL -lGL -lglut
	COPY = cp
	MAKEDIR = mkdir -p
	REMOVE = rm -rf
endif

SYSDIR = ./system
BUILDDIR = ./build
APPNAME = $(BUILDDIR)/csonic
CFLAGS = -W -Wall -O3 -g

all: stack.o linkedlist.o
	$(MAKEDIR) $(BUILDDIR)
	gcc -o $(APPNAME) stack.o linkedlist.o palette.c level.c main.c $(LINKS) $(CFLAGS)
	$(MAKEDIR) $(BUILDDIR)/data
	$(MAKEDIR) $(BUILDDIR)/resources
	$(COPY) ./resources/icon.bmp $(BUILDDIR)/resources/icon.bmp

stack.o: $(SYSDIR)/stack.c $(SYSDIR)/stack.h
	gcc -o stack.o -c $(SYSDIR)/stack.c $(CFLAGS)

linkedlist.o: $(SYSDIR)/linkedlist.c $(SYSDIR)/linkedlist.h
	gcc -o linkedlist.o -c $(SYSDIR)/linkedlist.c $(CFLAGS)

clean:
	$(REMOVE) *.o

delete: clean
	$(REMOVE) $(BUILDDIR)
