#include <iostream>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Ritual.hpp"

Ritual::Ritual(std::string name, int token, int id, std::string info): 
Card(name, token), SpecialCard(name, token, id, info){
    std::cout << "[Ritual] : Création de " << this << std::endl;
}

Ritual::~Ritual(){
    std::cout << "[Ritual] : Destruction de " << this << std::endl;
}

void Ritual::print(){}