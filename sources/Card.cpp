#include <iostream>
#include <vector>
#include <string>

#include "../includes/Card.hpp"

#include "../includes/FonctionsAux.hpp"

Card::Card(int c, std::string name, int token): m_class(c), m_name(name), m_token(token){
    std::cout << "[Card] : Création de " << this << std::endl;
}

Card::~Card(){
    std::cout << "[Card] : Destruction de " << this << std::endl;

}

int Card::get_class() const{
    return m_class;
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

bool Card::is_class(int cl) const{
    return this->get_class() == cl;
}

std::vector<Card*> Card::remove(std::vector<Card*> cards){
    cards.erase(element_position(this, cards) + cards.begin());
    return cards;
}

