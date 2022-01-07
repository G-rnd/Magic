#include <iostream>
#include <string>

#include "Card.hpp"

Card::Card(std::string name, int token): m_name(name), m_token(token){}

Card::~Card(){}

std::string Card::get_name() const{
    return this->m_name;
}

int Card::get_token() const{
    return this->m_token;
}

void Card::set_name(std::string name){
    this->m_name = name;
}

void Card::set_token(int token){
    this->m_token = token;
}

