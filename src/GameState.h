#pragma once

#include "Shared.h"
#include "Snake.h"

struct Cell {
public:
    Cell() = default;

    Cell(int pos_x, int pos_y, Color color) : color(color) {
        rect.w = CellSize;
        rect.h = CellSize;
        rect.x = pos_x;
        rect.y = pos_y;
    }

    void render(SDL_Renderer *renderer) const;
    void set_color(Color color) { this->color = color; }
private:
    SDL_Rect rect;
    Color color;
};


inline static Cell grid[GridWidth][GridHeight];

void init_grid();
void render_grid(SDL_Renderer *renderer);
void set_grid_color(int x, int y, Color color);

struct Snake;
struct GameState {
    int score = 0;
    SDL_Point food_point;
    Snake snake;
    bool game_over = false;

    GameState() {
        set_food_point();
    }

    void update();
    void render();
    void set_food_point();
    void set_game_over();
};

inline static GameState game_state;

