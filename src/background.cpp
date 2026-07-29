#include "background.h"
#include <iostream>
#include <SFML/Graphics.hpp>

void drawBackground(sf::RenderWindow& window, const std::string& backgroundPath) {
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(backgroundPath)) {
        std::cerr << "Không thể load ảnh background: " << backgroundPath << std::endl;
        return;
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setTexture(backgroundTexture);

    // Lấy kích thước ảnh gốc
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // Tính hệ số scale theo chiều ngang và dọc
    float scaleX = static_cast<float>(window.getSize().x) / textureSize.x;
    float scaleY = static_cast<float>(window.getSize().y) / textureSize.y;

    // Áp dụng scale
    backgroundSprite.setScale(sf::Vector2f(scaleX, scaleY));

    // Vẽ ảnh nền
    window.draw(backgroundSprite);
}