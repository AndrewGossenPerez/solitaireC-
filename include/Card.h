// Card.h, created by Andrew Gossen.
// Defines a card object, i.e. as Queen of Hearts, used to represent a playing card
// Stores the suit, card value, whether the card is face up, and the cards location

#pragma once 
#include <iostream>

enum class Suit{ // Card Suits
Spades,Hearts,Clubs,Diamonds
};
enum class Value{ // Cards value in ascending order 
    Ace,Two,Three,Four,Five,Six,Seven,Eight,Nine,Ten,Jack,Queen,King
};
enum class Location{ // Enum of possible card locations
    Reserve,Stockpile,Tableau,Foundation,Undecided
};

// Represents a single playing card with suit, value, face-up state, and card location.
class Card{

public:

    // Constructor 
    Card(Suit suit, Value value,bool faceUp,Location location, bool dragging)
    : suit(suit), value(value), faceUp(faceUp), location(location), dragging(dragging) {}; 

    // Getter functions
    Suit getSuit() const{ return suit; }
    Value getValue() const { return value; }
    Location getLocation() const { return location; }
    bool getFaceUp() const { return faceUp; }
    int getTableauIndex() const { return tableauIndex;}
    int getTableauPile() const { return tableauPile; }
    int getFoundationPile() const { return foundationPile; }

    // Setter functions 
    void setFaceUp(bool up) {faceUp = up;}
    void setDragging(bool _dragging) {dragging = _dragging;}
    void setLocation(Location loc) {location = loc;}
    void setTableauIndex(int index) {tableauIndex = index;}
    void setTableauPile(int pile) {tableauPile = pile;}
    void setFoudationPile(int pile) {foundationPile = pile;}

private:

    Suit suit; // The suit of the card 
    Value value; // The value of the card, i.e. Ace 

    bool faceUp;
    Location location; // The location of the card 
    bool dragging=false; // Whether the player is currently dragging this card around 

    // Below are -1 if it's not in such pile
    int tableauIndex=-1; // Incase it's in a Tableau Pile, keep note of it's index so I don't need to iterate each time to find out
    int tableauPile=-1; // Incase it's in a Tableau Pile, keep note of it's pile index so I don't need to iterate each time to find out
    int foundationPile=-1; // Incase it's in a Foundation piile, keep note of it's pile index 

};