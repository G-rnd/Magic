#include <iostream>
#include <string>

#include "Cards/Card.hpp"
#include "Cards/SpecialCards/SpecialCard.hpp"
#include "Enchantment.hpp"

Enchantment::Enchantment(std::string name, int token, int id, std::string info): 
Card(name, token), SpecialCard(name, token, id, info){}

Enchantment::~Enchantment(){}