#!/bin/bash

emcc src/main.cpp -s WASM=1 -s USE_SDL=2 -O3 -o index.js