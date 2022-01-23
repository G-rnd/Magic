#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Enchantment.hpp"
#include "FonctionsAux.hpp"
#include "SaveParser.hpp"

Enchantment::Enchantment(int c, std::string name, int token, Cost* cost, std::vector<int> effects): 
Card(c, name, token), SpecialCard(c, name, token, cost, effects) {
    //std::cout << "[Enchantment] : Création de " << this << std::endl;
}

Enchantment::~Enchantment() {
    //std::cout << "[Enchantment] : Destruction de " << this << std::endl;
}

void Enchantment::print() {
    // TODO : print
}

std::string Enchantment::to_string() {
    std::string s = "";

    s += SaveParser::begin_card + "\n";
    s += SaveParser::classcard + std::to_string(Card_class::ENCHANTEMENT) + "\n";
    s += SaveParser::name + get_name() + "\n";
    s += SaveParser::token + std::to_string(get_token()) + "\n";
    s += SaveParser::effects + list_int_to_string(get_effects()) + "\n";
    s += get_cost()->to_string() + "\n";
    s += SaveParser::end_card + "\n";
    
    return s;
}

bool Enchantment::is_valid(size_t token, size_t id) {
    switch (token) {
        case Token::Black:
            return id < Black_enchantment_effects::Count;
        case Token::White:
            return id < White_enchantment_effects::Count;
        case Token::Blue:
            return id < Blue_enchantment_effects::Count;
        case Token::Red:
            return id < Red_enchantment_effects::Count;
        case Token::Green:
            return id < Green_enchantment_effects::Count;
    }
    return false;
}
