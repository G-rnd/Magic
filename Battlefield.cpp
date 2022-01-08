#include <iostream>
#include <vector>
#include <string>

#include "Battlefield.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"

Battlefield::Battlefield() {
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard> Battlefield::get_basic_cards() {
    return m_basic_card;
}
std::vector<Enchantment> Battlefield::get_enchantments() {
    return m_enchantments;
}

std::vector<Creature> Battlefield::get_engaged_creatures() {
    return nullptr;
}
std::vector<Creature> Battlefield::get_available_creatures() {
    return nullptr;
}
std::vector<Land> Battlefield::get_available_lands() {
    return nullptr;
}

void Battlefield::print() {

}