// Spritesheet.h, created by Andrew Gossen.
#pragma once 
#include <SFML/Graphics.hpp>
#include "Card.h"

class Spritesheet{ 

    public:

    bool loadFromFile(const std::string& filename);
    bool loadUndo(const std::string& filename);

    // Getters
    sf::Sprite makeCardSprite(const Card& card) const;
    sf::Sprite makeBackSprite() const;
    sf::Sprite makeResetSprite() const;
    sf::Sprite getCardSprite(int col, int row) const;
    sf::Texture getUndo() const { return undo; }  // Get the texture 

    int cardWidth() const { return _cardWidth; }
    int cardHeight() const { return _cardHeight; }

    // Setters 

    private:

    sf::Texture _texture;
    sf::Texture undo;
    int _cardWidth;
    int _cardHeight;
    int suitRow(Suit s) const;

};