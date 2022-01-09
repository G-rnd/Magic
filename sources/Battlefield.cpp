#include <iostream>
#include <string>

#include "Battlefield.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"
#include "Vector.hpp"

#include "FonctionsAux.cpp"

Battlefield::Battlefield() {
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

Vector<BasicCard> Battlefield::get_basic_cards() {
    return m_basic_cards;
}

Vector<Enchantment> Battlefield::get_enchantments() {
    return m_enchantments;
}

Vector<Creature> Battlefield::get_engaged_creatures() {
    Vector<Creature> v = {};
    /*
    
    for (int i = 0; i < m_basic_cards.size(); i++) {
        if(instanceof<Creature>(m_basic_cards[i]) && m_basic_cards[i].get_engaged()) {
            v.push_back(m_basic_cards[i]);
        }
    }
    */
    return v;
}

Vector<Creature> Battlefield::get_available_creatures() {
    Vector<Creature> v = {};
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
Vector<Land> Battlefield::get_available_lands() {
    Vector<Land> v = {};
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