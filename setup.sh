#!/bin/sh

cwd=$(pwd)

if [ ! -d .tmp ]; then
    mkdir .tmp
fi

cd .tmp

# SDL2_image
if [ ! -f SDL_image.zip ]; then
    curl -L 'https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.zip' -o SDL_image.zip
    unzip SDL_image.zip -d SDL_image
    cd SDL_image-2.0.5
    ./configure --disable-sdltest CPPFLAGS='-I/usr/local/include/SDL2' LDFLAGS='-L/usr/local/lib' LIBS='-lSDL2'
    make clean && make VERBOSE=1
    mkdir -p ${cwd}/libs/SDL2_image/include/SDL2_image/ ${cwd}/libs/SDL2_image/lib/
    cp SDL_image.h ${cwd}/libs/SDL2_image/include/SDL2_image
    cp .libs/libSDL2_image-2.0.0.dylib ${cwd}/libs/SDL2_image/lib/
fi

# SDL_ttf
if [ ! -f SDL_ttf.zip ]; then
    curl -L 'https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.15.zip' -o SDL_ttf.zip
    unzip SDL_ttf.zip
    cd SDL2_ttf-2.0.15
    ./configure --disable-sdltest CPPFLAGS='-I/usr/local/include/SDL2' LDFLAGS='-L/usr/local/lib' LIBS='-lSDL2'
    make clean && make VERBOSE=1
    mkdir -p ${cwd}/libs/SDL2_ttf/include/SDL2_ttf ${cwd}/libs/SDL2_ttf/lib/
    cp SDL_ttf.h ${cwd}/libs/SDL2_ttf/include/SDL2_ttf
    cp .libs/libSDL2_ttf-2.0.0.dylib ${cwd}/libs/SDL2_ttf/lib/
fi
