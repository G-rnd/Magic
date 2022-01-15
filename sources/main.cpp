#include <iostream>
#include <typeinfo>
#include <iomanip>

#include "../includes/Game.hpp"
#include "CardParser.hpp"

#include "../includes/FonctionsAux.hpp"

int main() {
    Land* l1  = new Land(1, "terrain 1", 2, 999);
    Land* l2  = new Land(1, "terrain 2", 2, 3);
    Land* l3  = new Land(1, "terrain 333333333", 2, 3);
    Creature* c1 = new Creature(0, "crecre", 1, 90 ,1, {}, {}, new Cost(1, 1, 1, 1, 1, 1));
    Creature* c2 = new Creature(0, "yolo", 1, 1 ,1, {}, {}, new Cost(1, 1, 1, 1, 1, 1));
    Creature* c3 = new Creature(0, "Ange de ces mprts", 1, 1 ,1, {}, {}, new Cost(1, 1, 1, 1, 1, 1));

    std::vector<BasicCard*> bc = {l1, c1, l2, l3, c2, c3};

    Battlefield* b = new Battlefield();
    b->set_basic_cards(bc);

    b->print();

    std::cout << "Entrée pour continuer." << std::endl;
    std::string cmd;
    std::getline(std::cin, cmd);
  
    Game g{};
    g.start();


    std::cout << "Entrée pour continuer." << std::endl;
    std::getline(std::cin, cmd);
  
    return 0;
}