// Main.cpp created by Andrew Gossen.
// Self explanatory

#include <optional>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Spritesheet.h"
#include "Graphics.h"
#include "Input.h"
#include <iostream>

// -- The main function for this Solitaire gmae 
int main() {

    sf::RenderWindow window(sf::VideoMode({ 1024u, 768u }), "Solitaire");
    window.setFramerateLimit(140);

    // Load all assets
    Spritesheet sheet;
    sf::Font font;
    if (!sheet.loadFromFile("assets/Spritesheet.png")) return 1;
    if (!sheet.loadUndo("assets/Undo.png")) return 1;
    if (!sheet.loadNewDeal("assets/NewDeal.png")) return 1;
    if (!font.openFromFile("assets/arial.ttf")) return 1;

    // Establish our essential objects
    Game game;
    game.dealNewGame();
    SolitaireGraphics graphics(sheet,font,game);
    Input input(game,graphics,sheet);

    while (window.isOpen()) { 

        while (const std::optional<sf::Event> event = window.pollEvent()) { // Player has initiated an event 
            if (event->is<sf::Event::Closed>()) { // Player wants to close the window 
                window.close(); // Close the window  
            }
        }

        window.clear(sf::Color(0, 120, 0)); // Establish a green background 
        input.getHovered(window); // Establish any hovered cards ( Cards that the mouse is on top of for dragging/release )
        graphics.draw(window, game,false); // Render 
        window.display(); // Display 

    }

    return 0;

}
