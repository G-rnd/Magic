#include <iostream>
#include <string>
#include <vector>

#include "Land.hpp"
#include "BasicCard.hpp"
#include "Enchantment.hpp"
#include "Cost.hpp"

Land::Land(std::string name, int token, bool engaged, bool is_first_turn, int value): 
BasicCard(name, token, engaged, is_first_turn), m_value(value){}

Land::~Land(){}

int Land::get_value() const{
    return this->m_value;
}

void Land::set_value(int value){
    this->m_value = value;
}

