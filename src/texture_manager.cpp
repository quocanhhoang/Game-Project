#include "texture_manager.h"

#include <stdexcept>

bool TextureManager::load(const std::string& id, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        return false;
    }

    textures_[id] = std::move(texture);
    return true;
}

const sf::Texture& TextureManager::get(const std::string& id) const {
    auto it = textures_.find(id);
    if (it == textures_.end()) {
        throw std::runtime_error("Texture not found: " + id);
    }
    return it->second;
}

bool TextureManager::has(const std::string& id) const {
    return textures_.find(id) != textures_.end();
}

void TextureManager::clear() {
    textures_.clear();
}
