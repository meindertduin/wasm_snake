#include "GameState.h"
#include "Snake.h"

struct Context {
    SDL_Renderer *renderer;
};

std::queue<int> input_queue;
extern "C" {
void on_input(int key) {
        input_queue.push(key);
    }
}

void mainloop(void *arg)
{
    auto ctx = static_cast<Context*>(arg);
    SDL_Renderer *renderer = ctx->renderer;

    if (!input_queue.empty()) {
        auto key = input_queue.back();
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

        while(!input_queue.empty()) input_queue.pop();
    }

    if (game_state.game_over) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
    } else {
        game_state.update();
        game_state.render();
        render_grid(renderer);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(CanvasWidth, CanvasHeight, 0, &window, &renderer);

    Context context{};
    context.renderer = renderer;

    init_grid();
    const int simulate_infinite_loop = 1;
    const int fps = -1; // call the function as fast as the browser wants to render (typically 60fps)
    emscripten_set_main_loop_arg(mainloop, &context, fps, simulate_infinite_loop);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
