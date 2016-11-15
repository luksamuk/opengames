D_COMPILER=dmd
D_OBJ_FLAG=-c
D_LIBS=-L-lDerelictUtil -L-lDerelictSDL2 -L-lDerelictGL3
DEXEC_FLAG=-of
EXEC=dsnake

.PHONY: clean

all: main.o scrmngr.o
	$(D_COMPILER) main.o scrmngr.o $(D_LIBS) $(DEXEC_FLAG)$(EXEC)

main.o: main.d
	$(D_COMPILER) $(D_OBJ_FLAG) main.d $(DEXEC_FLAG)main.o

scrmngr.o: game/screenmanager.d
	$(D_COMPILER) $(D_OBJ_FLAG) game/screenmanager.d $(DEXEC_FLAG)scrmngr.o

clean:
	rm -rf *.o

delete: $(EXEC)
	rm -f $(EXEC)