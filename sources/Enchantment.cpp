#include <iostream>
#include <vector>
#include <string>

#include "../includes/Card.hpp"
#include "../includes/SpecialCard.hpp"
#include "../includes/Enchantment.hpp"

Enchantment::Enchantment(int c, std::string name, int token, std::string info, Cost* cost, std::vector<int> effects): 
Card(c, name, token), SpecialCard(c, name, token, info, cost, effects){
    std::cout << "[Enchantment] : Création de " << this << std::endl;
}

Enchantment::~Enchantment(){
    std::cout << "[Enchantment] : Destruction de " << this << std::endl;
}

void Enchantment::print(){
    // TODO : print
}