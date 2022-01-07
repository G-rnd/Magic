#include <iostream>
#include <string>
#include <vector>

#include "BasicCard.hpp"
#include "Card.hpp"
#include "Enchantment.hpp"

BasicCard::BasicCard(std::string name, int token, bool engaged, bool is_first_turn): 
Card(name, token), m_engaged(engaged), m_is_first_turn(is_first_turn){}

BasicCard::~BasicCard(){}

bool BasicCard::get_engaged() const{
    return this->m_engaged;
}

bool BasicCard::get_is_first_turn() const{
    return this->m_is_first_turn;
}

std::vector<Enchantment> BasicCard::get_enchantments() const{
    return this->m_enchantments;
}

void BasicCard::set_engaged(bool engaged){
    this->m_engaged = engaged;
}

void BasicCard::set_is_first_turn(bool is_first_turn){
    this->m_is_first_turn = is_first_turn;
}

void BasicCard::add_enchantment(Enchantment e){
    this->m_enchantments.push_back(e);
}