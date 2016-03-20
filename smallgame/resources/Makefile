#!/bin/sh
SYS := $(shell $(CXX) -dumpmachine)

# Linux routine
ifneq (, $(findstring linux, $(SYS)))
	PREPARE_ASSETS = ./prepare_assets.sh
endif
# Windows MinGW routine
ifneq (, $(findstring mingw, $(SYS)))
	PREPARE_ASSETS = prepare_assets.bat
endif

# MacOSX routine
# Coming soon

all:
	@echo Executing $(PREPARE_ASSETS)
	$(PREPARE_ASSETS)
