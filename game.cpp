#include "game.h"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <cassert>
#include <algorithm>

Game::Game()
{
    moving_direction = Snake::MovingDirection::RIGHT;

    // Start page content
    sf::RectangleShape r1(sf::Vector2f(RECTANGLE_WIDTH, RECTANGLE_HEIGHT));
    sf::RectangleShape r2(sf::Vector2f(RECTANGLE_WIDTH, RECTANGLE_HEIGHT));
    sf::RectangleShape r3(sf::Vector2f(RECTANGLE_WIDTH, RECTANGLE_HEIGHT));
    sf::RectangleShape r4(sf::Vector2f(RECTANGLE_WIDTH, RECTANGLE_HEIGHT));
    sf::RectangleShape r5(sf::Vector2f(RECTANGLE_WIDTH, RECTANGLE_HEIGHT));
    const float verticalMargin = 40.f;
    r1.setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - RECTANGLE_WIDTH / 2, 100));
    r2.setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - RECTANGLE_WIDTH / 2, r1.getPosition().y + RECTANGLE_HEIGHT + verticalMargin));
    r3.setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - RECTANGLE_WIDTH / 2, r2.getPosition().y + RECTANGLE_HEIGHT + verticalMargin));
    r4.setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - RECTANGLE_WIDTH / 2, r3.getPosition().y + RECTANGLE_HEIGHT + verticalMargin));
    r5.setPosition(sf::Vector2f(SCREEN_WIDTH / 2 - RECTANGLE_WIDTH / 2, r4.getPosition().y + RECTANGLE_HEIGHT + verticalMargin));

    start_rectangles_options.push_back(r1);
    start_rectangles_options.push_back(r2);
    start_rectangles_options.push_back(r3);
    start_rectangles_options.push_back(r4);
    start_rectangles_options.push_back(r5);

    for (auto& e : start_rectangles_options)
    {
        e.setFillColor(StartRectangleColor);
        e.setOutlineThickness(3.f);
        e.setOutlineColor(sf::Color(30, 30, 30));
    }
    if (!font.loadFromFile("Alphabet.ttf"))
        exit(0);

    int i = 0;
    std::string texts_strings[5] = { "Start - solo (1)", "Start - duo (2)", "Register (3)", "Hall Of Fame (4)", "Instructions (5)"};
    int y_pos[5] = { 105, 225, 345, 462, 582 };

    for (int i = 0; i < start_rectangles_options.size(); ++i)
    {
        texts.push_back(sf::Text());
        texts[i].setFont(font);
        texts[i].setString(texts_strings[i]);
        texts[i].setCharacterSize(TEXT_FONT_SIZE);
        texts[i].setFillColor(sf::Color::White);
        sf::FloatRect textBounds = texts[i].getLocalBounds();
        float textWidth = textBounds.width;
        float x_pos = (SCREEN_WIDTH - textWidth) / 2.0f;
        texts[i].setPosition(sf::Vector2f(x_pos, y_pos[i]));
    }
    assert(texts.size() == start_rectangles_options.size());

    // Instruction page content
    sf::Vector2f rectSize = instruction_rectangle.getSize();
    instruction_rectangle.setPosition(
        (SCREEN_WIDTH - rectSize.x) / 2, // Center horizontally
        (SCREEN_HEIGHT - rectSize.y) / 2  // Center vertically
    );
    instruction_rectangle.setFillColor(StartRectangleColor);
    instruction_text.setFont(font);
    instruction_text.setFillColor(sf::Color::White);
    instruction_text.setCharacterSize(INSTRUCTION_TEXT_SIZE);
    std::string t = "Welcome to the Snake Game!\n"
        "Use arrow keys or a-w-s-d to move.\n"
        "Play in solo or duo mode.\n"
        "Collect foods and avoid bombs.\n"
        "Register yourself and compete with others.\n"
        "Click 'Escape' to go back.";
    instruction_text.setString(t);
    sf::FloatRect textBounds = instruction_text.getLocalBounds();
    sf::Vector2f rectPosition = instruction_rectangle.getPosition();
    instruction_text.setPosition(
        rectPosition.x + (rectSize.x - textBounds.width) / 2 - textBounds.left,
        rectPosition.y + (rectSize.y - textBounds.height) / 2 - textBounds.top
    );

    /*
        Registration page content
    */
    register_r.setPosition(50, 150);
    register_r.setOutlineThickness(3.0f);
    register_r.setOutlineColor(sf::Color::Black);
    name_text.setFont(font);
    name_text.setCharacterSize(INSTRUCTION_TEXT_SIZE);
    name_text.setFillColor(sf::Color::Black);

    name_text_start.setFont(font);
    name_text_start.setCharacterSize(INSTRUCTION_TEXT_SIZE);
    name_text_start.setFillColor(sf::Color::White);
}

