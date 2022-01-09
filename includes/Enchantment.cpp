#include <iostream>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Enchantment.hpp"

Enchantment::Enchantment(std::string name, int token, int id, std::string info): 
Card(name, token), SpecialCard(name, token, id, info){
    std::cout << "[Enchantment] : Création de " << this << std::endl;
}

Enchantment::~Enchantment(){
    std::cout << "[Enchantment] : Destruction de " << this << std::endl;
}

void Enchantment::print(){

}