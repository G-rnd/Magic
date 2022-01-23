#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Ritual.hpp"

Ritual::Ritual(int c, std::string name, int token, Cost* cost, std::vector<int> effects): 
Card(c, name, token), SpecialCard(c, name, token, cost, effects) {
    //std::cout << "[Ritual] : Création de " << this << std::endl;
}

Ritual::~Ritual() {
    //std::cout << "[Ritual] : Destruction de " << this << std::endl;
}

void Ritual::print() {
    // TODO : print
}
