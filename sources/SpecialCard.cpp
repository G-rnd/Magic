#include <iostream>
#include <vector>
#include <string>

#include "SpecialCard.hpp"
#include "Card.hpp"
#include "Cost.hpp"

SpecialCard::SpecialCard(int c, std::string name, int token, Cost* cost, std::vector<int> effects): 
Card(c, name, token), m_cost(cost), m_effects(effects) {
    //std::cout << "[SpecialCard] : Création de " << this << std::endl;
}

SpecialCard::~SpecialCard() {
    //std::cout << "[SpecialCard] : Destruction de " << this << std::endl;
}

Cost* SpecialCard::get_cost() const {
    return m_cost;
}

std::vector<int> SpecialCard::get_effects() const {
    return m_effects;
}