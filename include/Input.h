// Input.h, created by Andrew Gossen.

#include "Game.h"
#include "Card.h"
#include "Spritesheet.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
#pragma once 

class Input{

    public:

    Input(Game& gameInstance,SolitaireGraphics& graphics,Spritesheet& sheet) 
    : game(gameInstance), _graphics(graphics), _sheet(sheet) {};

    void getHovered(sf::RenderWindow& window); // Sets the hovered card data

    private:

    sf::Clock _dealClock;  // for cooldown
    sf::Time _dealCooldown = sf::milliseconds(100); // 0.1s between deals
    sf::Vector2f lastPosition;

    Game& game;
    SolitaireGraphics& _graphics;
    Spritesheet& _sheet;


};