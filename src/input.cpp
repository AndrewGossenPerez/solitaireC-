#include "Game.h"
#include "Input.h"
#include "Card.h"
#include "Spritesheet.h"
#include "Graphics.h"
#include <SFML/Graphics.hpp>
#include <Move.h>
#include <iostream>

bool _mouseWasDown=false;


void Input::getHovered(sf::RenderWindow& window) {

    float cardWidth  = static_cast<float>(_sheet.cardWidth());
    float cardHeight = static_cast<float>(_sheet.cardHeight());

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // Card dragged and dropped somewhere detection
    if (!mouseDown && _mouseWasDown && _graphics.draggedCard!=nullptr) { // Mouse released

        const Card& draggedCardObj=*_graphics.draggedCard;
        _graphics.draggedCard = nullptr;
        Location startingLocation=draggedCardObj.getLocation();
        int startingPile=-1;
        // Establish starting pile 

        if (startingLocation==Location::Tableau){
            startingPile=draggedCardObj.getTableauPile();
        } else if (startingLocation==Location::Foundation){
            startingPile=draggedCardObj.getFoundationPile();
        }
        
        // Check now if they've dragged this card onto a foundation pile or the Tableau

        // Foundation Check
        float xStart = _graphics.stockpileXOffset + 3.0f * _graphics.pileSpacing;
        for (int i=0;i<4;i++){
            float x = xStart+i*_graphics.pileSpacing;
            sf::FloatRect stockRect( // Establish hitbox
                {x,_graphics.foundationYOffset},  
                {cardWidth,                    
                cardHeight}                   
            );
            if (stockRect.contains(mouse)){
                // They've dragged onto this area, create a move and apply it
                std::cout << "Foundation move" << std::endl;
                Move move(
                   draggedCardObj,
                   startingLocation,
                   Location::Foundation,
                   i,
                   startingPile
                );
                game.applyMove(move,false);
            }
        }

        // Tableau Check 
        for (int p=0;p<7;p++){ // Iterate through each pile 
            // Set up a hitbox spanning entire screen 
            sf::FloatRect stockRect( // Establish hitbox
            {_graphics.stockpileXOffset+(_graphics.pileSpacing*p),
            _graphics.tableauYOffset},  
            {cardWidth,                    
            cardHeight*15}                   
            );
            if (stockRect.contains(mouse)){
                // They've dragged onto this area, create a move and apply it
                std::cout << "Tableau move" << std::endl;
                Move move(
                   draggedCardObj,
                   startingLocation,
                   Location::Tableau,
                   p,
                   startingPile
                );
                game.applyMove(move,false);
            }

        }


    } 

    // Click detection for the deal card / reset deck 
    if (mouseDown && !_mouseWasDown && _graphics.draggedCard==nullptr) { // Click detected, ensure we have a valid ptr to avoid seg fault
        
        if (_dealClock.getElapsedTime() >= _dealCooldown) { // Deal cooldown passed,

            sf::FloatRect stockRect( // Establish hitbox
                {_graphics.stockpileXOffset,  
                _graphics.foundationYOffset},  
                {cardWidth,                    
                cardHeight}                   
            );
            
            sf::FloatRect undoRect( // Establish hitbox
                {_graphics.undoXOffset,  
                _graphics.undoYOffset},  
                {static_cast<float>(_sheet.getUndo().getSize().x),
                static_cast<float>(_sheet.getUndo().getSize().y)}
            );

            sf::FloatRect newDealRect( // Establish hitbox
                {_graphics.newDealXOffset,  
                _graphics.newDealYOffset},  
                {static_cast<float>(_sheet.getNewDeal().getSize().x),
                static_cast<float>(_sheet.getNewDeal().getSize().y)}
            );

            if (stockRect.contains(mouse)) {
                if (!game.getReserve().empty()) {
                    // Player wants to deal
                    game.dealFromReserve();  
                } else {
                    // Player wants to reset the reserve, i.e. bring all stockpile cards back to reset
                    game.resetStockpile();
                }
                _dealClock.restart();
            } else if (undoRect.contains(mouse)){
                // Player would like to undo
                game.undo();
            } else if (newDealRect.contains(mouse)){
                // Player would like a new deal
                game.dealNewGame();
            }

        }
    } 

    // Drag detection
    if (mouseDown && _mouseWasDown && _graphics.draggedCard==nullptr){ // Mouse was down this frame and last 
       
        // Check deal area to see if the player is initiating a drag
        sf::FloatRect cardRect( // Establish hitbox
            {_graphics.stockpileXOffset+_graphics.pileSpacing,  
            _graphics.foundationYOffset},  
            {cardWidth,                    
            cardHeight}                   
        );

        if (cardRect.contains(mouse) && !game.getStockpile().empty()) {
            // Initiate the drag on top-most (last index) stockpile card
            _graphics.draggedCard=&game.getStockpile().back(); // Set currently drag card to the top-most stockpile card 
        }

        // Check each Foundation pile 
        float xStart = _graphics.stockpileXOffset + 3.0f * _graphics.pileSpacing;
        for (int i=0;i<4;i++){
            float x = xStart+i*_graphics.pileSpacing;
            sf::FloatRect cardRect( // Establish hitbox
                {x,_graphics.foundationYOffset},  
                {cardWidth,                    
                cardHeight}                   
            );
            if (cardRect.contains(mouse) && !game.getFoundation(i).empty()){
                _graphics.draggedCard=&game.getFoundation(i).back(); // Set currently drag card to the top-most foundation pile card 
            }
        }

        // Check Tableau
        for (int p=0;p<7;p++){ // Iterate through each pile 
            std::vector<Card> cards=game.getTableau(p);
            if (!cards.empty()){ 
                for (int c=0;c<cards.size();c++){
                    // Establish hitbox for each card 
                    sf::FloatRect cardRect;

                    if (c==cards.size()-1){ // Final card so full hitbox
                        cardRect= sf::FloatRect( // Establish hitbox
                        {_graphics.stockpileXOffset+(_graphics.pileSpacing*p),
                        _graphics.tableauYOffset+(c*_graphics.tableauYSpacing)},  
                        {cardWidth,                    
                        cardHeight}                   
                        );
                    } else { // Not final card so partial hitbox 
                        cardRect= sf::FloatRect( // Establish hitbox
                        {_graphics.stockpileXOffset+(_graphics.pileSpacing*p),
                        _graphics.tableauYOffset + c*_graphics.tableauYSpacing},  
                        {cardWidth,                    
                        _graphics.tableauYSpacing}                   
                        );
                    }
                    if (cardRect.contains(mouse)){
                        _graphics.draggedCard=&game.getTableau(p)[c]; // Set currently drag card to this Tableau card
                    }

                }
            }
        }
    }

    // remember current state for next frame
    _mouseWasDown = mouseDown;

}