void Game::main_page()
{
    States current_state = States::DEFAULT;
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake - Mateusz Wieczorek");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape &&
                    current_state == States::DEFAULT))
                window.close();

            if (event.type == sf::Event::KeyPressed && current_state == States::REGISTER)
            {
                if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Enter)
                {
                    current_state = States::DEFAULT;
                    if (registered_name.size())
                    {
                        // Create new player or witch to existing
                        bool exist = false;
                        for (int i = 0; i < playing_players.size(); i++)
                        {
                            if (playing_players.at(i).name == registered_name)
                            {
                                // Set index for previous player

                                std::cout << "Logging as: " << registered_name << std::endl;
                                player_playing_index = i;
                                std::cout << "Index = " << player_playing_index << std::endl;
                                exist = true;
                                break;
                            }
                        }
                        if (!exist)
                        {
                            playing_players.push_back(Player(registered_name));
                            player_playing_index = playing_players.size() - 1;
                            std::cout << "Creating new player: " << registered_name << std::endl;
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Backspace)
                {
                    if (registered_name.size() > 0)
                        registered_name.erase(registered_name.begin() + registered_name.size() - 1);
                }
                else if (event.key.code == sf::Keyboard::Space)
                {
                    registered_name += ' ';
                }
                else
                {
                    registered_name += static_cast<char>(97 + event.key.code);
                }
            }

            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Num1:
                    current_state = States::START;
                    snake.set_sleep(SINGLE_SLEEP_TIME);
                    break;
                case sf::Keyboard::Num2:
                    current_state = States::START_DUO;
                    snake.set_sleep(DUO_SLEEP_TIME);
                    break;
                case sf::Keyboard::Num3:
                    current_state = States::REGISTER;
                    // Reset registered name
                    this->registered_name.clear();
                    break;
                case sf::Keyboard::Num4:
                    current_state = States::HALL_OF_FAME;
                    break;
                case sf::Keyboard::Num5:
                    current_state = States::INSTRUCTION;
                    break;
                case sf::Keyboard::Escape:
                    current_state = States::DEFAULT;
                    break;
                default:
                    break;
                }
            }
        }
        window.clear(BACKGROUND_COLOR);

        // Bottom text
        sf::FloatRect textBounds = name_text_start.getLocalBounds();
        float textWidth = textBounds.width;
        float x = (SCREEN_WIDTH - textWidth) / 2.0f;

        // Render based on the current state
        switch (current_state)
        {
        case States::DEFAULT:
            for (size_t i = 0; i < start_rectangles_options.size(); ++i)
            {
                window.draw(start_rectangles_options[i]);
                window.draw(texts[i]);
            }

            if (registered_name.size() == 0)
                registered_name = "UNKNOWN";
            name_text_start.setString("You are logged as: " + registered_name);
            name_text_start.setPosition(sf::Vector2f(x, SCREEN_HEIGHT - 70));
            name_text_start.setFillColor(sf::Color::White);
            window.draw(name_text_start);

            break;

        case States::START:
            window.close();
            snake.reset();
            this->game_loop();
            window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake - Mateusz Wieczorek"); // Reopen the window
            current_state = States::DEFAULT;
            break;

        case States::START_DUO:
            window.close();
            this->game_loop_duo();
            window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake - Mateusz Wieczorek"); // Reopen the window
            current_state = States::DEFAULT;
            break;

        case States::INSTRUCTION:
            this->instruction_page(window);
            break;

        case States::REGISTER:
            this->registration_page(window);
            break;

        case States::HALL_OF_FAME:
            this->hall_of_fame_page(window);
            break;

        default:
            break;
        }
        window.display();
    }
}


