// Graphics.h, created by Andrew Gossen.
// Defines the SolitaireGraphics class, which holds all essentials required for rendering the game 

#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Move.h"
#include "Card.h"
#include "Spritesheet.h"

class SolitaireGraphics {

public:

    SolitaireGraphics(Spritesheet& sheet, sf::Font& font,Game& gameInstance)
    : sheet(sheet), font(font), game(gameInstance) {};

    void draw(sf::RenderWindow& window, const Game& game, bool showWinText) const; // Renders the entire game 
    
    // UI Config
    const float pileSpacing       = 120.f; // Card spacing between Tableau piles and foundation piles 
    const float tableauYSpacing  = 20.f; // Spacing between cards on the Tableau pile 
    const float stockpileXOffset  = 100.f; // How many pixels right to the screen the stockpile is 
    const float dealXOffset  = 100.f + pileSpacing; // How many pixels right to the screen the deal area is 
    const float tableauYOffset    = 180.f; // How many pixels down the Tableau starts 
    const float tableauXOffset    = 100.f; // How many pixels right the Tableau starts 
    const float foundationYOffset = 60.f; // How many pixels down the foundation starts, the X offset is based on the stockpile offset 
    const float mouseXOffset=-30.0f; // Where dragged cards are positioned relative to the mouse 
    const float mouseYOffset=-30.0f; // Where dragged cards are positioned relative to the mouse 
    const float undoXOffset=500.0f; // How many pixels to the right the Undo button is 
    const float undoYOffset=600.0f; // How many pixels down the Undo button is 
    const float newDealXOffset=210.0f; // How many pixels to the right the new deal button is 
    const float newDealYOffset=600.0f; // How many pixels down the new deal button is 

    const Card* draggedCard=nullptr; // Points to any card being dragged 

private:

    Spritesheet& sheet;
    sf::Font& font;
    Game& game; 

    sf::Texture undo;

    void drawDealtCard(sf::RenderWindow&, const Game&) const;
    void drawFoundations(sf::RenderWindow&, const Game&) const;
    void drawTableau(sf::RenderWindow&) const;
    void drawStockpile(sf::RenderWindow&, const Game&) const;
    void drawDragging(sf::RenderWindow&) const;
    void drawUndo(sf::RenderWindow&) const;
    void drawNewDeal(sf::RenderWindow&) const;

};
