

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "2d Tilemap Editor");
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int c = 3;
    int s = 3;
    float size = 10.f;



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        mousePos = sf::Mouse::getPosition(window);
        //std::cout << mousePos.x;


        
        window.clear();

        window.display();

    }




	return 0;
}
















