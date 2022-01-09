#include <iostream>
#include <string>
#include <vector>

#include "BasicCard.hpp"
#include "../Card.hpp"
#include "../SpecialCards/Enchantment.hpp"

BasicCard::BasicCard(std::string name, int token): 
Card(name, token){
    std::cout << "[BasicCard] : Création de " << this << std::endl;
    m_engaged = false;
}

BasicCard::~BasicCard(){
    std::cout << "[BasicCard] : Destruction de " << this << std::endl;
}

bool BasicCard::get_engaged() const{
    return m_engaged;
}

std::vector<Enchantment> BasicCard::get_enchantments() const{
    return m_enchantments;
}

void BasicCard::set_engaged(bool engaged){
    m_engaged = engaged;
}

void BasicCard::add_enchantment(Enchantment &e){
    m_enchantments.push_back(e);
}

void BasicCard::print(){
}