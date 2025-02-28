#include "board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Board::Board(int board_size)
    : grid(board_size, std::vector<char>(board_size, Empty))
{
    BOARD_SIZE = NUMBER_VERTICAL_SQUARES = NUMBER_HORIZONTAL_SQUARES = board_size;
    SQUARE_HEIGHT = (SCREEN_HEIGHT - 200) / NUMBER_VERTICAL_SQUARES;
    SQUARE_WIDTH = SCREEN_WIDTH / NUMBER_HORIZONTAL_SQUARES;
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

    if (!fruit_buffer.loadFromFile("point_beep.mp3"))
    {
        std::cerr << "Error loading sound for fruit.mp3\n";
        exit(0);
    }
    if (!bomb_buffer.loadFromFile("bomb_explosion.mp3"))
    {
        std::cerr << "Error loading sound for bomb.mp3\n";
        exit(0);
    }
    if (!collision_buffer.loadFromFile("collision.mp3"))
    {
        std::cerr << "Error loading sound for collision.mp3\n";
        exit(0);
    }
    fruit_sound.setBuffer(fruit_buffer);
    bomb_sound.setBuffer(bomb_buffer);
    collision_sound.setBuffer(collision_buffer);
}

void Board::draw_board(sf::RenderWindow& window)
{
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
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (grid[row][col] == Fruit)
            {
                float x_pos = col * SQUARE_SIZE;
                float y_pos = 200 + row * SQUARE_SIZE;
                fruit_square.setPosition(sf::Vector2f(x_pos, y_pos));
                window.draw(fruit_square);
            }
            else if (grid[row][col] == Bomb)
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
    if (msg.size())
        points_text.setString(msg);
    else if (duo_points != -1)
        points_text.setString("P1 points: " + std::to_string(points) + " P2 points: " + std::to_string(duo_points));
    else
        points_text.setString("Points: " + std::to_string(this->points));

    float width = points_text.getLocalBounds().width;
    float offset = width / 2.f;
    points_text.setPosition(sf::Vector2f(SCREEN_WIDTH / 2.f - offset, 30));
    window.draw(points_text);
}

bool Board::check_fruit_snake_collision(Point& head, bool duo)
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
    if (grid[row][col] == Fruit)
    {
        this->play_fruit_grabbing_sound();
        if (!duo)
        {
            this->points += 1;
            this->snake2counter;
        }
        else
        {
            this->duo_points += 1;
            this->snake1counter += 1;
        }
        return true;
    }
    return false;
}

bool Board::check_bomb_snake_collision(Point& head)
{
    Point snake_head = head;
    int row = snake_head.square_row;
    int col = snake_head.square_col;
    if (grid[row][col] == Bomb)
    {
        this->play_bomb_sound();
        return true;
    }
    return false;
}

char Board::check_snake_snake_collision(Point& first_head, Point& second_head)
{
    if (grid[first_head.square_row][first_head.square_col] == Snake2)
    {
        // second win
        return 's';
    }
    if (grid[second_head.square_row][second_head.square_col] == Snake1)
    {
        // first win
        return 'f';
    }
    // nothing happened
    return 'n';
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
        if (grid[row][column] == Empty)
            run = false;
    }
    this->grid[row][column] = Fruit;
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
        if (grid[row][column] == Empty)
            run = false;
    }
    this->grid[row][column] = Bomb;
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
            grid[i][j] = Empty;
    this->points = 0;
    fruit_amount = bomb_amount = 0;
    duo_points = -1;
    snake1counter = snake2counter = 3;
}

void Board::remove_last_snake(const Snake& snake)
{
    std::vector<Point> points = snake.get_snake_squares();
    for (int i = 0; i < points.size(); i++)
    {
        int row = points.at(i).square_row;
        int col = points.at(i).square_col;
        grid[row][col] = Empty;
    }
}

void Board::update_grid_snake(Snake& snake)
{
    std::vector<Point> points = snake.get_snake_squares();
    for (int i = 0; i < snake.get_snake_squares().size(); i++)
    {
        int row = points.at(i).square_row;
        int col = points.at(i).square_col;
        if (snake.get_ordinal())
        {
            grid[row][col] = Snake1;
        }
        else
        {
            if (grid[row][col] == Snake1
                ///&& snake.get_tail().square_row != row &&
                //snake.get_tail().square_col != col
                )
            {
                snake.set_lost(true);
                play_collision_sound();
            }
            grid[row][col] = Snake2;
        }
    }
}

void Board::play_fruit_grabbing_sound()
{
    this->fruit_sound.play();
}

void Board::play_bomb_sound()
{
    this->bomb_sound.play();
}

void Board::play_collision_sound()
{
    collision_sound.play();
}

int Board::get_snake1counter()
{
    return snake1counter;
}

int Board::get_snake2counter()
{
    return snake2counter;
}