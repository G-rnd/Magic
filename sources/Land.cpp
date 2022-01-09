#include <iostream>
#include <string>
#include <vector>

#include "../includes/Land.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Enchantment.hpp"

Land::Land(std::string name, int token, int value): 
Card(name, token), BasicCard(name, token), m_value(value){
    std::cout << "[Land] : Création de " << this << std::endl;
}

Land::~Land(){
    std::cout << "[Land] : Destruction de " << this << std::endl;
}

int Land::get_value() const{
    return this->m_value;
}

void Land::set_value(int value){
    this->m_value = value;
}

void Land::print(){

}

