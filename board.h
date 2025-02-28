#pragma once

#include "SFML/Audio.hpp"
#include <vector>
#include "snake.h"

const int SCREEN_WIDTH = 700, SCREEN_HEIGHT = 900;
const sf::Color BACKGROUND_COLOR(144, 238, 144, 255);

class Board
{
private:
	const int SQUARE_SIZE = 35;
	int NUMBER_VERTICAL_SQUARES = 20;
	int NUMBER_HORIZONTAL_SQUARES = 20;
	int BOARD_SIZE;
	double SQUARE_HEIGHT;
	double SQUARE_WIDTH;

	const int SCORE_FONT_SIZE = 100;
	int points, duo_points = -1;
	sf::Text points_text;
	sf::Font font;
	sf::Color font_color = sf::Color::White;

	sf::RectangleShape fruit_square;
	sf::RectangleShape bomb_square;
	sf::Texture fruit_texture;
	sf::Texture bomb_texture;

	std::vector<std::vector<char>> grid;
	const static int MAX_FRUITS = 15, MAX_BOMBS = 10;
	int fruit_amount = 0, bomb_amount = 0;

	/*
		Sound effects
	*/
	sf::SoundBuffer fruit_buffer, bomb_buffer, collision_buffer;
	sf::Sound fruit_sound, bomb_sound, collision_sound;

	/*
		Enum characters for grid
	*/
	enum GridCharacters
	{
		Empty = 'e',
		Snake1 = 's',
		Snake2 = 'd',
		Fruit = 'f',
		Bomb = 'b'
	};
	int snake1counter = 3, snake2counter = 0;


public:
	Board(int board_size = 20);
	void draw_board(sf::RenderWindow&);
	void draw_top_info(sf::RenderWindow&, const std::string&);
	bool check_fruit_snake_collision(Point& head, bool duo = false);
	bool check_bomb_snake_collision(Point& head);
	char check_snake_snake_collision(Point& first_head, Point& second_head);
	void add_fruit();
	void add_bomb();

	int get_points();
	void set_points(int);
	void reset();
	void remove_last_snake(const Snake&);
	void update_grid_snake(Snake&);
	void play_fruit_grabbing_sound();
	void play_bomb_sound();
	void play_collision_sound();
	void start_duo_points() { this->duo_points = 0; };
	void reset_duo_points() { this->duo_points = -1; };
	int get_snake1counter();
	int get_snake2counter();

	int get_square_size() { return SQUARE_SIZE; }
	int get_nv_squares() { return NUMBER_VERTICAL_SQUARES; }
	int get_nh_squares() { return NUMBER_HORIZONTAL_SQUARES; }
};