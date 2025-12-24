//graphics.cpp, Created by Andrew Gossen.
//Handles rendering of the Solitaire game

#include "Graphics.h"
#include "Game.h"
#include "Input.h"
#include <iostream>

//    --- Stockpile rendering
void SolitaireGraphics::drawStockpile(sf::RenderWindow& window,const Game& game) const {
                         
    // window -- The Solitaire window object 
    // game -- The solitaire game instance storing all game data
    
    if (game.getReserve().empty()){ // All cards have been dealt, so show the reset card
        auto sprite = sheet.makeResetSprite(); 
        sprite.setPosition({ stockpileXOffset, foundationYOffset });
        window.draw(sprite);
    } else { // Not all card's have been dealt, so show the back card
        auto sprite = sheet.makeBackSprite(); 
        sprite.setPosition({ stockpileXOffset, foundationYOffset });
        window.draw(sprite);
    }

    // Render the topmost card in the stockpile/dealing area, excluding dragged ones ( Handled in seperate function drawDragged)
    if (!game.getStockpile().empty()) {
        const Card& c = game.getStockpile().back();
        if (draggedCard!=nullptr){
            if (draggedCard==&c) return; // This card is being dragged, hence we don't want to render it 
        }
        auto sprite = sheet.makeCardSprite(c);
        sprite.setPosition({ stockpileXOffset+pileSpacing, foundationYOffset });
        window.draw(sprite);
    } 
}

//    --- Foundation pile rendering
void SolitaireGraphics::drawFoundations(sf::RenderWindow& window,const Game& game) const {

    // window - The Solitaire window object 
    // game - The solitaire game instance storing all game data

    float cardWidth = static_cast<float>(sheet.cardWidth());
    float cardHeight= static_cast<float>(sheet.cardHeight());

    float xStart = stockpileXOffset + 3.0f * pileSpacing;
    float y = foundationYOffset;

    for (int i = 0; i < 4; ++i) { // Iterate through the foundation piles 
        float x = xStart+i*pileSpacing;
        if (game.getFoundation(i).empty()) { // The foundation pile is empty
            sf::RectangleShape rect;
            rect.setSize({ cardWidth, cardHeight });
            rect.setPosition({ x, y });
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color(200, 200, 200));
            rect.setOutlineThickness(2.f);
            window.draw(rect); // If the foundation pile is empty draw the default rectangle to indicate an empty pile
        } else { // The foundation pile isn't empty, so render the top-most foundation card, or if the current card is being dragged show the card behind it
            
            int index=1;
            const Card& card=game.getFoundation(i).back();
           
            if (&card==draggedCard) {
                index=2; // Get the card behind the dragged card 
            }

            int pileSize=static_cast<int>(game.getFoundation(i).size());

            if (index<=pileSize){ // Ensure that the index is appropriate such that we avoid a heap issue 
                const Card& c=game.getFoundation(i)[game.getFoundation(i).size()-index];
                auto sprite = sheet.makeCardSprite(c);
                sprite.setPosition({ x, y });
                window.draw(sprite);  // Draw out foundation card 
            }

        }
    }

}

//    --- Tableau rendering 
void SolitaireGraphics::drawTableau(sf::RenderWindow& window) const {

    // window - The Solitaire window object 
    // game - The solitaire game instance storing all game data

    for (int i=0;i<7;i++){ // Iterate through each Tableau pile 
        std::vector<Card> cards=game.getTableau(i);
        int pileSize=static_cast<int>(cards.size());
        for (int k=0;k<pileSize;k++){ // Iterate through each card 
            const Card& c = game.getTableau(i)[k]; 
            if (c.getFaceUp()){ // Card is face up, so we should show it 
                auto sprite = sheet.makeCardSprite(c);
                sprite.setPosition({ stockpileXOffset+(pileSpacing*i), tableauYOffset+(k*tableauYSpacing)} );

                if (draggedCard!=nullptr){ // Before we draw this card out, we need to ensure it isn't being dragged alongside the dragged card 
                    if (draggedCard->getLocation()==Location::Tableau){
                        if (draggedCard->getTableauPile() == c.getTableauPile() && draggedCard->getTableauIndex()<=c.getTableauIndex()){
                            continue; // This is a connected card to the currently dragged card, so it will be rendered in the drawDragging function instead
                        }
                    }
                }

                window.draw(sprite); // Not connected to the dragged card, we're free to render it 

            } else { // Card isn't face up, show back card instead 
                auto sprite = sheet.makeBackSprite();
                sprite.setPosition({ stockpileXOffset+(pileSpacing*i), tableauYOffset+(k*tableauYSpacing)} );
                window.draw(sprite);
            }

        }
    }
  
}

//    --- Dragged cards rendering 
void SolitaireGraphics::drawDragging(sf::RenderWindow& window) const { 

    // window - The Solitaire window object 
    // game - The solitaire game instance storing all game data

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (draggedCard!=nullptr){ // Check if there is a currently dragged card 

        Card draggedCardObj=*draggedCard;
        auto sprite = sheet.makeCardSprite(draggedCardObj);
        sprite.setPosition({ mouse.x+mouseXOffset,mouse.y+mouseYOffset } ); // Set the dragged card to the mouse 
 
        if (draggedCardObj.getLocation()==Location::Tableau){ // Check if any other cards are being dragged alongside this card, only applicable to Tableau cards
           
            // Go through cards ahead of this card in the tableau pile 
            int pile=draggedCardObj.getTableauPile();
            int i=1;
            int pileSize=static_cast<int>(game.getTableau(pile).size());
            for (int c=draggedCardObj.getTableauIndex();c<pileSize;c++){ // These cards are ahead of the dragged card ( Further down the Tableau ), so connected
                Card connectedCard=game.getTableau(pile)[c];
                auto sprite = sheet.makeCardSprite(connectedCard);
                sprite.setPosition({ mouse.x+mouseXOffset,mouse.y+mouseYOffset+(tableauYSpacing*i)} );
                window.draw(sprite); // Draw each connected card, inclusive of the sole dragged card 
                i++;
            }
        
        } else { 
            window.draw(sprite); // No connected cards so just draw the sole dragged card 
        }

    }

}

// -- Draw the undo button
void SolitaireGraphics::drawUndo(sf::RenderWindow& window) const { 

    // window - The Solitaire window object 

    sf::Texture undo=sheet.getUndo(); // Get texture 
    sf::Sprite undoButton{undo}; // Set texture to sprite 
    undoButton.setPosition({ // Position undo button
        undoXOffset,undoYOffset
    });
    window.draw(undoButton); // Draw undo button 
}


// -- Draw the new deal butotn 
void SolitaireGraphics::drawNewDeal(sf::RenderWindow& window) const{ 

    // window - The Solitaire window object 

    sf::Texture newDeal=sheet.getNewDeal(); // Get texture 
    sf::Sprite newDealButton{newDeal}; // Set texture to sprite 
    newDealButton.setPosition({ // Position the new deal button 
        newDealXOffset,newDealYOffset
    });
    window.draw(newDealButton); // Draw the new deal button 
}

// ----- Main Handler
void SolitaireGraphics::draw(sf::RenderWindow& window,const Game& game,bool showWinText) const {

    // window - The Solitaire window object 
    // game - The solitaire game instance storing all game data,
    // showWinText - Whether the player has won, and as such whether to show a win text 

    drawFoundations(window, game);
    drawTableau(window);
    drawStockpile(window,game);
    drawDragging(window);
    drawUndo(window);
    drawNewDeal(window);

}

