// Aquarium Ecosystem Simulator
// Minimal starting point: opens a window and draws a placeholder "fish"
// (a circle) so we can confirm SFML + CMake + VS Code are wired up
// correctly before Lab 1 (Abstract Factory) adds real structure.

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Aquarium Ecosystem Simulator");
    window.setFramerateLimit(60);

    // Placeholder "fish": just a circle for now.
    sf::CircleShape fish(15.f);
    fish.setFillColor(sf::Color(255, 165, 0)); // orange
    fish.setPosition(400.f, 300.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(10, 40, 80)); // dark blue "water"
        window.draw(fish);
        window.display();
    }

    return 0;
}
