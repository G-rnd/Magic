#include <iostream>
#include <vector>
#include <string>

#include "../includes/Card.hpp"
#include "../includes/SpecialCard.hpp"
#include "../includes/Ritual.hpp"

Ritual::Ritual(std::string name, int token, int id, std::string info, Cost* cost, std::vector<int> effects): 
Card(name, token), SpecialCard(name, token, id, info, cost), m_effects(effects){
    std::cout << "[Ritual] : Création de " << this << std::endl;
}

Ritual::~Ritual(){
    std::cout << "[Ritual] : Destruction de " << this << std::endl;
}

std::vector<int> Ritual::get_effects() const{
    return m_effects;
}

void Ritual::print(){
    // TODO : print
}

