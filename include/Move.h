// Move.h created by Andrew Gossen.
// Defines a move object, used for move-validation

#pragma once 
#include "Card.h" // Include Card.h for Card object and location enum

// Class representing a move, including which card we want to move, and where to 
class Move{

    public:
    Move(Card card,Location startingPosition,Location destination,int pile,int startingPile)
    : card(card), pile(pile),startingPosition(startingPosition),destination(destination),startingPile(startingPile) {};

    // Getter functions
    int getPile() const {return pile;}
    int getStartingPile() const {return startingPile;}
    Location getStartingPosition() const {return startingPosition;} 
    Location getDestination() const {return destination;} 
    const Card& getCard() const { return card;} 

    // Setter functions 
    void setCard(Card c) {card=c;} // Set card after we've deleted the original and placed a new one in desired position.

    private:

    int pile; // Pile card wants to move to 
    int startingPile; // Pile card is currently in 
    Card card;
    Location startingPosition;
    Location destination;

};