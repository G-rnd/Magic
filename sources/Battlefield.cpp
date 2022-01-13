#include <iostream>
#include <algorithm>
#include <string>

#include "../includes/Battlefield.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Creature.hpp"
#include "../includes/Land.hpp"
#include "../includes/Enchantment.hpp"

#include "../includes/FonctionsAux.hpp"

Battlefield::Battlefield() {
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard*> Battlefield::get_basic_cards() const{
    return m_basic_cards;
}

std::vector<Enchantment> Battlefield::get_enchantments() const{
    return m_enchantments;
}

void Battlefield::set_basic_cards(std::vector<BasicCard*> bc){
    // TODO : à verifier que ça fonctionne
    m_basic_cards = bc;
}

void Battlefield::set_enchantments(std::vector<Enchantment> e){
    // TODO : à verifier que ça fonctionne
    m_enchantments = e;
}

std::vector<Creature> Battlefield::get_engaged_creatures() {
    std::vector<Creature> v = {};

    for (auto card : m_basic_cards) {
        if(instanceof<Creature>(card) && card->get_engaged()) {
            v.push_back(*dynamic_cast<Creature*>(card));
        }
    }
    
    return v;
}

std::vector<Creature> Battlefield::get_available_creatures() {
    std::vector<Creature> v = {};

    for (auto card : m_basic_cards) {
        if(instanceof<Creature>(card) && !card->get_engaged()){
            Creature c = *dynamic_cast<Creature*>(card);
            if(!c.get_is_first_turn()){
                v.push_back(c);
            }
        }
    }

    return v;
}
std::vector<Land> Battlefield::get_available_lands() {
    std::vector<Land> v = {};
    
    for (auto card : m_basic_cards) {
        if(instanceof<Land>(card) && !card->get_engaged()){
            v.push_back(*dynamic_cast<Land*>(card));
        }
    }
    
    return v;
}

void Battlefield::disengage_card(BasicCard* bc){
    bc->set_engaged(false);
}

void Battlefield::print() {
    //TODO : print
}