void Game::game_loop()
{
    auto start = std::chrono::high_resolution_clock::now();
    bool lost = false;

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake - Mateusz Wieczorek");
    while (window.isOpen() && !lost)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                    moving_direction = Snake::MovingDirection::TOP;
                else if (event.key.code == sf::Keyboard::Down)
                    moving_direction = Snake::MovingDirection::BOTTOM;
                else if (event.key.code == sf::Keyboard::Left)
                    moving_direction = Snake::MovingDirection::LEFT;
                else if (event.key.code == sf::Keyboard::Right)
                    moving_direction = Snake::MovingDirection::RIGHT;
            }
        }
        if (snake.lost())
        {
            lost = true;
            Sleep(500);
            window.clear();
            board.reset_duo_points();
            board.draw_top_info(window, "You Lost!");
            window.display();
            Sleep(2500);

            if (player_playing_index != -1)
            {
                Player& player = playing_players[player_playing_index];
                player.games_played++;
                player.points += board.get_points();
                board.set_points(0);
            }
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        duration_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start).count();
        if (duration_time > 0 && duration_time % TIME_TO_GENERATE_FRUIT == 0)
        {
            start = std::chrono::high_resolution_clock::now();
            board.add_fruit();
            board.add_bomb();
        }

        if (!snake.lost())
        {
            window.clear(BACKGROUND_COLOR);
            this->draw(window, snake, moving_direction);
            window.display();
        }
    }
    snake.reset();
    board.reset();
}

void Game::game_loop_duo()
{
    board.start_duo_points();
    Snake second_snake(true);
    second_snake.set_sleep(DUO_SLEEP_TIME);
    Snake::MovingDirection second_moving_direction = Snake::RIGHT;

    auto start = std::chrono::high_resolution_clock::now();
    bool lost = false;
    bool second_lost = false;

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake - Mateusz Wieczorek");
    while (window.isOpen() && !lost && !second_lost)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                // For ordinal snake
                if (event.key.code == sf::Keyboard::Up)
                    moving_direction = Snake::MovingDirection::TOP;
                else if (event.key.code == sf::Keyboard::Down)
                    moving_direction = Snake::MovingDirection::BOTTOM;
                else if (event.key.code == sf::Keyboard::Left)
                    moving_direction = Snake::MovingDirection::LEFT;
                else if (event.key.code == sf::Keyboard::Right)
                    moving_direction = Snake::MovingDirection::RIGHT;
                
                // For second snake
                if (event.key.code == sf::Keyboard::W)
                    second_moving_direction = Snake::MovingDirection::TOP;
                else if (event.key.code == sf::Keyboard::S)
                    second_moving_direction = Snake::MovingDirection::BOTTOM;
                else if (event.key.code == sf::Keyboard::A)
                    second_moving_direction = Snake::MovingDirection::LEFT;
                else if (event.key.code == sf::Keyboard::D)
                    second_moving_direction = Snake::MovingDirection::RIGHT;
            }
        }
        if (snake.lost())
        {
            lost = true;
            Sleep(500);
            window.clear();
            board.draw_top_info(window, "Second Player Win!");
            window.display();
            Sleep(2500);
            board.set_points(0);
            board.reset_duo_points();
        }
        if (second_snake.lost())
        {
            second_lost = true;
            Sleep(500);
            window.clear();
            board.draw_top_info(window, "First Player Win!");
            window.display();
            Sleep(2500);
            board.set_points(0);
            board.reset_duo_points();
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        duration_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start).count();
        if (duration_time > 0 && duration_time % TIME_TO_GENERATE_FRUIT == 0)
        {
            start = std::chrono::high_resolution_clock::now();
            board.add_fruit();
            board.add_bomb();
        }

        if (!snake.lost() && !second_snake.lost())
        {
            window.clear(BACKGROUND_COLOR);
            draw(window, snake, moving_direction, false);
            draw(window, second_snake, second_moving_direction, true);
            char snake_end = board.check_snake_snake_collision(snake.get_head(), second_snake.get_head());
            if (snake_end == 's')
            {
                snake.set_lost(true);
                board.play_collision_sound();
            }
            window.display();
        }
    }
    snake.reset();
    second_snake.reset();
    board.reset();
}

