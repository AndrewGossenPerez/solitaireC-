// Spritesheet.h, created by Andrew Gossen.
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
    sf::Texture getUndo() const { return _undo; }  // Get the texture 
    sf::Texture getNewDeal() const { return _newDeal; }
    int cardWidth() const { return _cardWidth; }
    int cardHeight() const { return _cardHeight; }

    private:

    sf::Clock _backClock;  // For stockcard animation cooldown
    sf::Time _backCardDelay = sf::milliseconds(1000); // Card changes every second 
    sf::Texture _texture;
    sf::Texture _undo;
    sf::Texture _newDeal;
    int _cardWidth;
    int _cardHeight;
    int suitRow(Suit s) const;

};