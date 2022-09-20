#include "GameState.h"

EM_JS(int, random_num, (int max), {
return Math.round(Math.random() * max);
});

void init_grid() {
    for (int y = 0; y < GridHeight; y++) {
        for (int x = 0; x < GridWidth; x++) {
            grid[x][y] = Cell(x * CellSize, y * CellSize, {0, 0, 0});
        }
    }
}

void render_grid(SDL_Renderer *renderer) {
    for (auto &row: grid) {
        for (auto &cell: row) {
            cell.render(renderer);
        }
    }
}

void set_grid_color(int x, int y, Color color) {
    auto &cell = grid[x][y];
    cell.set_color(color);
}

void Cell::render(SDL_Renderer *renderer) const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void GameState::update() {
    snake.update();
}

void GameState::render() {
    init_grid();
    snake.render();
    set_grid_color(food_point.x, food_point.y, { 255, 0, 0 });
}

void GameState::set_game_over() {
    game_state.game_over = true;
}

void GameState::set_food_point() {
    food_point.x = random_num(GridWidth - 1);
    food_point.y = random_num(GridHeight - 1);
}
