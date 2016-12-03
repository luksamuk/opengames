#!/bin/bash
g++ --std=c++11 dagarden.cpp -o dagarden.bin -loficinad -lSDL2 -lGLEW -lGL -lSDL2_image -lopenal -logg -lvorbis -lvorbisfile -lphysfs
cd resources
./prepare_assets.sh
cd ..
