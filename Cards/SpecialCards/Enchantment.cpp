#include <iostream>
#include <string>

#include "../Card.hpp"
#include "../SpecialCards/SpecialCard.hpp"
#include "Enchantment.hpp"

Enchantment::Enchantment(std::string name, int token, int id, std::string info): 
Card(name, token), SpecialCard(name, token, id, info){}

Enchantment::~Enchantment(){}

void Enchantment::print(){

}