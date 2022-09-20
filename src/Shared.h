#pragma once

#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>

#include <array>
#include <vector>
#include <random>
#include <queue>

static constexpr int CanvasWidth = 500;
static constexpr int CanvasHeight = 500;
static constexpr int CellSize = 10;
static constexpr int GridWidth = CanvasWidth / CellSize;
static constexpr int GridHeight = CanvasHeight / CellSize;

enum Direction {
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color() = default;

    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};
