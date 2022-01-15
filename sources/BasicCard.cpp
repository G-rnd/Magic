#include <iostream>
#include <vector>
#include <string>

#include "../includes/BasicCard.hpp"
#include "../includes/Card.hpp"
#include "../includes/Enchantment.hpp"

#include "../includes/FonctionsAux.hpp"

BasicCard::BasicCard(int c, std::string name, int token): 
Card(c, name, token){
    std::cout << "[BasicCard] : Création de " << this << std::endl;
    m_engaged = false;
}

BasicCard::~BasicCard(){
    std::cout << "[BasicCard] : Destruction de " << this << std::endl;
}

bool BasicCard::get_engaged() const{
    return m_engaged;
}

std::vector<Enchantment*> BasicCard::get_enchantments() const{
    return m_enchantments;
}

void BasicCard::set_engaged(bool engaged){
    m_engaged = engaged;
}

void BasicCard::add_enchantment(Enchantment* e){
    m_enchantments.push_back(e);
}

void BasicCard::reset_enchantments(){
    m_enchantments = {};
}

std::vector<BasicCard*> BasicCard::remove(std::vector<BasicCard*> cards){
    cards.erase(element_position(this, cards) + cards.begin());
    return cards;
}
