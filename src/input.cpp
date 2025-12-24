//input.cpp, created by Andrew Gossen.
// Handles all user-input actions, i.e. button clicking, card dragging, card releasing.

#include "Game.h"
#include "Input.h"
#include "Card.h"
#include "Spritesheet.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
#include <Move.h>
#include <iostream>

bool mouseWasDown=false;

void Input::getHovered(sf::RenderWindow& window) {

    // window -- The solitaire window

    float cardWidth  = static_cast<float>(sheet.cardWidth());
    float cardHeight = static_cast<float>(sheet.cardHeight());

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // ---- Check if a dragged card has been 'dropped' somewhere

    if (!mouseDown && mouseWasDown && graphics.draggedCard!=nullptr) { // Mouse released

        const Card& draggedCardObj=*graphics.draggedCard;
        graphics.draggedCard = nullptr;
        Location startingLocation=draggedCardObj.getLocation();
        int startingPile=-1;

        // Establish starting pile, ( Pile where the card originally was before we attempted to apply a move )
        if (startingLocation==Location::Tableau){
            startingPile=draggedCardObj.getTableauPile();
        } else if (startingLocation==Location::Foundation){
            startingPile=draggedCardObj.getFoundationPile();
        }
        
        //  --- Check if they've dragged a card onto the foundation pile 
        float xStart = graphics.stockpileXOffset + 3.0f * graphics.pileSpacing;
        for (int i=0;i<4;i++){
            float x = xStart+i*graphics.pileSpacing;
            sf::FloatRect stockRect( // Establish hitbox
                {x,graphics.foundationYOffset},  
                {cardWidth,                    
                cardHeight}                   
            );
            if (stockRect.contains(mouse)){ // They've released the dragged card onto a foundation pile 

                Move move( // Create a move from the card's original location to the foundation, specifically at pile i
                   draggedCardObj,
                   startingLocation,
                   Location::Foundation,
                   i,
                   startingPile
                ); 
                game.applyMove(move,false); // Apply this move 
            }
        }

        //  --- Check if they've dragged a card onto the Tableau
        for (int p=0;p<7;p++){ // Iterate through each pile 
        
            sf::FloatRect stockRect(     // Set up a hitbox spanning entire y-axis of the screen
            {graphics.stockpileXOffset+(graphics.pileSpacing*p),
            graphics.tableauYOffset},  
            {cardWidth,                    
            cardHeight*15}                   
            );

            if (stockRect.contains(mouse)){ // Mouse is within bounds of the hitbox 

                // This implies they've dragged their card onto a Tableau pile

                Move move( // Set up the move from the card's original location to the Tableau
                   draggedCardObj,
                   startingLocation,
                   Location::Tableau,
                   p,
                   startingPile
                );
                game.applyMove(move,false); // Apply this move 
            }

        }


    } 

    //  ---- Click detection for buttons 
    if (mouseDown && !mouseWasDown && graphics.draggedCard==nullptr) { // Click detected, ensure we have a valid ptr to avoid seg fault
        
        if (dealClock.getElapsedTime() >= dealCooldown) { // Debouncer to avoid excessive calling

            sf::FloatRect stockRect( // Establish hitbox for the stockpile/reset button
                {graphics.stockpileXOffset,  
                graphics.foundationYOffset},  
                {cardWidth,                    
                cardHeight}                   
            );
            
            sf::FloatRect undoRect( // Establish hitbox for the undo button
                {graphics.undoXOffset,  
                graphics.undoYOffset},  
                {static_cast<float>(sheet.getUndo().getSize().x),
                static_cast<float>(sheet.getUndo().getSize().y)}
            );

            sf::FloatRect newDealRect( // Establish hitbox for the new deal button
                {graphics.newDealXOffset,  
                graphics.newDealYOffset},  
                {static_cast<float>(sheet.getNewDeal().getSize().x),
                static_cast<float>(sheet.getNewDeal().getSize().y)}
            );

            if (stockRect.contains(mouse)) {
                if (!game.getReserve().empty()) { // Player wants to deal
                    game.dealFromReserve();  
                } else { // Player wants to reset the reserve, i.e. bring all stockpile cards back to reset
                    game.resetStockpile();
                }
                dealClock.restart();
            } else if (undoRect.contains(mouse)){ // Player would like to undo a move
                game.undo();
            } else if (newDealRect.contains(mouse)){ // Player would like a new deal
                game.dealNewGame();
            }

        }
    } 

    //  ----  Drag detection for cards 
    if (mouseDown && mouseWasDown && graphics.draggedCard==nullptr){ // Mouse was down this frame and last 
       
        sf::FloatRect cardRect( // Establish hitbox for card in the dealing area 
            {graphics.stockpileXOffset+graphics.pileSpacing,  
            graphics.foundationYOffset},  
            {cardWidth,                    
            cardHeight}                   
        );

        if (cardRect.contains(mouse) && !game.getStockpile().empty()) {  // Users mouse is within the dealing area 
            graphics.draggedCard=&game.getStockpile().back(); // Set the topmost stockpile card ( That is, the visible dealt card ) as the dragged card 
        }

        //  --- Check each Foundation pile 
        float xStart = graphics.stockpileXOffset + 3.0f * graphics.pileSpacing;
        for (int i=0;i<4;i++){
            float x = xStart+i*graphics.pileSpacing;
            sf::FloatRect cardRect( // Establish hitbox for foundation pile 
                {x,graphics.foundationYOffset},  
                {cardWidth,                    
                cardHeight}                   
            );
            if (cardRect.contains(mouse) && !game.getFoundation(i).empty()){ // User's mouse is within a foundation pile 
                graphics.draggedCard=&game.getFoundation(i).back(); // Set currently drag card to the top-most foundation pile card 
            }
        }

        // --- Check Tableau
        for (int p=0;p<7;p++){ // Iterate through each pile 
            std::vector<Card> cards=game.getTableau(p);
            if (!cards.empty()){  // If the pile is not empty 
                int pileSize=static_cast<int>(cards.size());
                for (int c=0;c<pileSize;c++){  // Establish hitbox for each card in the pile

                    sf::FloatRect cardRect; // Acts as the hitbox 

                    if (c==pileSize-1){ // Final card  of the pile, so full hitbox
                        cardRect= sf::FloatRect( // Establish hitbox
                        {graphics.stockpileXOffset+(graphics.pileSpacing*p),
                        graphics.tableauYOffset+(c*graphics.tableauYSpacing)},  
                        {cardWidth,                    
                        cardHeight}                   
                        );
                    } else { // Not final card, so partial hitbox as it's 'peeking' out
                        cardRect= sf::FloatRect( // Establish hitbox
                        {graphics.stockpileXOffset+(graphics.pileSpacing*p),
                        graphics.tableauYOffset + c*graphics.tableauYSpacing},  
                        {cardWidth,                    
                        graphics.tableauYSpacing}                   
                        );
                    }

                    if (cardRect.contains(mouse)){ // Player's mouse is within the bounds of a Tableau card
                        graphics.draggedCard=&game.getTableau(p)[c]; // Set this tableau card as the currently dragged card 
                    }

                }
            }
        }
    }

    mouseWasDown = mouseDown; // Remember the state for the next frame 

}
