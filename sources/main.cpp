#include <iostream>
#include <string>

#include "Game.hpp"
#include "FonctionsAux.hpp"
#include "DeckBuild.hpp"

#include "CardParser.hpp"

int main() {
    std::cout << CardParser::parse("data/t0").size() << std::endl;

    return 0;

    while(1){
        std::string s;
        bool quit = false;

        while(!quit){
            Game::print_title_screen();
            std::getline(std::cin, s);
            
            if(s == "play"){
                Game g{};
                g.start();
                cls();
            } else if(s == "load"){
                // TODO : charger une partie
            } else if(s == "deck-build"){
                DeckBuild db;
                db.create_file();
            } else if(s == "exit"){
                return EXIT_SUCCESS;
            } else {
                print_info("Commande non reconnue.");
                Game::print_title_screen();
            }

        }

    }

    return 0;
}