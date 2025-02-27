#pragma once

#include "SFML/Audio.hpp"
#include <vector>
#include "snake.h"

const int SCREEN_WIDTH = 700, SCREEN_HEIGHT = 900;
const sf::Color BACKGROUND_COLOR(144, 238, 144, 255);

class Board
{
private:
	const double SQUARE_HEIGHT = (SCREEN_HEIGHT - 200) / NUMBER_VERTICAL_SQUARES;
	const double SQUARE_WIDTH = SCREEN_WIDTH / NUMBER_HORIZONTAL_SQUARES;

	const int SCORE_FONT_SIZE = 100;
	int points;
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
	sf::SoundBuffer fruit_buffer, bomb_buffer;
	sf::Sound fruit_sound, bomb_sound;

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

public:
	Board();
	void draw_board(sf::RenderWindow&);
	void draw_top_info(sf::RenderWindow&, const std::string&);
	bool check_fruit_snake_collision(Point& head);
	bool check_bomb_snake_collision(Point& head);
	void add_fruit();
	void add_bomb();

	int get_points();
	void set_points(int);
	void reset();
	void remove_last_snake(const Snake&);
	void update_grid_snake(const Snake&);
	void play_fruit_grabbing_sound();
	void play_bomb_sound();
};