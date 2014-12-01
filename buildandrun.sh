#!/bin/sh
g++ -o main main.cpp common/shaders.cpp common/texture.cpp common/controls.cpp -L/usr/lib -L/usr/local/lib -lglfw -lGLEW -lGL -lGLU
./main
