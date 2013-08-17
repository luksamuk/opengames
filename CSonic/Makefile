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
	COPYSDL = echo "NOTE: Copy of SDL.dll is unecessary, jumping step."

	APPNAME = build/csonic
endif

SYSDIR = system
BUILDDIR = build

CFLAGS = -W -Wall -O3 -g

all: mkdirs
	@echo "> Compiling files...\n"
	gcc -o $(APPNAME) input.c palette.c level.c main.c $(LINKS) $(CFLAGS)
	@echo "\n> Copying resources...\n"
	@$(COPY) $(ICONPATH) $(ENDICONPATH)
	@$(COPYSDL)
	@echo "Done.\n"

clean:
	@echo "> Removing object files..."
	@$(REMOVE) *.o

mkdirs:
	@echo "\n> Creating resource directories...\n"
	@$(MAKEDIR) $(BUILDDIR)
	@cd $(BUILDDIR)
	@$(MAKEDIR) $(RESOURCESDIR)
	@$(MAKEDIR) $(DATADIR)

delete: clean
	@echo "> Deleting build directory..."
	@$(DELDIR) $(BUILDDIR)
