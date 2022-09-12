#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>

#include <array>
#include <vector>
#include <queue>
#include <random>

constexpr int CanvasWidth = 500;
constexpr int CanvasHeight = 500;
constexpr int CellSize = 10;
constexpr int GridWidth = CanvasWidth / CellSize;
constexpr int GridHeight = CanvasHeight / CellSize;

EM_JS(int, random_num, (int max), {
    return Math.round(Math.random() * max);
});

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

struct SnakePart {
public:
    std::queue<Direction> move_queue;
    SDL_Point point;

    void move();

    void add(Direction direction) {
        move_queue.push(direction);
    }

    void render();
};

struct Snake {
public:
    Snake() {
        auto mid_grid_x = GridWidth / 2;
        auto mid_grid_y = GridHeight / 2;

        this->body_points = std::vector<SnakePart>(3);
        this->body_points[0].point = { mid_grid_x, mid_grid_y };
        this->body_points[1].point = { mid_grid_x - 1, mid_grid_y };
        this->body_points[2].point = { mid_grid_x - 2, mid_grid_y };

        this->direction = Direction::Right;

        this->body_points[1].add(direction);
        this->body_points[2].add(direction);
        this->body_points[2].add(direction);
    }

    void render();
    void update();

    void set_direction(Direction direction);

    void add_part();

    std::vector<SnakePart> body_points;
private:
    int length { 3 };
    Direction direction;
};


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
} game_state;

EM_JS(void, update_score, (int score), {
    document.getElementById('score').innerText = score.toString();
});

struct context
{
    SDL_Renderer *renderer;
};

void handle_input(int key) {
    switch (key) {
        case 0:
            game_state.snake.set_direction(Direction::Up);
            break;
        case 1:
            game_state.snake.set_direction(Direction::Down);
            break;
        case 2:
            game_state.snake.set_direction(Direction::Left);
            break;
        case 3:
            game_state.snake.set_direction(Direction::Right);
            break;
    }
}

extern "C" {
void on_input(int key) {
        handle_input(key);
    }
}

void mainloop(void *arg)
{
    auto ctx = static_cast<context*>(arg);
    SDL_Renderer *renderer = ctx->renderer;

    if (game_state.game_over) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
    } else {
        game_state.update();
        game_state.render();
        render_grid(renderer);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(100);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(CanvasWidth, CanvasHeight, 0, &window, &renderer);

    context ctx{};
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

void SnakePart::render() {
    set_grid_color(point.x, point.y, { 255, 255, 0 });
}

void SnakePart::move() {
    Direction direction = move_queue.front();
    move_queue.pop();

    if (direction == Direction::Up) {
        point.y -= 1;
        if (point.y < 0) {
            point.y = GridHeight - 1;
        }
        return;
    }

    if (direction == Direction::Down) {
        point.y += 1;
        if (point.y > GridHeight -1) {
            point.y = 0;
        }
        return;
    }

    if (direction == Direction::Left) {
        point.x -= 1;
        if (point.x < 0) {
            point.x = GridWidth - 1;
        }
        return;
    }

    if (direction == Direction::Right) {
        point.x += 1;

        if (point.x > GridWidth - 1) {
            point.x = 0;
        }
        return;
    }
}

void Snake::render() {
    for (auto &part : body_points) {
        part.render();
    }
}

void Snake::update() {
    // move each part
    for (auto &part : body_points) {
        part.add(direction);
        part.move();
    }

    if(body_points[0].point.x == game_state.food_point.x
        && body_points[0].point.y == game_state.food_point.y)
    {
        game_state.set_food_point();
        game_state.score += 10;
        update_score(game_state.score);
        this->add_part();
    }

    // check if snake is not hitting himself
    for (auto it = body_points.begin() + 1; it != body_points.end(); it++) {
        if (body_points[0].point.x == it->point.x && body_points[0].point.y == it->point.y) {
            game_state.set_game_over();
            break;
        }
    }
}

void Snake::set_direction(Direction direction) {
    switch (direction) {
        case Direction::Up:
            if (this->direction == Direction::Down)
                return;
            break;
        case Direction::Down:
            if (this->direction == Direction::Up)
                return;
            break;
        case Direction::Left:
            if (this->direction == Direction::Right)
                return;
            break;
        case Direction::Right:
            if (this->direction == Direction::Left)
                return;
            break;
    }
    this->direction = direction;
}

void Snake::add_part() {
    SnakePart snakePart;
    auto &last_part = this->body_points[this->body_points.size() - 1];
    auto move_queue = last_part.move_queue;
    snakePart.move_queue.push(move_queue.front());

    while (!move_queue.empty()) {
        snakePart.move_queue.push(move_queue.front());
        move_queue.pop();
    }

    snakePart.point = last_part.point;

    switch (snakePart.move_queue.front()) {
        case Direction::Up:
            snakePart.point.y += 1;
            break;
        case Direction::Down:
            snakePart.point.y -= 1;
            break;
        case Direction::Left:
            snakePart.point.x += 1;
            break;
        case Direction::Right:
            snakePart.point.x -= 1;
            break;
    }

    this->body_points.push_back(snakePart);
}

void GameState::set_food_point() {
    food_point.x = random_num(GridWidth - 1);
    food_point.y = random_num(GridHeight - 1);
}