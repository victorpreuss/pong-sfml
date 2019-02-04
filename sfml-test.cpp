#include <cstdio>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

void processEvents (sf::RenderWindow&);

int main () {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello World!", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);

    window.setVerticalSyncEnabled(true);

    sf::Texture texture;

    if (!texture.loadFromFile("/home/vpreuss/Downloads/me.jpg")) window.close();
    else printf("x = %d y = %d\n", texture.getSize().x, texture.getSize().y);

    texture.setSmooth(true);
    texture.setRepeated(true);

    sf::CircleShape circle(100);
    circle.setTexture(&texture);

    //sf::Sprite sprite;
    //sprite.setTexture(texture);

    while (window.isOpen()) {

        processEvents(window);   

        window.clear(sf::Color::Green);
        
        //sprite.setPosition(sf::Vector2f(200, 100));

        //window.draw(sprite);
        window.draw(circle);

        window.display();
    }

    return 0;
}

void processEvents (sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                printf("Closing window\n");
                window.close();
                break;

            case sf::Event::Resized:
                printf("new width: %d\n", event.size.width);
                printf("new height: %d\n", event.size.height);
                break;
            
            case sf::Event::TextEntered:
                // it is nice to test if the value of the character is < 128 (unicode)
                printf("key pressed: %c\n", static_cast<char>(event.text.unicode));
                break;

            case sf::Event::GainedFocus:
                printf("window gained focus\n");
                break;

            case sf::Event::LostFocus:
                printf("window lost focus\n");
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::A) {
                    printf("pressed A\n");
                    printf("%s\n", event.key.alt ? "true" : "false");
                    printf("%s\n", event.key.control ? "true" : "false");
                    printf("%s\n", event.key.shift ? "true" : "false");
                    printf("%s\n", event.key.system ? "true" : "false");
                }
                break;

            case sf::Event::KeyReleased:
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Right)
                    printf("right button: %d %d\n", event.mouseButton.x, event.mouseButton.y);
                if (event.mouseButton.button == sf::Mouse::Left)
                    printf("left button: %d %d\n", event.mouseButton.x, event.mouseButton.y);
                break;
        }
    }
}