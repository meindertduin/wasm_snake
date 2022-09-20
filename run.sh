#!/bin/bash

emcc src/main.cpp src/GameState.cpp src/Snake.cpp -std=c++20 -s WASM=1 -s USE_SDL=2 -O3 -o index.js -s EXPORTED_FUNCTIONS='['_main', '_on_input']'