#include <iostream>
#include <string>

#include "Game.hpp"
#include "FonctionsAux.hpp"
#include "DeckBuild.hpp"

int main() {

    while(1){

        std::cout << "Bienvenue dans une nouvelle édition de Magic The Gathering !" << std::endl << std::endl;

        std::cout << "play       : pour lancer une nouvelle partie." << std::endl;
        std::cout << "load       : pour charger une de vos parties." << std::endl;
        std::cout << "deck-build : pour créer votre propre deck." << std::endl;
        std::cout << "exit       : pour quitter le jeu." << std::endl << std::endl;

        std::cout << "Choississez votre mode : " << std::endl;

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
            }

        }

    }

    return 0;
}