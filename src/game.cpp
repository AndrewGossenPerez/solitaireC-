// Game.cpp, created by Andrew Gossen.
// The main framework of this Solitaire project, handles game events such as dealing and reseting the stockpile
// Handles the actual game logic following Klondike Solitaire

// Essential headers
#include "Game.h"
#include "Card.h"
#include <random>
#include <iostream> // Debugging remove later 

// Debugging 
void Game::printTableau(int p){
    std::cout<<"Printing";
    for (int c=0;c<tableau[p].size();c++){
        std::cout<<static_cast<int>(tableau[p][c].getValue());
        std::cout<<static_cast<int>(tableau[p][c].getSuit());
    }
}

// Game events 
void Game::resetStockpile(){ // Resets the stockpile by returning all cards back to the reserve
    if (stockpile.empty()) return;
    std::cout << reserve.size() << std::endl;
    if (!reserve.empty()) return;

    while (!stockpile.empty()) {
        Card c = stockpile.back();           
        c.setLocation(Location::Reserve);
        reserve.insert(reserve.end(), c);  
        stockpile.pop_back();               
    }
}

void Game::dealNewGame(){

    // Erase current game state
    if (!stockpile.empty())  stockpile.clear();
    if (!reserve.empty())  reserve.clear();

    for (int i=0;i<7;i++){
        if (i<=3) { 
            if (!foundations.empty()) foundations[i].clear(); 
        }
        if (!tableau.empty()) tableau[i].clear();
    };

    for (int s = 0; s < 4; ++s) { // Create the possible 52 cards 
        for (int v = 0; v < 13; ++v) {
            reserve.push_back(Card{
                static_cast<Suit>(s),
                static_cast<Value>(v),
                false,
                Location::Reserve, 
                false
            });
        }
    }

    static std::mt19937 rng(std::random_device{}()); // Tick dependant RNG 
    std::shuffle(reserve.begin(), reserve.end(), rng); // Shuffle stockpile to make it random 

    // Assign cards to Tableau, rest will remain in the reserve
    for (int p=0;p<7;p++){ // Pile iteration
        for (int c=0;c<(p+1);c++){
            Card card=reserve.back();
            card.setLocation(Location::Tableau);
            reserve.pop_back();
            tableau[p].push_back(card);
            if (c==p){
                tableau[p][c].setFaceUp(true);
            }
            tableau[p][c].setTableauPile(p);
            tableau[p][c].setTableauIndex(c);
        }
    }

    printTableau(2);

    return;

}

void Game::dealFromReserve(){ // Moves a card from the reserve to the stockpile, hence dealing it

    if (reserve.empty()) return; 
    Card c = reserve.back();
    c.setLocation(Location::Stockpile);
    reserve.pop_back();
    stockpile.push_back(c); // Goes from Back of stockpile -> Front of stockpile, i.e. last index is currently shown card

    // Log the move 
    Move move(
        c,
        Location::Reserve,
        Location::Stockpile,
        -1,-1
    );

    logMove(c,move);


}

// Helper Functions 
void Game::pushToTableau(const Move &move,Card card,std::vector<Card> &popBackArray){
    card.setLocation(Location::Tableau);
    card.setFaceUp(true);
    card.setTableauIndex(tableau[move.getPile()].size());
    card.setTableauPile(move.getPile());
    card.setFoudationPile(-1);
    tableau[move.getPile()].push_back(card);
    popBackArray.pop_back();
    popBackArray.back().setFaceUp(true);
}

void Game::pushToStockpile(const Move& move, Card card,std::vector<Card> &popBackArray){
    std::cout << "Pushing to stockpile" << std::endl;
    card.setLocation(Location::Stockpile);
    card.setFaceUp(true);
    card.setTableauIndex(-1);
    card.setTableauPile(-1);
    card.setFoudationPile(-1);
    stockpile.push_back(card);
    popBackArray.pop_back();
}

// Logic functions 

void Game::logMove(Card c,Move originalMove){

    Move newMove(
        c,
        originalMove.getStartingPosition(),
        originalMove.getDestination(),
        originalMove.getPile(),
        originalMove.getStartingPile()
    );

    moveHistory.push_back(newMove);

}

