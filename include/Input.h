// Input.h, created by Andrew Gossen.
// Defines the Input class utilised for player-inputs, being card dragging, card releasing, and button clicking

#include "Game.h"
#include "Card.h"
#include "Spritesheet.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
#pragma once 

class Input{

public:

    // Class constructor 
    Input(Game& gameInstance,SolitaireGraphics& graphics,Spritesheet& sheet) 
    : game(gameInstance), graphics(graphics), sheet(sheet) {};

    void getHovered(sf::RenderWindow& window); // Sets the hovered card data

private:

    sf::Clock dealClock; // Used for debouncing buttons 
    sf::Time dealCooldown = sf::milliseconds(100); // 0.1s debounce delay for buttons 

    Game& game;
    SolitaireGraphics& graphics;
    Spritesheet& sheet;

};