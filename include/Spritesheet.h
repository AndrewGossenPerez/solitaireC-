// Spritesheet.h, created by Andrew Gossen.
// Defines the Spritesheet class used to store textures

#pragma once 
#include <SFML/Graphics.hpp>
#include "Card.h"

class Spritesheet{ 

public:

    // Asset Loaders
    bool loadFromFile(const std::string& filename);
    bool loadUndo(const std::string& filename);
    bool loadNewDeal(const std::string& filename);

    // Getters
    sf::Sprite makeCardSprite(const Card& card) const;
    sf::Sprite makeBackSprite();
    sf::Sprite makeResetSprite() const;
    sf::Sprite getCardSprite(int col, int row) const;
    sf::Texture getUndo() const { return undo; } 
    sf::Texture getNewDeal() const { return newDeal; }
    int cardWidth() const { return _cardWidth; }
    int cardHeight() const { return _cardHeight; }

private:

    sf::Clock backClock;  // For the deal card animation 
    sf::Time backCardDelay = sf::milliseconds(1000); // Card changes every second 
    sf::Texture texture;
    sf::Texture undo;
    sf::Texture newDeal;
    int _cardWidth;
    int _cardHeight;
    int suitRow(Suit s) const;

};