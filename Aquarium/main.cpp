#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Aquarium Ecosystem Simulator");
    window.setFramerateLimit(60);

    sf::CircleShape fish(15.f);
    fish.setFillColor(sf::Color(255, 165, 0));
    fish.setPosition({400.f, 300.f});

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(10, 40, 80));
        window.draw(fish);
        window.display();
    }

    return 0;
}