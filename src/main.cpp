// Main.cpp created by Andrew Gossen.

#include <optional>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Spritesheet.h"
#include "Graphics.h"
#include "Input.h"
#include <iostream>


int main() {

    sf::RenderWindow window(sf::VideoMode({ 1024u, 768u }), "Solitaire");
    window.setFramerateLimit(60);

    // Load all assets
    Spritesheet sheet;
    sf::Font font;
    if (!sheet.loadFromFile("assets/Spritesheet.png")) return 1;
    if (!sheet.loadUndo("assets/Undo.png")) return 1;
    if (!sheet.loadNewDeal("assets/NewDeal.png")) return 1;
    if (!font.openFromFile("assets/arial.ttf")) return 1;

    Game game;
    game.dealNewGame();
    SolitaireGraphics graphics(sheet,font,game);
    Input input(game,graphics,sheet);

    while (window.isOpen()) {

        while (const std::optional<sf::Event> event = window.pollEvent()) {
           
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

        }

        window.clear(sf::Color(0, 120, 0)); // table green
        input.getHovered(window);
        graphics.draw(window, game,false);
        window.display();

    }

    return 0;

}
