#include "board.h"
#include <iostream>
#include <cstdlib>
#include <ctime> 

Board::Board()
    : grid(20, std::vector<char>(20, '-'))
{
    this->points = 0;
    if (font.loadFromFile("Alphabet.ttf"))
        std::cout << "Succesfully load Alphabet font..." << std::endl;
    else
    {
        std::cerr << "Cannot load font .ttf file\n";
        exit(0);
    }
    if (!fruit_texture.loadFromFile("fruit.png"))
    {
        std::cerr << "Cannot load fruit png file\n";
        exit(0);
    }
    else
        std::cout << "Succesfully load fruit image..." << std::endl;
    if (!bomb_texture.loadFromFile("bomb.png"))
    {
        std::cerr << "Cannot load bomb png file\n";
        exit(0);
    }
    else
        std::cout << "Succesfully load bomb image..." << std::endl;
    fruit_texture.setSmooth(true);
    fruit_texture.setRepeated(false);
    fruit_square.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    fruit_square.setTexture(&fruit_texture);

    bomb_texture.setSmooth(true);
    bomb_texture.setRepeated(false);
    bomb_square.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
    bomb_square.setTexture(&bomb_texture);

    points_text.setFont(font);
    points_text.setString("Points: " + std::to_string(this->points));
    points_text.setCharacterSize(SCORE_FONT_SIZE);
    points_text.setFillColor(this->font_color);
    points_text.setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - SCORE_FONT_SIZE, 30));


}

void Board::draw_board(sf::RenderWindow& window)
{/*
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 20; col++)
        {
            std::cout << grid[row][col] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";*/

    sf::VertexArray vertical_lines(sf::Lines, NUMBER_VERTICAL_SQUARES * 2);
    sf::VertexArray horizontal_lines(sf::Lines, NUMBER_HORIZONTAL_SQUARES * 2);

    for (int i = 0; i < NUMBER_VERTICAL_SQUARES; i++)
    {
        double y_pos = SQUARE_HEIGHT * i + 200; // 200 beacuse we will use 200 upper pixels for displaying the score
        vertical_lines[i * 2] = sf::Vertex(sf::Vector2f(0, y_pos));
        vertical_lines[i * 2 + 1] = sf::Vertex(sf::Vector2f(SCREEN_WIDTH, y_pos));
    }
    for (int i = 0; i < NUMBER_HORIZONTAL_SQUARES; i++)
    {
        double x_pos = SQUARE_WIDTH * i;
        horizontal_lines[i * 2] = sf::Vertex(sf::Vector2f(x_pos, 200));
        horizontal_lines[i * 2 + 1] = sf::Vertex(sf::Vector2f(x_pos, SCREEN_HEIGHT));
    }
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 20; col++)
        {
            if (grid[row][col] == 'f')
            {
                float x_pos = col * SQUARE_SIZE;
                float y_pos = 200 + row * SQUARE_SIZE;
                fruit_square.setPosition(sf::Vector2f(x_pos, y_pos));
                window.draw(fruit_square);
            }
            else if (grid[row][col] == 'b')
            {
                float x_pos = col * SQUARE_SIZE;
                float y_pos = 200 + row * SQUARE_SIZE;
                bomb_square.setPosition(sf::Vector2f(x_pos, y_pos));
                window.draw(bomb_square);
            }
        }
    }
    window.draw(vertical_lines);
    window.draw(horizontal_lines);
}

void Board::draw_top_info(sf::RenderWindow& window, const std::string& msg)
{
    points_text.setString("Points: " + std::to_string(this->points));
    if (msg.size())
        points_text.setString(msg);
    window.draw(points_text);
}

bool Board::check_fruit_snake_collision(Point& head)
{
    Point new_point = head;
    if (new_point.square_row >= NUMBER_HORIZONTAL_SQUARES)
        head.square_row = 0;
    if (new_point.square_row < 0)
        head.square_row = NUMBER_HORIZONTAL_SQUARES - 1;
    if (new_point.square_col >= NUMBER_VERTICAL_SQUARES)
        head.square_col = 0;
    if (new_point.square_col < 0)
        head.square_col = NUMBER_VERTICAL_SQUARES - 1;

    Point snake_head = head;
    int row = snake_head.square_row;
    int col = snake_head.square_col;
    if (grid[row][col] == 'f')
    {
        this->play_fruit_grabbing_sound();
        this->points += 1;
        return true;
    }
    return false;
}

bool Board::check_bomb_snake_collision(Point& head)
{
    Point snake_head = head;
    int row = snake_head.square_row;
    int col = snake_head.square_col;
    if (grid[row][col] == 'b')
    {
        this->play_bomb_sound();
        return true;
    }
    return false;
}

void Board::add_fruit()
{
    if (this->fruit_amount >= MAX_FRUITS)
        return;
    srand((unsigned)time(0));

    bool run = true;
    int row;
    int column;

    while (run)
    {
        row = (rand() % NUMBER_VERTICAL_SQUARES);
        column = (rand() % NUMBER_HORIZONTAL_SQUARES);
        if (grid[row][column] == '-')
            run = false;
    }
    this->grid[row][column] = 'f';
    this->fruit_amount++;
}

void Board::add_bomb()
{
    if (this->bomb_amount >= MAX_BOMBS)
        return;
    srand((unsigned)time(0));

    bool run = true;
    int row;
    int column;

    while (run)
    {
        row = (rand() % NUMBER_VERTICAL_SQUARES);
        column = (rand() % NUMBER_HORIZONTAL_SQUARES);
        if (grid[row][column] == '-')
            run = false;
    }
    this->grid[row][column] = 'b';
    this->bomb_amount++;
}

int Board::get_points()
{
    return points;
}

void Board::set_points(int p)
{
    points = p;
}

void Board::reset()
{
    for (int i = 0; i < grid.size(); i++)
        for (int j = 0; j < grid[i].size(); j++)
            grid[i][j] = '-';
}

void Board::remove_last_snake(const Snake& snake)
{
    std::vector<Point> points = snake.get_snake_squares();
    for (int i = 0; i < points.size(); i++)
    {
        int row = points.at(i).square_row;
        int col = points.at(i).square_col;
        grid[row][col] = '-';
    }
}

void Board::update_grid_snake(const Snake& snake)
{
    std::vector<Point> points = snake.get_snake_squares();
    for (int i = 0; i < snake.get_snake_squares().size(); i++)
    {
        int row = points.at(i).square_row;
        int col = points.at(i).square_col;
        grid[row][col] = 's';
    }
}

void Board::play_fruit_grabbing_sound()
{

}

void Board::play_bomb_sound()
{

}