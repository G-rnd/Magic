#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip> 

#include "Game.hpp"
#include "FonctionsAux.hpp"
#include "DeckBuild.hpp"

int main() {

    while(1){
        std::string s;
        bool quit = false;

        while(!quit){
            Game::print_title_screen();
            std::getline(std::cin, s);
            
            if(s == "play"){
                Game* g = new Game();
                g->start();
                delete g;
            } else if(s == "load"){
                Game::load();
                cls();
            } else if(s == "deck-builder"){
                DeckBuild db;
                db.create_file();
            } else if(s == "exit"){
                return EXIT_SUCCESS;
            } else {
                print_info("Commande non reconnue.");
            }
        }
    }

    return 0;
}