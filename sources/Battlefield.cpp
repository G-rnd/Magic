#include <iostream>
#include <string>

#include "../includes/Battlefield.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Creature.hpp"
#include "../includes/Land.hpp"
#include "../includes/Enchantment.hpp"

#include "FonctionsAux.cpp"

Battlefield::Battlefield() {
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard*> Battlefield::get_basic_cards() {
    return m_basic_cards;
}

std::vector<Enchantment> Battlefield::get_enchantments() {
    return m_enchantments;
}

std::vector<Creature> Battlefield::get_engaged_creatures() {
    std::vector<Creature> v = {};
    /*
    
    for (int i = 0; i < m_basic_cards.size(); i++) {
        if(instanceof<Creature>(m_basic_cards[i]) && m_basic_cards[i].get_engaged()) {
            v.push_back(m_basic_cards[i]);
        }
    }
    */
    return v;
}

std::vector<Creature> Battlefield::get_available_creatures() {
    std::vector<Creature> v = {};
    /*
    
    for (int i = 0; i < m_basic_cards.size(); i++) {
        if(instanceof<Creature>(m_basic_cards[i]) && !m_basic_cards[i].get_engaged()){
            Creature c = m_basic_cards[i];
            if(!c.get_is_first_turn()){
                v.push_back(c);
            }
        }
    }
    */
    return v;
}
std::vector<Land> Battlefield::get_available_lands() {
    std::vector<Land> v = {};
    /*
    for (int i = 0; i < m_basic_cards.size(); i++) {
        if(instanceof<Land>(m_basic_cards[i]) && !m_basic_cards[i].get_engaged()){
            v.push_back(m_basic_cards[i]);
        }
    }
    */
   return v;
}

void Battlefield::print() {

}