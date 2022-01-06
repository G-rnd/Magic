#include <iostream>
#include <string>
#include <vector>

#include "Creature.hpp"
#include "BasicCard.hpp"
#include "Enchantment.hpp"

Creature::Creature(std::string name, int token, bool engaged, bool is_first_turn): 
BasicCard(name, token, engaged, is_first_turn){
    
}

Creature::~Creature(){}

