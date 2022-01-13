#include <iostream>
#include <string>

#include "../includes/Card.hpp"
#include "../includes/SpecialCard.hpp"
#include "../includes/Ritual.hpp"

Ritual::Ritual(std::string name, int token, int id, std::string info, Cost* cost): 
Card(name, token), SpecialCard(name, token, id, info, cost){
    std::cout << "[Ritual] : Création de " << this << std::endl;
}

Ritual::~Ritual(){
    std::cout << "[Ritual] : Destruction de " << this << std::endl;
}

void Ritual::print(){
    // TODO : print
}