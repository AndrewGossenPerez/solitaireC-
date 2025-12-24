//Game.h created by Andrew Gossen.
// A core header for the game
// Stores arrays for the Stockpile, Tableau, and Foundation Piles 

#pragma once
#include "Card.h" // Access Card object 
#include "Move.h" // Access Move object 
#include "Game.h"
#include <array>
#include <vector>
#include "Move.h"

// Represents current state of the game and holds functions to make modifications to game
class Game{

public:
    
    void dealNewGame(); // Will clear foundation piles and establish the stockpile and Tableau for a new game.
    void applyMove(const Move& move,bool undo); // Will apply a move (assumed to be Valid) onto the private arrays in Game
    void undo(); // Undos the latest move 
    bool validMove(const Move& move) const; // Returns whether a move is legal for Solitaire Klondike. 
    void dealFromReserve(); // Will add a card from the reserve to the stockpile as the player wants to deal
    void resetStockpile(); // Will add a card from the reserve to the stockpile as the player wants to deal

    //Getters
    const std::vector<Card>& getStockpile() const { return stockpile; }
    const std::vector<Card>& getReserve() const { return reserve; }
    const std::vector<Card>& getTableau(int i) const { return tableau[i]; }
    const std::vector<Card>& getFoundation(int i) const { return foundations[i]; }
    bool getWon() const { return won; }

    //Setters
    void setWon(bool hasWon) {won=hasWon;}

private:

    bool won=false; // Whether the game has been won 

    void FoundationLogic(const Move& move, const Card& movingCard,std::vector<Card> &cardArray,bool undo);
    void TableauToTableauLogic(const Move& move, const Card& movingCard,bool undo);
    void pushToTableau(const Move& move, Card movingCard,std::vector<Card> &popBackArray);
    void pushToStockpile(Card movingCard,std::vector<Card> &popBackArray);
    void logMove(Card c,Move move);

    std::vector<Card> reserve; // Holds all cards not yet dealt
    std::vector<Card> stockpile; // Holds all cards currently dealt
    std::vector<Move> moveHistory; // Holds history of moves for the undo function 
    std::array<std::vector<Card>, 7> tableau;  // Holds each seven Tabelau piles and their respective cards.
    std::array<std::vector<Card>, 4> foundations; // Holds each four foundation piles and their respective cards.

};