void Game::FoundationLogic(const Move& move, const Card& movingCard,std::vector<Card> &popBackArray,bool undo){ // Game Logic for a move to the foundation pile 

    // This is assuming that movingCard is not connected to any other cards ( I.e. not a Tableau drag with multiple cards involved)
    // ^ This is pre-validated before this function is ran
            
    int cardValue=static_cast<int>(movingCard.getValue());
    int suitValue=static_cast<int>(movingCard.getSuit()); // If the suit is <=1 then red, else >=2 black
    bool proceed=false;
    
    if (foundations[move.getPile()].empty()){
        if (cardValue==0){ // Ace 
            // Move to foundation pile as we're moving an ace to an empty foundation pile
            proceed=true;
        }
    } else {
        Card& foundationCard=foundations[move.getPile()].back();
        int foundationValue=static_cast<int>(foundationCard.getValue());
        if (foundationCard.getSuit()==movingCard.getSuit() && cardValue==foundationValue+1){
            // Move to foundation as we're moving the same suit but one higher than current top-most foundation card
            proceed=true;
        }
    }

    if (proceed){

        Card c=movingCard;

        c.setLocation(Location::Foundation);
        c.setTableauPile(-1);
        c.setTableauIndex(-1);
        c.setFoudationPile(move.getPile());
        c.setFaceUp(true);
        foundations[move.getPile()].push_back(c);
        popBackArray.pop_back();

        if(!undo){
            // Create a clone card with the new pile and index so we can create a 'flipped' move in case the player Undoes
            Card clone=c;
            logMove(clone,move);
        } 


    }


};

void Game::TableauToTableauLogic(const Move& move, const Card& movingCard,bool undo){ // Game Logic for a move to a Tableau pile 

    // Moving card in this instance can be connected to other cards 

    int cardValue=static_cast<int>(movingCard.getValue());
    int suitValue=static_cast<int>(movingCard.getSuit()); 

    std::cout << " Tableau Pile : "<< static_cast<int>(move.getPile());

    if (undo){

        std::cout << static_cast<int>(movingCard.getTableauPile());
        std::cout << static_cast<int>(movingCard.getTableauIndex());
        std::cout << static_cast<int>(move.getPile());

        // If it's an undo doesn't need to follow game logic
        int endIndex=static_cast<int>(tableau[movingCard.getTableauPile()].size());
        int indexsGoneThrough=1;
        for (int c=movingCard.getTableauIndex();c<endIndex;c++){ // Go through moved card and connected cards, and pop back
            pushToTableau(move,tableau[movingCard.getTableauPile()][c],tableau[movingCard.getTableauPile()]);
            indexsGoneThrough++;
        }

        // Set the card above the intended index to faceDown now, as this is what it would've been before the move 
        tableau[move.getPile()][tableau[move.getPile()].size()-indexsGoneThrough].setFaceUp(false);

        return;

    }
    
    int newIndex=-1;

    if (!tableau[move.getPile()].empty()){
       
        Card &pileEndCard=tableau[move.getPile()].back();
        int endCardValue=static_cast<int>(pileEndCard.getValue());
        int cardSuit=static_cast<int>(pileEndCard.getSuit());
        bool differentColors =(cardSuit%2)!=(suitValue%2);

        if (endCardValue==cardValue+1 && differentColors){
            int endIndex=static_cast<int>(tableau[movingCard.getTableauPile()].size());
            newIndex=tableau[move.getPile()].size();
            for (int c=movingCard.getTableauIndex();c<endIndex;c++){ // Go through moved card and connected cards, and pop back
                pushToTableau(move,tableau[movingCard.getTableauPile()][c],tableau[movingCard.getTableauPile()]);
            }
        }

    } else if(cardValue==12) {  // King move to an empty Tableau pile 
        // Move card and all connected cards 
        int endIndex=static_cast<int>(tableau[movingCard.getTableauPile()].size());
        newIndex=tableau[move.getPile()].size();
        for (int c=movingCard.getTableauIndex();c<endIndex;c++){ // Go through moved card and connected cards, and pop back
            pushToTableau(move,tableau[movingCard.getTableauPile()][c],tableau[movingCard.getTableauPile()]);
        }
    }

    // Create a clone card with the new pile and index so we can create a 'flipped' move in case the player Undoes
    Card clone=movingCard;
    clone.setTableauPile(move.getPile());
    clone.setTableauIndex(newIndex);
    if (!undo) logMove(clone,move);

};

