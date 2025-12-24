// Spritesheet.cpp created by Andrew Gossen.
// Handles the spritesheet, and sprite textures

#include "Spritesheet.h"
#include "Graphics.h"

// -- Loads the Spritesheet 
bool Spritesheet::loadFromFile(const std::string& path) {

    // path --The file path for the texture 

    if (!texture.loadFromFile(path)) return false;

    // Initialise card width and height, note the sprite sheet is 13 cols by 6 rows
    sf::Vector2u texSize = texture.getSize();
    _cardWidth  = static_cast<int>(texSize.x / 13);
    _cardHeight = static_cast<int>(texSize.y / 6);
    return true;

}

// -- Loads the Undo button texture 
bool Spritesheet::loadUndo(const std::string& path){

    // path - The file path for the texture 
    if (!undo.loadFromFile(path)) return false;
    return true;
}

// -- Loads the new deal button Texture  
bool Spritesheet::loadNewDeal(const std::string& path){

    // path - The file path for the texture 
    if (!newDeal.loadFromFile(path)) return false;
    return true;
}

// -- Returns a sprite for a card on column col and row row of the Spritesheet
sf::Sprite Spritesheet::getCardSprite(int col, int row) const {

    // col - The column we want to isolate 
    // row - The row we want to isolate 

    sf::IntRect rect( // Isolate card from spritesheet 
        {col * _cardWidth,
        row * _cardHeight},
       { _cardWidth,
        _cardHeight}
    );

    sf::Sprite sprite{texture, rect};
    return sprite;

}


// -- Back card animation 
int start=3; // Iterate from 3 to 5 to animate the back card
sf::Sprite Spritesheet::makeBackSprite() {
    if (backClock.getElapsedTime() >= backCardDelay){ // We've passed the delay 
        start++; // Increment the back card sprite 
        if (start>5){ 
            start=3;
        }
        backClock.restart(); // Reset the clock
    }
    return Spritesheet::getCardSprite(start,5); // Set the back card sprite 
}

// -- Creates the reset button sprite, which is on col 10 row 4
sf::Sprite Spritesheet::makeResetSprite() const {
    return Spritesheet::getCardSprite(10,4);
}

// -- Creates a card sprite for a Card object 
sf::Sprite Spritesheet::makeCardSprite(const Card& card) const {

    // card - The card object we want to create a sprite for 
    int col = static_cast<int>(card.getValue()); // Enums established to fit the Spritesheet 
    int row = static_cast<int>(card.getSuit());  

    sf::IntRect rect(
        {col * _cardWidth,
        row * _cardHeight},
       { _cardWidth,
        _cardHeight}
    );

    sf::Sprite sprite{texture, rect};
    return sprite;

}