void Game::draw(sf::RenderWindow& window, Snake& snake_current, Snake::MovingDirection dir, bool duo)
{
    board.draw_board(window);
    board.draw_top_info(window, "");
    board.remove_last_snake(snake_current);
    snake_current.update(dir);
    bool collision = board.check_fruit_snake_collision(snake_current.get_head(), duo);
    bool end_game = board.check_bomb_snake_collision(snake_current.get_head());
    if (end_game) snake_current.set_lost(true);
    snake_current.update_back(collision);
    board.update_grid_snake(snake_current);
    snake_current.draw(window);
}

void Game::set_moving_direction(Snake::MovingDirection d)
{
    moving_direction = d;
}

void Game::instruction_page(sf::RenderWindow& window)
{
    window.draw(instruction_rectangle);
    window.draw(instruction_text);
}

void Game::registration_page(sf::RenderWindow& window)
{
    name_text.setString(registered_name);
    sf::FloatRect r_textBounds = name_text.getLocalBounds();
    float textWidth = r_textBounds.width;
    float r_x_pos = (SCREEN_WIDTH - textWidth) / 2.0f;
    name_text.setPosition(sf::Vector2f(r_x_pos, 160));
    window.draw(register_r);
    window.draw(name_text);
}

int min(int a, int b)
{
    if (a > b)
        return b;
    return a;
}

void Game::hall_of_fame_page(sf::RenderWindow& window)
{
    std::vector<sf::RectangleShape> rectangles;
    int positions[5] = { 115, 265, 415, 565, 715 };
    sf::Color colors[5] =
    {
        sf::Color(255, 215, 0),  // Gold
        sf::Color(192, 192, 192), // Silver
        sf::Color(205, 127, 50),  // Bronze
        sf::Color(135, 206, 250), // Light Blue
        sf::Color(255, 105, 180)  // Hot Pink
    };

    std::sort(playing_players.begin(), playing_players.end(),
        [](const Player& a, const Player& b) 
        {
            return (a.points > b.points) || (a.points == b.points && a.games_played < b.games_played);
        });

    for (int i = 0; i < min(5, playing_players.size()); i++)
    {
        sf::Vector2f(SCREEN_WIDTH - 100, (SCREEN_HEIGHT - 200) / 5);
        rectangles.push_back(sf::RectangleShape(sf::Vector2f(SCREEN_WIDTH - 100, (SCREEN_HEIGHT - 200) / 5)));
        rectangles[i].setFillColor(colors[i]);
        rectangles[i].setOutlineThickness(1.f);
        rectangles[i].setOutlineColor(sf::Color(30, 30, 30));
        rectangles[i].setPosition(50, (SCREEN_HEIGHT - instruction_rectangle.getSize().y) / 2 + i * (instruction_rectangle.getSize().y / 5));
    }

    int change = instruction_rectangle.getSize().y / 5;
    int y_pos[5] = {};
    for (int i = 0; i < 5; i++)
        y_pos[i] = 110 + i * change;

    std::vector<sf::Text> texts_players;
    for (int i = 0; i < playing_players.size(); i++)
    {
        Player player = playing_players.at(i);
        std::string s = player.name + "\nPoints: " + std::to_string(player.points) + "\nGames Played: " + std::to_string(player.games_played) + "\n";

        texts_players.push_back(sf::Text());
        texts_players[i].setFont(font);
        texts_players[i].setString(s);
        texts_players[i].setCharacterSize(HALL_TEXT_FONT_SIZE);
        texts_players[i].setFillColor(sf::Color::White);
        sf::FloatRect textBounds = texts_players[i].getLocalBounds();
        float textWidth = textBounds.width;
        float x_pos = (SCREEN_WIDTH - textWidth) / 2.0f;
        texts_players[i].setPosition(sf::Vector2f(x_pos, y_pos[i]));
    }

    window.draw(instruction_rectangle);

    for (int i = 0; i < texts_players.size(); i++)
    {
        window.draw(rectangles[i]);
        window.draw(texts_players[i]);
    }
}