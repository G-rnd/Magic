#include <iostream>
#include <string>
#include <vector>

#include "Land.hpp"
#include "BasicCard.hpp"
#include "Enchantment.hpp"

Land::Land(int c, std::string name, int token, int value): Card(c, name, token), BasicCard(c, name, token), m_value(value) {
    //std::cout << "[Land] : Création de " << this << std::endl;
}

Land::~Land(){
    //std::cout << "[Land] : Destruction de " << this << std::endl;
}

int Land::get_value() const{
    return m_value;
}

void Land::set_value(int value){
    m_value = value;
}

void Land::print(){
    // TODO : print
}

