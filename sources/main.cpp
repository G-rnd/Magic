#include <iostream>
#include <string>

#include "Game.hpp"
#include "FonctionsAux.hpp"
#include "DeckBuild.hpp"

int main() {
    Game::print_title_screen();
    while(1){

        std::string s;
        bool quit = false;

        while(!quit){
            std::getline(std::cin, s);
            
            if(s == "play"){
                Game g{};
                g.start();
                quit = true;
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