void Game::applyMove(const Move& move,bool undo){ // Applies a move based on the logic of Klondike Solitaire 

    std::cout<<"Applying 2" <<std::endl;

    const Card& movingCard=move.getCard();
    int cardValue=static_cast<int>(movingCard.getValue());
    int suitValue=static_cast<int>(movingCard.getSuit()); // Red colour has property such that %2==1 
    
    if (move.getStartingPosition()==Location::Stockpile){  // We are moving a stockpile card

        std::cout << static_cast<int>(move.getDestination());

        // In this case, we're either moving from the stockpiole to the Tableau or Foundation


        if (move.getDestination()==Location::Foundation){ // We want to move from Stockpile to the Foundation
            FoundationLogic(move,movingCard,stockpile,undo);
        }

        if (move.getDestination()==Location::Tableau){ // We want to move from Stockpile to a Tableau pile
    
            if (tableau[move.getPile()].empty() && cardValue==12){
                std::cout << " King Move stockpile " << std::endl;
                // King move to an empty Tablau pile 
                Card c=movingCard;
                pushToTableau(move,c,stockpile);
            }

            if (!tableau[move.getPile()].empty()){

                std::cout << " Moving to non-empty pile from stockpile, validate " << std::endl;

                Card endCard=tableau[move.getPile()].back();
                int endCardValue=static_cast<int>(endCard.getValue());
                bool differentColors =(static_cast<int>(endCard.getSuit())%2)!=(suitValue%2);
                std::cout << "Different colors : " << differentColors << std::endl;

                if (differentColors && (endCardValue==cardValue+1)){
                    Card c=movingCard;
                    pushToTableau(move,c,stockpile);
                }

            }

            if (undo){ // At this point the index would be size-1 
                Card c=movingCard;
                c.setTableauIndex(tableau[move.getPile()].size()-1);
                c.setTableauPile(move.getPile());
                c.setFoudationPile(-1);
                logMove(c,move);
            }

        }

    }

    if (move.getStartingPosition()==Location::Tableau){ // We're moving from the Tableau to either Foundation or another Tableau pile 
       
        if (move.getDestination()==Location::Foundation && !undo){ // We want to move from Tableau to the Foundation, so the dragged card must be the last card of the pile
            if (tableau[movingCard.getTableauPile()].size()==movingCard.getTableauIndex()+1){
                // This is the last card, so we can apply Foundation logic 
                FoundationLogic(move,movingCard,tableau[movingCard.getTableauPile()],undo);
                tableau[movingCard.getTableauPile()].back().setFaceUp(true);
            }
        } 

        if (move.getDestination()==Location::Stockpile){ // This would occur during an Undo
            std::cout<<"WE WANT TO RETURN TO THE STOCKPILE !!!"<<std::endl;
            pushToStockpile(move,movingCard,tableau[movingCard.getTableauPile()]);
        }
        
        if (move.getDestination()==Location::Tableau){
            std::cout << " tableu-tablau " << std::endl;
            TableauToTableauLogic(move,movingCard,undo);
        }

    }

    if (move.getStartingPosition()==Location::Foundation){ // Moving from foundation to a tableau pile 

        std::cout << " Location is foundation " <<std::endl;

        Card endCard=tableau[move.getPile()].back();
        int endCardSuit=static_cast<int>(endCard.getSuit());
        int endCardValue=static_cast<int>(endCard.getValue());
        bool differentColors =(endCardSuit%2)!=(suitValue%2);

        if (move.getDestination()==Location::Stockpile){ // This would occur during an Undo
            std::cout<<"WE WANT TO RETURN TO THE STOCKPILE"<<std::endl;
            pushToStockpile(move,movingCard, foundations[movingCard.getFoundationPile()]);
        } else if ( (differentColors && endCardValue==cardValue+1) || undo ){
            // Valid move, push it to the tableau
            Card c=movingCard; // Create clone 
            std::cout << "Push to tableau" << std::endl;
            pushToTableau(move,c,foundations[movingCard.getFoundationPile()]);
            if (undo){
                // Make the index before the newly applied card now face down
                int faceDownIndex=tableau[move.getPile()].size()-2;
                tableau[move.getPile()][faceDownIndex].setFaceUp(false);
            }
        } 

    }

    // Check if they've won after applying a move 
    bool hasWon=true;
    for (int i=0;i<4;i++){
        if (foundations[i].size()!=13) hasWon=false;
    }
    won=hasWon;

}

void Game::undo(){

    if (moveHistory.empty()) return; 

    std::cout << "Performing an Undo " << std::endl;
    
    Move &lastMove=moveHistory.back();

    Location startingPosition=lastMove.getStartingPosition();
    Location destination=lastMove.getDestination();
    int startingPile=lastMove.getStartingPile();

    std::cout<<"Destination : " << static_cast<int>(destination) << std::endl;
    std::cout<<"StartingPosition : " << static_cast<int>(startingPosition) << std::endl;
    std::cout<<"destination Pile : " << static_cast<int>(startingPile) << std::endl;

    Move undoMove( // Essentialy create a 'flipped' move of the last move 
        lastMove.getCard(),
        destination,
        startingPosition,
        startingPile,
        -1
    );

    if (startingPosition==Location::Reserve) {
        // Bring the dealt card back to the reseve 
        Card card=stockpile.back();
        reserve.push_back(card);
        stockpile.pop_back();
    } else { 
        applyMove(undoMove,true);
        moveHistory.pop_back();
    }

}