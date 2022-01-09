#include <iostream>
#include <vector>
#include <string>

#include "Battlefield.hpp"
#include "Cards/BasicCards/BasicCard.hpp"
#include "Cards/BasicCards/Creature.hpp"
#include "Cards/BasicCards/Land.hpp"
#include "Cards/SpecialCards/Enchantment.hpp"
#include "FonctionsAux.cpp"

Battlefield::Battlefield() {
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard> Battlefield::get_basic_cards() const{
    return m_basic_cards;
}
std::vector<Enchantment> Battlefield::get_enchantments() const{
    return m_enchantments;
}

std::vector<Creature> Battlefield::get_engaged_creatures() const{
    std::vector<Creature> v = {};
    for (auto bc : m_basic_cards){
        if(instanceof<Creature>(&bc) && bc.get_engaged()){
            v.push_back(*dynamic_cast<Creature*>(&bc));
        }
    }
    return v;
}
std::vector<Creature> Battlefield::get_available_creatures() const{
    std::vector<Creature> v = {};
    for (auto bc : m_basic_cards){
        if(instanceof<Creature>(&bc) && !bc.get_engaged()){
            Creature c = *dynamic_cast<Creature*>(&bc);
            if(!c.get_is_first_turn()){
                v.push_back(*dynamic_cast<Creature*>(&bc));
            }
        }
    }
    return v;
}
std::vector<Land> Battlefield::get_available_lands() const{
    std::vector<Land> v = {};
    for (auto bc : m_basic_cards){
        if(instanceof<Land>(&bc) && !bc.get_engaged()){
            v.push_back(*dynamic_cast<Land*>(&bc));
        }
    }
    return v;
}

void Battlefield::add_basic_card(BasicCard &bc){
    m_basic_cards.push_back(bc);
}

void Battlefield::add_enchantment(Enchantment &e){
    m_enchantments.push_back(e);
}

void Battlefield::disengage_card(BasicCard &bc) {
    bc.set_engaged(false);
}

void Battlefield::print() {

}