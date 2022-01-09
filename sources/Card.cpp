#include <iostream>
#include <string>

#include "../includes/Card.hpp"

Card::Card(std::string name, int token): m_name(name), m_token(token){
    std::cout << "[Card] : Création de " << this << std::endl;
}

Card::~Card(){
    std::cout << "[Card] : Destruction de " << this << std::endl;

}

std::string Card::get_name() const{
    return m_name;
}

int Card::get_token() const{
    return m_token;
}

void Card::set_name(std::string name){
    m_name = name;
}

void Card::set_token(int token){
    m_token = token;
}

