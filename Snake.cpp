#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    // Create a window with a title and size
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Main loop to keep the window open
    while (window.isOpen())
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close the window if the user presses the close button
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with a color
        window.clear(sf::Color::Black);

        // Draw here (if needed)

        // Display the contents of the window
        window.display();
    }

    std::cout << "Goodbye World!\n";
    return 0;
}
