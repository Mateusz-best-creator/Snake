#pragma once

#include <SFML/Graphics.hpp>

const int SQUARE_SIZE = 35;
const int NUMBER_VERTICAL_SQUARES = 20;
const int NUMBER_HORIZONTAL_SQUARES = 20;

struct Point
{
	int square_row, square_col;
};

class Snake
{
public:
	enum MovingDirection
	{
		LEFT = 'l',
		RIGHT = 'r',
		BOTTOM = 'b',
		TOP = 't',
	};

private:
	std::vector<Point> snake_squares;
	MovingDirection current_moving_direction;
	Point head;
	const int SCREEN_SLEEP_TIME = 90;
	Point last_snake_square;
	bool lost_the_game = false;

public:
	Snake();
	void reset();
	void update(MovingDirection);
	void update_back(bool);
	void draw(sf::RenderWindow&);
	bool lost();
	void set_lost(bool);
	Point& get_head();
	const std::vector<Point>& get_snake_squares() const;
};