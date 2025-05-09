// IF NAG HOMEBREW KA ZEANN eto po yung command
// g++ main.cpp -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -o main

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Constants The Laws
const float GRAVITY = 1000.f;
const float JUMP_FORCE = -500.f;
const float MOVE_SPEED = 300.f;
const sf::Vector2f BLOCK_SIZE(50.f, 50.f);

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(60);

    // Font Setter 
    sf::Font font;
    if (!font.openFromFile("../arial.ttf")) {
        std::cerr << "Could not load font\n";
        return 1;
    }

    sf::Text title(font, "Project T");
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(20.f, 20.f));

    // Player
    sf::RectangleShape player(BLOCK_SIZE);
    player.setFillColor(sf::Color::Green);
    player.setPosition(sf::Vector2f(100.f, 100.f));

    // Platforms
    std::vector<sf::RectangleShape> platforms;

    sf::RectangleShape ground(sf::Vector2f(static_cast<float>(windowSize.x), 50.f));
    ground.setPosition(sf::Vector2f(0.f, static_cast<float>(windowSize.y - 50)));
    ground.setFillColor(sf::Color(100, 100, 255));
    platforms.push_back(ground);

    sf::RectangleShape floatingPlatform(sf::Vector2f(300.f, 20.f));
    floatingPlatform.setPosition(sf::Vector2f(400.f, 500.f));
    floatingPlatform.setFillColor(sf::Color(100, 100, 255));
    platforms.push_back(floatingPlatform);

    sf::Clock clock;
    sf::Vector2f velocity(0.f, 0.f);
    bool canJump = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // SFML 3 event polling style
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) // More idiomatic SFML 3 event type checking
                window.close();
        }

        // Input
        // SFML 3: Use sf::Keyboard::Key enum
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            velocity.x = -MOVE_SPEED;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            velocity.x = MOVE_SPEED;
        else
            velocity.x = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && canJump) {
            velocity.y = JUMP_FORCE;
            canJump = false;
        }

        // Gravity
        velocity.y += GRAVITY * dt;

        // Move
        player.move(velocity * dt);

        // Collision
        canJump = false;
        /* Define playerBounds once here for the loop and subsequent clamping 
        // in construction trying o get the syntqax right please advise to return the comments after checking
        sf::FloatRect playerBounds = player.getGlobalBounds();

        for (const auto& platform : platforms) {
            sf::FloatRect platformBounds = platform.getGlobalBounds();

            if (playerBounds.intersects(platformBounds)) {
                float overlapY = (playerBounds.getTop() + playerBounds.getHeight()) - platformBounds.getTop();

                if (velocity.y > 0 && overlapY > 0 && overlapY < 20.f) {
                    player.setPosition(player.getPosition().x, platformBounds.getTop() - playerBounds.getHeight());
                    velocity.y = 0;
                    canJump = true;
                }
            }
        }

        // Clamp to screen
        sf::Vector2f pos = player.getPosition();
        if (pos.x < 0.f) pos.x = 0.f;
        if (pos.x + playerBounds.getWidth() > static_cast<float>(windowSize.x)) {
            pos.x = static_cast<float>(windowSize.x) - playerBounds.getWidth();
        }
        
        if (pos.y + playerBounds.getHeight() > static_cast<float>(windowSize.y)) {
            pos.y = static_cast<float>(windowSize.y) - playerBounds.getHeight();
            if (velocity.y > 0) {
                velocity.y = 0;
                canJump = true;
            }
        }
        player.setPosition(pos);
        */
        // window display update/ draw
        window.clear(sf::Color(30, 30, 30));
        window.draw(title);
        for (const auto& p : platforms) window.draw(p);
        window.draw(player);
        window.display();
    }

    return 0;
}