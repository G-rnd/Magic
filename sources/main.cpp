#include <iostream>
#include <fstream>
#include <string>

#include "Game.hpp"
#include "FonctionsAux.hpp"
#include "DeckBuild.hpp"

int main() {

    std::vector<std::pair<std::string, std::string > > available_decks = {};

    for (const auto & file : std::filesystem::directory_iterator(path))
        available_decks.push_back({"", (file.path()).string().substr(path.size())});

    for (auto deck : available_decks)
    {
        std::cout<< deck<<std::endl;
    }

    return 0;
    

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