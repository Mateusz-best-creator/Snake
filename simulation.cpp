#include "game.h"
#include <iostream>

void Game::simulation()
{
    Board simulation_board = Board(50);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake - Mateusz Wieczorek");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }
        window.clear(BACKGROUND_COLOR);
        simulation_board.draw_top_info(window, "AI Snake Simulation");
        simulation_board.draw_board(window);
        window.display();
    }
}