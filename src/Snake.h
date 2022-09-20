#pragma once

#include "Shared.h"

struct SnakePart {
public:
    std::queue<Direction> move_queue;
    SDL_Point point;

    void move();
    void add(Direction direction) { move_queue.push(direction); }

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


