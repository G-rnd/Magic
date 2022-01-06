#include <iostream>
#include <string>
#include <vector>

#include "BasicCard.hpp"
#include "Card.hpp"
#include "Enchantment.hpp"

BasicCard::BasicCard(std::string name, int token, bool engaged, bool is_first_turn): 
Card(name, token), m_engaged(engaged), m_is_first_turn(is_first_turn){}

BasicCard::~BasicCard(){}

