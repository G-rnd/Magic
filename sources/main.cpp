#include <iostream>
#include <string>

#include "Game.hpp"
#include "FonctionsAux.hpp"
#include "DeckBuild.hpp"

#include "CardParser.hpp"

int main() {

/*
    std::string s = "hello world";
    std::cout << get_color(Color::Magenta) << s << std::endl;

    std::cout << get_color(Color::Yellow) << s << get_color(Color::BrightRed) << std::endl;

    std::cout << get_background_color(Color::Magenta) << s << get_background_color(Color::Reset) << std::endl;
    
    return 0;*/

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
            } else if(s == "deck-builder"){
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