#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"

#include "FonctionsAux.hpp"

Card::Card(int c, std::string name, int token): m_class(c), m_name(name), m_token(token) {
    //std::cout << "[Card] : Création de " << this << std::endl;
}

Card::~Card() {
    //std::cout << "[Card] : Destruction de " << this << std::endl;

}

/*
    Retourne l'attribut m_class de Card
*/
int Card::get_class() const {
    return m_class;
}

/*
    Retourne l'attribut m_name de Card
*/
std::string Card::get_name() const {
    return m_name;
}

/*
    Retourne l'attribut m_name de Card
*/
int Card::get_token() const {
    return m_token;
}

/*
    Met à jour l'attribut m_name de Card
*/
void Card::set_name(std::string name) {
    m_name = name;
}

/*
    Met à jour l'attribut m_token de Card
*/
void Card::set_token(int token) {
    m_token = token;
}

/*
    Retourne vrai si la carte est de type cl
*/
bool Card::is_class(int cl) const {
    return this->get_class() == cl;
}

/*
    Retire la carte d'une liste de carte si elle est présente.
*/
std::vector<Card*> Card::remove(std::vector<Card*> cards){
    cards.erase(element_position(this, cards) + cards.begin());
    return cards;
}
