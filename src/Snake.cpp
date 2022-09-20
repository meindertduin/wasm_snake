#include "Snake.h"
#include "GameState.h"

EM_JS(void, update_score, (int score), {
    document.getElementById('score').innerText = score.toString();
});

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

