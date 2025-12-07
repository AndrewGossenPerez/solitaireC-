// Spritesheet.cpp created by Andrew Gossen.

#include "Spritesheet.h"
#include "Graphics.h"

bool Spritesheet::loadFromFile(const std::string& path) {

    if (!_texture.loadFromFile(path)) return false;

    // Initialise card width and height, note the sprite sheet is 13 cols by 6 rows
    sf::Vector2u texSize = _texture.getSize();
    _cardWidth  = static_cast<int>(texSize.x / 13);
    _cardHeight = static_cast<int>(texSize.y / 6);
    return true;

}

bool Spritesheet::loadUndo(const std::string& path){
    if (!undo.loadFromFile(path)) return false;
    return true;
}

sf::Sprite Spritesheet::getCardSprite(int col, int row) const {

    sf::IntRect rect(
        {col * _cardWidth,
        row * _cardHeight},
       { _cardWidth,
        _cardHeight}
    );

    sf::Sprite sprite{_texture, rect};
    return sprite;

}

sf::Sprite Spritesheet::makeBackSprite() const {
    return Spritesheet::getCardSprite(3,5);
}

sf::Sprite Spritesheet::makeResetSprite() const {
    return Spritesheet::getCardSprite(10,4);
}

sf::Sprite Spritesheet::makeCardSprite(const Card& card) const {

    int col = static_cast<int>(card.getValue());
    int row = static_cast<int>(card.getSuit());  

    sf::IntRect rect(
        {col * _cardWidth,
        row * _cardHeight},
       { _cardWidth,
        _cardHeight}
    );

    sf::Sprite sprite{_texture, rect};

    return sprite;

}