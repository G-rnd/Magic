#include <iostream>
#include <string>
#include <vector>

#include "Creature.hpp"
#include "BasicCard.hpp"
#include "Enchantment.hpp"

Creature::Creature(std::string name, int token, bool engaged, bool is_first_turn, int power, int toughness, std::vector<int> abilities, std::vector<int> types): 
BasicCard(name, token, engaged, is_first_turn), m_power(power), m_toughness(toughness), m_power_current(power), m_toughness_current(toughness), m_abilities(abilities), m_types(types){}

Creature::~Creature(){}

