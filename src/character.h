#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class Character {
public:
    Character(float x, float y);

    bool setTexture(const sf::Texture& texture);
    bool loadAnimation(const std::string& leftFolder, const std::string& rightFolder);
    void update(float deltaTime);
    void setDirection(float xAxis, float yAxis);
    sf::Sprite& getSprite();
    void setWorldBounds(float width, float height);
    void startAttack();
    void takeDamage(int amount = 1);
    bool isDead() const;
    int getHealth() const;

private:
    std::unique_ptr<sf::Sprite> sprite_;
    sf::Vector2f velocity_;
    float speed_;
    float positionX_;
    float positionY_;
    float fallVelocity_;
    float gravity_;
    float jumpImpulse_;
    float groundLevel_;
    float worldWidth_;
    float worldHeight_;
    std::vector<sf::Texture> walkLeftFrames_;
    std::vector<sf::Texture> walkRightFrames_;
    std::vector<sf::Texture> idleLeftFrames_;
    std::vector<sf::Texture> idleRightFrames_;
    std::vector<sf::Texture> jumpLeftFrames_;
    std::vector<sf::Texture> jumpRightFrames_;
    std::vector<sf::Texture> hurtLeftFrames_;
    std::vector<sf::Texture> hurtRightFrames_;
    std::vector<sf::Texture> deathLeftFrames_;
    std::vector<sf::Texture> deathRightFrames_;
    std::vector<sf::Texture> attackFirstLeftFrames_;
    std::vector<sf::Texture> attackFirstRightFrames_;
    std::vector<sf::Texture> attackLastLeftFrames_;
    std::vector<sf::Texture> attackLastRightFrames_;
    bool hasAnimation_;
    int currentFrameIndex_;
    float animationTimer_;
    float attackTimer_;
    float hurtTimer_;
    float deathTimer_;
    int health_;
    int maxHealth_;
    int attackSequence_;
    bool isAttacking_;
    bool isHurt_;
    bool isDead_;
    bool isJumping_;
    bool isGrounded_;
    bool jumpRequested_;
    enum class FacingDirection {
        Left,
        Right
    } facingDirection_;
};