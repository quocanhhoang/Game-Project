#include "character.h"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

Character::Character(float x, float y)
    : sprite_(nullptr), velocity_(0.f, 0.f), speed_(200.f), positionX_(x), positionY_(y), fallVelocity_(0.f), gravity_(900.f), jumpImpulse_(-350.f), groundLevel_(720.f - 80.f), worldWidth_(1280.f), worldHeight_(720.f), hasAnimation_(false), currentFrameIndex_(0), animationTimer_(0.f), attackTimer_(0.f), hurtTimer_(0.f), deathTimer_(0.f), health_(5), maxHealth_(5), attackSequence_(0), isAttacking_(false), isHurt_(false), isDead_(false), isJumping_(false), isGrounded_(true), jumpRequested_(false), facingDirection_(FacingDirection::Right) {
}

bool Character::setTexture(const sf::Texture& texture) {
    if (texture.getSize().x == 0 || texture.getSize().y == 0) {
        return false;
    }
    sprite_ = std::make_unique<sf::Sprite>(texture);
    sprite_->setOrigin(sf::Vector2f(texture.getSize()) * 0.5f);
    sprite_->setPosition(sf::Vector2f(positionX_, positionY_));
    return true;
}

bool Character::loadAnimation(const std::string& leftFolder, const std::string& rightFolder) {
    auto loadFramesFromFolder = [](const std::string& folder, std::vector<sf::Texture>& out, const std::string& filePrefix, int maxFrames = 9) {
        out.clear();
        for (int i = 1; i <= maxFrames; ++i) {
            std::ostringstream path;
            path << folder << "/" << filePrefix << "_" << (i < 10 ? "0" : "") << i << ".png";
            sf::Texture texture;
            if (!texture.loadFromFile(path.str())) {
                break;
            }
            out.push_back(std::move(texture));
        }
        return !out.empty();
    };

    std::vector<sf::Texture> leftWalkFrames;
    std::vector<sf::Texture> rightWalkFrames;
    std::vector<sf::Texture> leftIdleFrames;
    std::vector<sf::Texture> rightIdleFrames;
    std::vector<sf::Texture> leftJumpFrames;
    std::vector<sf::Texture> rightJumpFrames;
    std::vector<sf::Texture> leftHurtFrames;
    std::vector<sf::Texture> rightHurtFrames;
    std::vector<sf::Texture> leftDeathFrames;
    std::vector<sf::Texture> rightDeathFrames;
    std::vector<sf::Texture> leftFirstAttackFrames;
    std::vector<sf::Texture> rightFirstAttackFrames;
    std::vector<sf::Texture> leftLastAttackFrames;
    std::vector<sf::Texture> rightLastAttackFrames;

    const std::string walkToken = "/walk/";
    const std::string idleToken = "/idle/";
    const std::string jumpToken = "/jump/";
    const std::string hurtToken = "/hurt/";
    const std::string deathToken = "/death/";
    const std::string firstAttackToken = "/attack/firstAttack/";
    const std::string lastAttackToken = "/attack/lastAttack/";

    auto replaceFolderToken = [](std::string path, const std::string& fromToken, const std::string& toToken) {
        if (path.find(fromToken) != std::string::npos) {
            path.replace(path.find(fromToken), fromToken.size(), toToken);
        }
        return path;
    };

    const bool hasLeftWalk = loadFramesFromFolder(leftFolder, leftWalkFrames, "Walk");
    const bool hasRightWalk = loadFramesFromFolder(rightFolder, rightWalkFrames, "Walk");
    const bool hasLeftJump = loadFramesFromFolder(replaceFolderToken(leftFolder, walkToken, jumpToken), leftJumpFrames, "Jump");
    const bool hasRightJump = loadFramesFromFolder(replaceFolderToken(rightFolder, walkToken, jumpToken), rightJumpFrames, "Jump");
    const bool hasLeftHurt = loadFramesFromFolder(replaceFolderToken(leftFolder, walkToken, hurtToken), leftHurtFrames, "Hurt");
    const bool hasRightHurt = loadFramesFromFolder(replaceFolderToken(rightFolder, walkToken, hurtToken), rightHurtFrames, "Hurt");
    const bool hasLeftDeath = loadFramesFromFolder(replaceFolderToken(leftFolder, walkToken, deathToken), leftDeathFrames, "Death");
    const bool hasRightDeath = loadFramesFromFolder(replaceFolderToken(rightFolder, walkToken, deathToken), rightDeathFrames, "Death");

    const std::string leftAttackFirstFolder = replaceFolderToken(leftFolder, walkToken, firstAttackToken);
    const std::string rightAttackFirstFolder = replaceFolderToken(rightFolder, walkToken, firstAttackToken);
    const std::string leftAttackLastFolder = replaceFolderToken(leftFolder, walkToken, lastAttackToken);
    const std::string rightAttackLastFolder = replaceFolderToken(rightFolder, walkToken, lastAttackToken);

    const bool hasLeftAttackFirst = loadFramesFromFolder(leftAttackFirstFolder, leftFirstAttackFrames, "Attack");
    const bool hasRightAttackFirst = loadFramesFromFolder(rightAttackFirstFolder, rightFirstAttackFrames, "Attack");
    const bool hasLeftAttackLast = loadFramesFromFolder(leftAttackLastFolder, leftLastAttackFrames, "Attack");
    const bool hasRightAttackLast = loadFramesFromFolder(rightAttackLastFolder, rightLastAttackFrames, "Attack");

    if (!hasLeftWalk || !hasRightWalk) {
        return false;
    }

    std::string leftIdleFolder = replaceFolderToken(leftFolder, walkToken, idleToken);
    std::string rightIdleFolder = replaceFolderToken(rightFolder, walkToken, idleToken);

    const bool hasLeftIdle = loadFramesFromFolder(leftIdleFolder, leftIdleFrames, "Idle");
    const bool hasRightIdle = loadFramesFromFolder(rightIdleFolder, rightIdleFrames, "Idle");

    if (!hasLeftIdle || !hasRightIdle) {
        leftIdleFrames.clear();
        rightIdleFrames.clear();
    }

    walkLeftFrames_ = std::move(leftWalkFrames);
    walkRightFrames_ = std::move(rightWalkFrames);
    idleLeftFrames_ = std::move(leftIdleFrames);
    idleRightFrames_ = std::move(rightIdleFrames);
    jumpLeftFrames_ = std::move(leftJumpFrames);
    jumpRightFrames_ = std::move(rightJumpFrames);
    hurtLeftFrames_ = std::move(leftHurtFrames);
    hurtRightFrames_ = std::move(rightHurtFrames);
    deathLeftFrames_ = std::move(leftDeathFrames);
    deathRightFrames_ = std::move(rightDeathFrames);
    attackFirstLeftFrames_ = std::move(leftFirstAttackFrames);
    attackFirstRightFrames_ = std::move(rightFirstAttackFrames);
    attackLastLeftFrames_ = std::move(leftLastAttackFrames);
    attackLastRightFrames_ = std::move(rightLastAttackFrames);
    hasAnimation_ = true;
    currentFrameIndex_ = 0;
    animationTimer_ = 0.f;
    attackTimer_ = 0.f;
    hurtTimer_ = 0.f;
    deathTimer_ = 0.f;

    if (!sprite_) {
        sprite_ = std::make_unique<sf::Sprite>(walkRightFrames_.front());
        sprite_->setOrigin(sf::Vector2f(walkRightFrames_.front().getSize()) * 0.5f);
        sprite_->setPosition(sf::Vector2f(positionX_, positionY_));
    } else {
        sprite_->setTexture(walkRightFrames_.front());
        sprite_->setOrigin(sf::Vector2f(walkRightFrames_.front().getSize()) * 0.5f);
        sprite_->setPosition(sf::Vector2f(positionX_, positionY_));
    }

    return true;
}

