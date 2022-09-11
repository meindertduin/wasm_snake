#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>
#include <array>
#include <vector>

constexpr int CanvasWidth = 500;
constexpr int CanvasHeight = 500;
constexpr int CellSize = 50;
constexpr int GridWidth = CanvasWidth / CellSize;
constexpr int GridHeight = CanvasHeight / CellSize;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) { }
};

struct Cell {
public:
    Cell() = default;
    Cell(int pos_x, int pos_y, Color color) : color(color) {
        rect.w = CellSize;
        rect.h = CellSize;
        rect.x = pos_x;
        rect.y = pos_y;
    }

    void render(SDL_Renderer *renderer) const {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    void set_color(Color color) {
        this->color = color;
    }

private:
    SDL_Rect rect;
    Color color;
};

Cell grid[GridWidth][GridHeight];

void init_grid() {
    for (int y = 0; y < GridHeight; y++) {
        for (int x = 0; x < GridWidth; x++) {
            grid[x][y] = Cell(x * CellSize, y * CellSize, { 0, 0, 0});
        }
    }
}

void render_grid(SDL_Renderer *renderer) {
    for (auto &row : grid) {
        for (auto & cell : row) {
            cell.render(renderer);
        }
    }
}

void set_grid_color(int x, int y, Color color) {
    auto &cell = grid[x][y];
    cell.set_color(color);
}

struct Snake;

struct GameState {
    int score = 0;
    SDL_Point food_point;
    Snake snake;

    void render();
} game_state;

struct Snake {
public:
    void render() {
        for (auto &point : body_points) {
            set_grid_color()
        }
    }
private:
    int length { 3 };
    std::vector<SDL_Point> body_points;
    enum class Direction {
        Up,
        Down,
        Right,
        Left,
    };
};

struct context
{
    SDL_Renderer *renderer;
    int iteration;
};

void mainloop(void *arg)
{
    auto *ctx = static_cast<context*>(arg);
    SDL_Event  event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            auto key = event.key.keysym.sym;
            switch () {

            }
        }
    }

    SDL_Renderer *renderer = ctx->renderer;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    set_grid_color(0, 0, Color { 255, 0, 0 });
    render_grid(renderer);

    SDL_RenderPresent(renderer);
    ctx->iteration++;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(CanvasWidth, CanvasHeight, 0, &window, &renderer);

    context ctx{};
    ctx.iteration = 0;
    ctx.renderer = renderer;

    init_grid();
    const int simulate_infinite_loop = 1; // call the function repeatedly
    const int fps = -1; // call the function as fast as the browser wants to render (typically 60fps)
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void GameState::render() {
    snake.render();
}