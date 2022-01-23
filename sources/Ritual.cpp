#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Ritual.hpp"
#include "FonctionsAux.hpp"
#include "SaveParser.hpp"

Ritual::Ritual(int c, std::string name, int token, Cost* cost, std::vector<int> effects): 
Card(c, name, token), SpecialCard(c, name, token, cost, effects) {
    //std::cout << "[Ritual] : Création de " << this << std::endl;
}

Ritual::~Ritual() {
    //std::cout << "[Ritual] : Destruction de " << this << std::endl;
}

void Ritual::print() {
    // TODO : print
}

std::string Ritual::to_string() {
    std::string s = "";

    s += SaveParser::begin_card + "\n";
    s += SaveParser::classcard + std::to_string(Card_class::RITUAL) + "\n";
    s += SaveParser::name + get_name() + "\n";
    s += SaveParser::token + std::to_string(get_token()) + "\n";
    s += SaveParser::effects + list_int_to_string(get_effects()) + "\n";
    s += SaveParser::cost + get_cost()->to_string() + "\n";
    s += SaveParser::end_card + "\n";
    
    return s;
}