void Character::update(float deltaTime) {
    if (!sprite_) {
        return;
    }

    if (isDead_) {
        const auto& frames = (facingDirection_ == FacingDirection::Left)
            ? deathLeftFrames_
            : deathRightFrames_;
        if (!frames.empty()) {
            deathTimer_ += deltaTime;
            const std::size_t frameIndex = static_cast<std::size_t>(deathTimer_ / 0.08f) % frames.size();
            sprite_->setTexture(frames[frameIndex]);
            sprite_->setOrigin(sf::Vector2f(frames[frameIndex].getSize()) * 0.5f);
        }
        return;
    }

    if (isHurt_) {
        const auto& frames = (facingDirection_ == FacingDirection::Left)
            ? hurtLeftFrames_
            : hurtRightFrames_;
        if (!frames.empty()) {
            hurtTimer_ += deltaTime;
            const std::size_t frameIndex = static_cast<std::size_t>(hurtTimer_ / 0.08f) % frames.size();
            sprite_->setTexture(frames[frameIndex]);
            sprite_->setOrigin(sf::Vector2f(frames[frameIndex].getSize()) * 0.5f);
            if (hurtTimer_ >= 0.35f) {
                isHurt_ = false;
                hurtTimer_ = 0.f;
                currentFrameIndex_ = 0;
            }
            return;
        }
        isHurt_ = false;
    }

    if (isAttacking_) {
        const auto& frames = (facingDirection_ == FacingDirection::Left)
            ? ((attackSequence_ % 2 == 1) ? attackFirstLeftFrames_ : attackLastLeftFrames_)
            : ((attackSequence_ % 2 == 1) ? attackFirstRightFrames_ : attackLastRightFrames_);
        if (!frames.empty()) {
            attackTimer_ += deltaTime;
            const std::size_t frameIndex = static_cast<std::size_t>(attackTimer_ / 0.08f) % frames.size();
            sprite_->setTexture(frames[frameIndex]);
            sprite_->setOrigin(sf::Vector2f(frames[frameIndex].getSize()) * 0.5f);
            if (attackTimer_ >= 0.35f) {
                isAttacking_ = false;
                attackTimer_ = 0.f;
                currentFrameIndex_ = 0;
            }
            return;
        }
        isAttacking_ = false;
    }

    if (jumpRequested_ && isGrounded_) {
        fallVelocity_ = jumpImpulse_;
        isGrounded_ = false;
        isJumping_ = true;
        jumpRequested_ = false;
    }

    sf::Vector2f movement(velocity_.x * speed_ * deltaTime, 0.f);
    movement.y += fallVelocity_ * deltaTime;
    sprite_->move(movement);

    const sf::FloatRect bounds = sprite_->getGlobalBounds();
    const float bottom = bounds.position.y + bounds.size.y;
    const float groundThreshold = groundLevel_ - 4.f;
    const bool isOnGround = bottom >= groundThreshold;

    if (isOnGround) {
        const float clampedY = groundLevel_ - bounds.size.y * 0.5f;
        sprite_->setPosition(sf::Vector2f(sprite_->getPosition().x, clampedY));
        fallVelocity_ = 0.f;
        if (!isGrounded_) {
            // Only reset animation on the frame we first land
            currentFrameIndex_ = 0;
            animationTimer_ = 0.f;
        }
        isJumping_ = false;
        isGrounded_ = true;
    } else {
        fallVelocity_ += gravity_ * deltaTime;
        isGrounded_ = false;
    }

    if (hasAnimation_ && !walkLeftFrames_.empty() && !walkRightFrames_.empty()) {
        const bool movingHorizontally = velocity_.x != 0.f;

        if (movingHorizontally) {
            const std::vector<sf::Texture>* activeFrames = nullptr;
            if (facingDirection_ == FacingDirection::Left) {
                activeFrames = &walkLeftFrames_;
            } else {
                activeFrames = &walkRightFrames_;
            }

            animationTimer_ += deltaTime;
            if (animationTimer_ >= 0.08f) {
                animationTimer_ = 0.f;
                currentFrameIndex_ = (currentFrameIndex_ + 1) % activeFrames->size();
            }
            sprite_->setTexture((*activeFrames)[currentFrameIndex_ % activeFrames->size()]);
            sprite_->setOrigin(sf::Vector2f((*activeFrames)[currentFrameIndex_ % activeFrames->size()].getSize()) * 0.5f);
        } else {
            const std::vector<sf::Texture>& idleSet = (facingDirection_ == FacingDirection::Left)
                ? idleLeftFrames_
                : idleRightFrames_;

            if (!idleSet.empty()) {
                animationTimer_ += deltaTime;
                if (animationTimer_ >= 0.12f) {
                    animationTimer_ = 0.f;
                    currentFrameIndex_ = (currentFrameIndex_ + 1) % idleSet.size();
                }
            } else {
                currentFrameIndex_ = 0;
            }

            const auto& idleFrame = idleSet.empty()
                ? (facingDirection_ == FacingDirection::Left ? walkLeftFrames_.front() : walkRightFrames_.front())
                : idleSet[currentFrameIndex_ % idleSet.size()];
            sprite_->setTexture(idleFrame);
            sprite_->setOrigin(sf::Vector2f(idleFrame.getSize()) * 0.5f);
        }
    }
}

