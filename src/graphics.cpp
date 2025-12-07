#include "Graphics.h"
#include "Game.h"
#include "Input.h"
#include <iostream>

// Stockpile

void SolitaireGraphics::drawStockpile(sf::RenderWindow& window,const Game& game) const {
                         
    float cardWidth = static_cast<float>(_sheet.cardWidth());
    float cardHeight = static_cast<float>(_sheet.cardHeight());

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (game.getReserve().empty()){
        // Show the reset card 
        auto sprite = _sheet.makeResetSprite(); // SHOW THE RESET CARD 
        sprite.setPosition({ stockpileXOffset, foundationYOffset });
        window.draw(sprite);
    } else { 
        // Reserve has cards, show card back instead of reset card 
        auto sprite = _sheet.makeBackSprite(); // SHOW THE RESET CARD 
        sprite.setPosition({ stockpileXOffset, foundationYOffset });
        window.draw(sprite);
    }

    // Render all current cards in the stockpile, ( Including dragged ones )
    if (!game.getStockpile().empty()) {
        const Card& c = game.getStockpile().back();

        if (draggedCard==&c){ // This card is currently being dragged, so move it to the mouse 
            auto sprite = _sheet.makeCardSprite(c);
            sprite.setPosition({ mouse.x + mouseXOffset, mouse.y + mouseYOffset });
            window.draw(sprite);
        } else { 
            auto sprite = _sheet.makeCardSprite(c);
            sprite.setPosition({ stockpileXOffset+pileSpacing, foundationYOffset });
            window.draw(sprite);
        }

    } 
}

// Foundation piles 
void SolitaireGraphics::drawFoundations(sf::RenderWindow& window,const Game& game) const {

    float cardWidth = static_cast<float>(_sheet.cardWidth());
    float cardHeight= static_cast<float>(_sheet.cardHeight());
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    float xStart = stockpileXOffset + 3.0f * pileSpacing;
    float y = foundationYOffset;

    for (int i = 0; i < 4; ++i) {

        float x = xStart+i*pileSpacing;

        if (game.getFoundation(i).empty()) {
            sf::RectangleShape rect;
            rect.setSize({ cardWidth, cardHeight });
            rect.setPosition({ x, y });
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color(200, 200, 200));
            rect.setOutlineThickness(2.f);
            window.draw(rect);
        } else {
            int index=1;
            const Card& card=game.getFoundation(i).back();
            if (&card==draggedCard) {
                index=2;
            }
            const Card& c=game.getFoundation(i)[game.getFoundation(i).size()-index];
            auto sprite = _sheet.makeCardSprite(c);
            sprite.setPosition({ x, y });
            window.draw(sprite);
        }

    }
}

// ---- TABLEAU ----
void SolitaireGraphics::drawTableau(sf::RenderWindow& window,const Game& gameData) const {

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (int i=0;i<7;i++){
        std::vector<Card> cards=game.getTableau(i);
        for (int k=0;k<cards.size();k++){
            const Card& c = game.getTableau(i)[k]; 
            if ( c.getFaceUp() ){ // Card is face up, so show it 
                auto sprite = _sheet.makeCardSprite(c);
                sprite.setPosition({ stockpileXOffset+(pileSpacing*i), tableauYOffset+(k*tableauYSpacing)} );
                    // Ensure this card isn't a connected card of the dragged card 
                if (draggedCard!=nullptr){
                    if (draggedCard->getLocation()==Location::Tableau){
                        if (draggedCard->getTableauPile() == c.getTableauPile() && draggedCard->getTableauIndex()<=c.getTableauIndex()){
                            // This is a connected card to the currently dragged card so it will be rendered in the drawDragging function instead
                            continue;
                        }
                    }
                }
                window.draw(sprite);
            } else { 
                auto sprite = _sheet.makeBackSprite();
                sprite.setPosition({ stockpileXOffset+(pileSpacing*i), tableauYOffset+(k*tableauYSpacing)} );
                window.draw(sprite);
            }

        }
    }
  
}

void SolitaireGraphics::drawDragging(sf::RenderWindow& window,const Game& gameData) const { 

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (draggedCard!=nullptr){

        Card draggedCardObj=*draggedCard;
        auto sprite = _sheet.makeCardSprite(draggedCardObj);
        sprite.setPosition({ mouse.x+mouseXOffset,mouse.y+mouseYOffset } );

        // Also check for connected cards if the dragged card is on the tableau
        if (draggedCardObj.getLocation()==Location::Tableau){
            // Go through cards ahead of this card in the tableau pile 
            int index=draggedCardObj.getTableauIndex();
            int pile=draggedCardObj.getTableauPile();
            int i=1;

            for (int c=draggedCardObj.getTableauIndex();c<game.getTableau(pile).size();c++){
                Card connectedCard=game.getTableau(pile)[c];
                auto sprite = _sheet.makeCardSprite(connectedCard);
                sprite.setPosition({ mouse.x+mouseXOffset,mouse.y+mouseYOffset+(tableauYSpacing*i)} );
                window.draw(sprite);
                i++;
            }
        
        } else { 
            window.draw(sprite);
        }

    }

}

void SolitaireGraphics::drawUndo(sf::RenderWindow& window) const { 

    sf::Texture undo=_sheet.getUndo();
    sf::Sprite undoButton{undo};
    undoButton.setPosition({
        undoXOffset,undoYOffset
    });
    window.draw(undoButton);

}

// Handler
void SolitaireGraphics::draw(sf::RenderWindow& window,const Game& game,bool showWinText) const {

    drawFoundations(window, game);
    drawTableau(window,game);
    drawStockpile(window,game);
    drawDragging(window,game);
    drawUndo(window);

}

