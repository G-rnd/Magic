#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Enchantment.hpp"

Enchantment::Enchantment(int c, std::string name, int token, Cost* cost, std::vector<int> effects): 
Card(c, name, token), SpecialCard(c, name, token, cost, effects) {
    //std::cout << "[Enchantment] : Création de " << this << std::endl;
}

Enchantment::~Enchantment() {
    //std::cout << "[Enchantment] : Destruction de " << this << std::endl;
}

void Enchantment::print() {
    // TODO : print
}
