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

	APPNAME = build/csonic
endif

SYSDIR = system
BUILDDIR = build

CFLAGS = -W -Wall -O3 -g

all: mkdirs
	@echo "> Compiling files..."
	gcc -o $(APPNAME) vec2.c input.c palette.c level.c main.c $(LINKS) $(CFLAGS)
	@echo "> Copying resources..."
	@$(COPY) $(ICONPATH) $(ENDICONPATH)
	@echo "Done."

clean:
	@echo "> Removing object files..."
	@$(REMOVE) *.o

mkdirs:
	@echo "> Creating resource directories..."
	@$(MAKEDIR) $(BUILDDIR)
	@cd $(BUILDDIR)
	@$(MAKEDIR) $(RESOURCESDIR)
	@$(MAKEDIR) $(DATADIR)

delete: clean
	@echo "> Deleting build directory..."
	@$(DELDIR) $(BUILDDIR)
