#pragma once

#include <SFML/Graphics.hpp>

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
	int SCREEN_SLEEP_TIME = 45;
	Point last_snake_square;
	bool lost_the_game = false;
	bool ordinal;

public:
	Snake(bool duo = false);
	void reset();
	void update(MovingDirection, int, int);
	void update_back(bool);
	void draw(sf::RenderWindow&, int);
	bool lost();
	void set_lost(bool);
	Point& get_head();
	Point& get_tail();
	const std::vector<Point>& get_snake_squares() const;
	const bool& get_ordinal() const { return ordinal; }
	void set_sleep(int);
	int get_length();
};