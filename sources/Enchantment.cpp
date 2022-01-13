#include <iostream>
#include <string>

#include "../includes/Card.hpp"
#include "../includes/SpecialCard.hpp"
#include "../includes/Enchantment.hpp"

Enchantment::Enchantment(std::string name, int token, int id, std::string info, Cost* cost): 
Card(name, token), SpecialCard(name, token, id, info, cost){
    std::cout << "[Enchantment] : Création de " << this << std::endl;
}

Enchantment::~Enchantment(){
    std::cout << "[Enchantment] : Destruction de " << this << std::endl;
}

void Enchantment::print(){
    // TODO : print
}