void Character::setDirection(float xAxis, float yAxis) {
    velocity_.x = xAxis;
    velocity_.y = 0.f;

    if (xAxis != 0.f) {
        const float length = std::sqrt(xAxis * xAxis);
        velocity_.x = xAxis / length;
    }

    if (yAxis < 0.f && isGrounded_) {
        jumpRequested_ = true;
    }

    if (xAxis < 0.f) {
        facingDirection_ = FacingDirection::Left;
    } else if (xAxis > 0.f) {
        facingDirection_ = FacingDirection::Right;
    }
}

void Character::setWorldBounds(float width, float height) {
    worldWidth_ = width;
    worldHeight_ = height;
    groundLevel_ = worldHeight_ - 80.f;
    if (groundLevel_ < 0.f) {
        groundLevel_ = 0.f;
    }
}

void Character::startAttack() {
    if (isDead_ || isAttacking_) {
        return;
    }

    ++attackSequence_;
    isAttacking_ = true;
    isHurt_ = false;
    currentFrameIndex_ = 0;
    attackTimer_ = 0.f;
    hurtTimer_ = 0.f;
}

void Character::takeDamage(int amount) {
    if (isDead_) {
        return;
    }

    health_ -= amount;
    if (health_ <= 0) {
        health_ = 0;
        isDead_ = true;
        isAttacking_ = false;
        isHurt_ = false;
        currentFrameIndex_ = 0;
        deathTimer_ = 0.f;
        hurtTimer_ = 0.f;
        attackTimer_ = 0.f;
        return;
    }

    isHurt_ = true;
    isAttacking_ = false;
    currentFrameIndex_ = 0;
    hurtTimer_ = 0.f;
    attackTimer_ = 0.f;
}

bool Character::isDead() const {
    return isDead_;
}

int Character::getHealth() const {
    return health_;
}

sf::Sprite& Character::getSprite() {
    if (!sprite_) {
        throw std::runtime_error("Sprite not initialized");
    }
    return *sprite_;
}
