#include <SFML/Graphics.hpp>
#include "src/character.h"
#include "src/texture_manager.h"
#include "src/background.h"
#include "src/map_manager.h"
#include <iostream>

int main() {
    const unsigned int windowWidth = 1280;
    const unsigned int windowHeight = 720;
    const std::string backgroundPath = "D:\\Game_C++\\assets\\backgrounds\\BG_03.png";

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "RPG Adventure");
    window.setFramerateLimit(60);

    Character player(300.f, 500.f);
    player.setWorldBounds(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
    if (!player.loadAnimation("sprites/players/walk/left", "sprites/players/walk/right")) {
        TextureManager textures;
        if (!textures.load("player", "assets/player.png")) {
            return -1;
        }
        if (!player.setTexture(textures.get("player"))) {
            return -1;
        }
    }

    loadTilesets(); // Load tileset một lần duy nhất trước vòng lặp

    sf::Clock clock;
    bool attackPressed = false;
    bool hurtPressed = false;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        const float deltaTime = clock.restart().asSeconds();

        sf::Vector2f direction(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
            direction.x -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
            direction.x += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
            direction.y -= 1.f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
            if (!attackPressed) {
                player.startAttack();
            }
            attackPressed = true;
        } else {
            attackPressed = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::H)) {
            if (!hurtPressed) {
                player.takeDamage(1);
            }
            hurtPressed = true;
        } else {
            hurtPressed = false;
        }

        player.setDirection(direction.x, direction.y);
        player.update(deltaTime);

        sf::Sprite& sprite = player.getSprite();
        const sf::Vector2f position = sprite.getPosition();
        const sf::FloatRect bounds = sprite.getGlobalBounds();
        const sf::Vector2f size(bounds.size.x, bounds.size.y);

        if (position.x - size.x * 0.5f < 0.f) {
            sprite.setPosition(sf::Vector2f(size.x * 0.5f, position.y));
        } else if (position.x + size.x * 0.5f > static_cast<float>(windowWidth)) {
            sprite.setPosition(sf::Vector2f(static_cast<float>(windowWidth) - size.x * 0.5f, position.y));
        }

        if (position.y - size.y * 0.5f < 0.f) {
            sprite.setPosition(sf::Vector2f(position.x, size.y * 0.5f));
        } else if (position.y + size.y * 0.5f > static_cast<float>(windowHeight)) {
            sprite.setPosition(sf::Vector2f(position.x, static_cast<float>(windowHeight) - size.y * 0.5f));
        }

        window.clear(sf::Color::Black);
        // drawBackground(window, backgroundPath);
        drawMap(window);
        window.draw(sprite);
        window.display();
    }

    return 0;
}