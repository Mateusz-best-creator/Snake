#include "snake.h"
#include <windows.h>
#include <iostream>
Snake::Snake(bool duo)
{
    if (!duo)
    {
        this->snake_squares = { {8,8}, {8, 9}, {8, 10} };
        this->current_moving_direction = MovingDirection::RIGHT;
        ordinal = true;
    }
    else
    {
        this->snake_squares = { {12,10}, {12, 9}, {12, 8} };
        this->current_moving_direction = MovingDirection::LEFT;
        ordinal = false;
    }
    head = snake_squares[snake_squares.size() - 1];
}

void Snake::reset()
{
    this->snake_squares = { {10,8}, {10, 9}, {10, 10} };
    this->current_moving_direction = MovingDirection::RIGHT;
    head = snake_squares[snake_squares.size() - 1];
    this->lost_the_game = false;
}

void Snake::update(MovingDirection d)
{
    if (!(current_moving_direction == MovingDirection::LEFT && d == MovingDirection::RIGHT ||
        current_moving_direction == MovingDirection::RIGHT && d == MovingDirection::LEFT ||
        current_moving_direction == MovingDirection::TOP && d == MovingDirection::BOTTOM ||
        current_moving_direction == MovingDirection::BOTTOM && d == MovingDirection::TOP) && d)
        current_moving_direction = d;
    int change_row = 0, change_col = 0;
    switch (current_moving_direction)
    {
    case Snake::LEFT:
        change_col--;
        break;
    case Snake::RIGHT:
        change_col++;
        break;
    case Snake::BOTTOM:
        change_row++;
        break;
    case Snake::TOP:
        change_row--;
        break;
    default:
        change_col++;
        break;
    }

    Point new_point = {
        snake_squares[snake_squares.size() - 1].square_row + change_row,
        snake_squares[snake_squares.size() - 1].square_col + change_col
    };
    head = new_point;
    if (new_point.square_row >= NUMBER_HORIZONTAL_SQUARES)
        new_point.square_row = 0;
    if (new_point.square_row < 0)
        new_point.square_row = NUMBER_HORIZONTAL_SQUARES - 1;
    if (new_point.square_col >= NUMBER_VERTICAL_SQUARES)
        new_point.square_col = 0;
    if (new_point.square_col < 0)
        new_point.square_col = NUMBER_VERTICAL_SQUARES - 1;

    last_snake_square = snake_squares.at(0);
    this->snake_squares.erase(this->snake_squares.begin());
    this->snake_squares.push_back(new_point);
}

void Snake::draw(sf::RenderWindow& window)
{
    Sleep(SCREEN_SLEEP_TIME);
    std::vector<sf::RectangleShape> snake;
    for (int i = 0; i < snake_squares.size(); i++)
    {
        snake.push_back(sf::RectangleShape(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE)));

        float x_pos = snake_squares[i].square_col * SQUARE_SIZE;
        float y_pos = 200 + snake_squares[i].square_row * SQUARE_SIZE;

        snake[i].setPosition(x_pos, y_pos);
        if (ordinal)
            snake[i].setFillColor(sf::Color::Blue);
        else
            snake[i].setFillColor(sf::Color(245, 29, 60, 255));
        window.draw(snake[i]);
    }
}

bool Snake::lost()
{
    if (lost_the_game)
        return true;
    for (int i = 0; i < snake_squares.size() - 1; i++)
        if (head.square_col == snake_squares[i].square_col &&
            head.square_row == snake_squares[i].square_row)
            return true;
    return false;
}

Point& Snake::get_head()
{
    return head;
}

const std::vector<Point>& Snake::get_snake_squares() const
{
    return this->snake_squares;
}

void Snake::update_back(bool colission)
{
    if (colission == false)
        return;
    this->snake_squares.insert(snake_squares.begin(), this->last_snake_square);
}

void Snake::set_lost(bool lost)
{
    this->lost_the_game = lost;
}

void Snake::set_sleep(int s)
{
    SCREEN_SLEEP_TIME = s;
}