ifdef SystemRoot
	LINKS = -lmingw32 -lSDLmain -lSDL -lopengl32 -lglut32win
	COPY = copy
	MAKEDIR = mkdir
	REMOVE = del /S /Q
	DELDIR = rmdir /S /Q

	DATADIR = build\data
	RESOURCESDIR = build\resources

	ICONPATH = resources\icon.bmp
	ENDICONPATH = build\resources\icon.bmp
	COPYSDL = copy SDL.dll build\SDL.dll

	APPNAME = build\csonic.exe
else
	LINKS = -lSDL -lGL -lglut
	COPY = cp
	MAKEDIR = mkdir -p
	REMOVE = rm -f
	DELDIR = rm -rf

	DATADIR = build/data
	RESOURCESDIR = build/resources

	ICONPATH = resources/icon.bmp
	ENDICONPATH = build/resources/icon.bmp
	COPYSDL = echo "Unecessary to copy SDL dlls."

	APPNAME = build/csonic
endif

SYSDIR = system
BUILDDIR = build

CFLAGS = -W -Wall -O3 -g

all: mkdirs
	gcc -o $(APPNAME) input.c palette.c level.c main.c $(LINKS) $(CFLAGS)
	$(COPY) $(ICONPATH) $(ENDICONPATH)
	$(COPYSDL)

clean:
	$(REMOVE) *.o

mkdirs:
	$(MAKEDIR) $(BUILDDIR)
	cd $(BUILDDIR)
	$(MAKEDIR) $(RESOURCESDIR)
	$(MAKEDIR) $(DATADIR)

delete: clean
	$(DELDIR) $(BUILDDIR)
