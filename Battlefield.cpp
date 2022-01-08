#include <iostream>
#include <vector>
#include <string>

#include "Battlefield.hpp"
#include "Cards/BasicCards/BasicCard.hpp"
#include "Cards/BasicCards/Creature.hpp"
#include "Cards/BasicCards/Land.hpp"
#include "Cards/SpecialCards/Enchantment.hpp"

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
    return std::vector<Creature> ();
}
std::vector<Creature> Battlefield::get_available_creatures() const{
    return std::vector<Creature> ();
}
std::vector<Land> Battlefield::get_available_lands() const{
    return std::vector<Land> ();
}

void Battlefield::print() {

}