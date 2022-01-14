#include <iostream>

#include "../includes/Game.hpp"
#include "CardParser.hpp"

int main() {
    Game g{};
    g.start();


    std::cout << "Entrée pour continuer." << std::endl;
    std::cin.ignore(32767,'\n');
    return 0;
}