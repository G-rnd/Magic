#include <iostream>
#include <vector>
#include <string>

#include "BasicCard.hpp"
#include "Card.hpp"
#include "Enchantment.hpp"

#include "FonctionsAux.hpp"

BasicCard::BasicCard(int c, std::string name, int token): Card(c, name, token) {
    //std::cout << "[BasicCard] : Création de " << this << std::endl;
    m_engaged = false;
}

BasicCard::~BasicCard() {
    //std::cout << "[BasicCard] : Destruction de " << this << std::endl;
}

/*
    Retourne l'attribut m_engaged d'une BasicCard.
*/
bool BasicCard::get_engaged() const {
    return m_engaged;
}

/*
    Retourne l'attribut m_enchantments d'une BasicCard.
*/
std::vector<Enchantment*> BasicCard::get_enchantments() const {
    return m_enchantments;
}

/*
    Met à jour l'attribut m_engaged d'une BasicCard.
*/
void BasicCard::set_engaged(bool engaged) {
    m_engaged = engaged;
}

/*
    Ajoute l'enchantement e à l'attribut m_enchantments d'une BasicCard.
*/
void BasicCard::add_enchantment(Enchantment* e) {
    m_enchantments.push_back(e);
}

/*
    Vide l'attribut m_enchantments d'une BasicCard.
*/
void BasicCard::reset_enchantments() {
    m_enchantments = {};
}

/*
    Retire une BasicCard d'une liste de BasicCards.
*/
std::vector<BasicCard*> BasicCard::remove(std::vector<BasicCard*> cards) {
    cards.erase(element_position(this, cards) + cards.begin());
    return cards;
}
