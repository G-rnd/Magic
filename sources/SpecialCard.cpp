#include <iostream>
#include <vector>
#include <string>

#include "../includes/SpecialCard.hpp"
#include "../includes/Card.hpp"
#include "../includes/Cost.hpp"
#include "Land.hpp"

SpecialCard::SpecialCard(int c, std::string name, int token, std::string info, Cost* cost, std::vector<int> effects): 
Card(c, name, token), m_info(info), m_cost(cost), m_effects(effects){
    //std::cout << "[SpecialCard] : Création de " << this << std::endl;
}

SpecialCard::~SpecialCard(){
    //std::cout << "[SpecialCard] : Destruction de " << this << std::endl;
}

std::string SpecialCard::get_info() const{
    return m_info;
}

Cost* SpecialCard::get_cost() const{
    return m_cost;
}

std::vector<int> SpecialCard::get_effects() const{
    return m_effects;
}

void SpecialCard::set_info(std::string info){
    m_info = info;
}