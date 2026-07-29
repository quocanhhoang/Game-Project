#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager {
public:
    bool load(const std::string& id, const std::string& filename);
    const sf::Texture& get(const std::string& id) const;
    bool has(const std::string& id) const;
    void clear();

private:
    std::map<std::string, sf::Texture> textures_;
};
