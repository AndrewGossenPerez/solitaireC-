// Graphics.h, created by Andrew Gossen.

#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Move.h"
#include "Card.h"
#include "Spritesheet.h"


class SolitaireGraphics {

public:

    SolitaireGraphics(Spritesheet& sheet, sf::Font& font,Game& gameInstance)
    : _sheet(sheet), _font(font), game(gameInstance) {};

    void draw(sf::RenderWindow& window,
              const Game& game,
              bool showWinText) const;
    
    // UI Config
    const float pileSpacing       = 120.f; // Card spacing between Tableau piles and foundation piles 
    const float tableauYSpacing  = 20.f; // Spacing between cards on the Tableau pile 
    const float stockpileXOffset  = 100.f;
    const float dealXOffset  = 100.f + pileSpacing;
    const float tableauYOffset    = 180.f;
    const float tableauXOffset    = 100.f;
    const float foundationYOffset = 60.f;
    const float mouseXOffset=-30.0f;
    const float mouseYOffset=-30.0f;
    const float undoXOffset=500.0f;
    const float undoYOffset=600.0f;
    const float newDealXOffset=210.0f;
    const float newDealYOffset=600.0f;

    const Card* draggedCard=nullptr;
    const Card* lastDragged=nullptr;
    sf::Vector2f lastPosition;

private:

    Game& game;
    Spritesheet& _sheet;
    sf::Texture undo;
    sf::Font& _font;

    void drawDealtCard(sf::RenderWindow&, const Game&) const;
    void drawFoundations(sf::RenderWindow&, const Game&) const;
    void drawTableau(sf::RenderWindow&, const Game&) const;
    void drawStockpile(sf::RenderWindow&, const Game&) const;
    void drawDragging(sf::RenderWindow&, const Game&) const;
    void drawUndo(sf::RenderWindow&) const;
    void drawNewDeal(sf::RenderWindow&) const;

};
