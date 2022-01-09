#include <iostream>
#include <string>

#include "Battlefield.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"
#include "Vector.hpp"

Battlefield::Battlefield() {
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

Vector<BasicCard> Battlefield::get_basic_cards() {
    return m_basic_card;
}
Vector<Enchantment> Battlefield::get_enchantments() {
    return m_enchantments;
}

Vector<Creature> Battlefield::get_engaged_creatures() {
    return nullptr;
}
Vector<Creature> Battlefield::get_available_creatures() {
    return nullptr;
}
Vector<Land> Battlefield::get_available_lands() {
    return nullptr;
}

void Battlefield::print() {

}