#pragma once

#include "board.h"
#include "snake.h"
#include <string>

const sf::Color StartRectangleColor(128, 128, 128, 255);

class Game
{
private:
	Board board;
	Snake snake;
	Snake::MovingDirection moving_direction;
	long long int duration_time = 0;
	const int TIME_TO_GENERATE_FRUIT = 3;

	// Constants about start page things location
	const int RECTANGLE_HEIGHT = 80;
	const int RECTANGLE_WIDTH = 220;
	std::vector<sf::RectangleShape> start_rectangles_options;

	// For text
	sf::Font font;
	sf::Color font_color = sf::Color::White;
	std::vector<sf::Text> texts;
	const int TEXT_FONT_SIZE = 50;

	// States for start page
	enum States
	{
		START = 's',
		START_DUO = 't',
		REGISTER = 'r',
		HALL_OF_FAME = 'h',
		INSTRUCTION = 'i',
		DEFAULT = 'd',
	};

	sf::RectangleShape instruction_rectangle{ sf::Vector2f(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 200) };
	sf::Text instruction_text;
	const int INSTRUCTION_TEXT_SIZE = 50;

	std::string registered_name{};
	sf::Text name_text, name_text_start;
	sf::RectangleShape register_r{ sf::Vector2f(SCREEN_WIDTH - 100, RECTANGLE_HEIGHT) };

	struct Player
	{
		int points;
		std::string name;
		int games_played;

		Player(std::string n)
			: name(n), points(0), games_played(0) {}
	};
	std::vector<Player> playing_players;
	int player_playing_index = -1;
	int previous_playing_player_index = -1;
	const int HALL_TEXT_FONT_SIZE = 35;
	const int DUO_SLEEP_TIME = 45;
	const int SINGLE_SLEEP_TIME = 90;
	const int EXPERIMENT_SLEEP_TIME = 0;


public:
	Game();

	void game_loop();
	void game_loop_duo();
	void draw(sf::RenderWindow& window, Snake& snake, Snake::MovingDirection dir, bool duo = false);
	void set_moving_direction(Snake::MovingDirection);

	void main_page();
	void instruction_page(sf::RenderWindow&);
	void registration_page(sf::RenderWindow&);
	void hall_of_fame_page(sf::RenderWindow&);
};

