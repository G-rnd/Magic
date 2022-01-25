#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Creature.hpp"
#include "BasicCard.hpp"
#include "Enchantment.hpp"
#include "Land.hpp"
#include "Cost.hpp"
#include "SaveParser.hpp"
#include "FonctionsAux.hpp"

Creature::Creature(int c, std::string name, int token, int power, int toughness, std::vector<int> abilities, std::vector<int> types, Cost* cost): 
Card(c, name, token), BasicCard(c, name, token), m_power(power), m_toughness(toughness), m_power_current(power), m_toughness_current(toughness), m_abilities(abilities), m_types(types), m_cost(cost) {
    //std::cout << "[Creature] : Création de " << this << std::endl;
    m_is_first_turn = false;
}

Creature::~Creature() {
    //std::cout << "[Creature] : Destruction de " << this << std::endl;
}

int Creature::get_power() const {
    return m_power;
}

int Creature::get_toughness() const {
    return m_toughness;
}

int Creature::get_power_current() const {
    return m_power_current;
}

int Creature::get_toughness_current() const {
    return m_toughness_current;
}

std::vector<int> Creature::get_abilities() const {
    return m_abilities;
}

std::vector<int> Creature::get_types() const {
    return m_types;
}

Cost* Creature::get_cost() const {
    return m_cost;
}

bool Creature::get_is_first_turn() const {
    return m_is_first_turn;
}

void Creature::set_power(int power) {
    m_power = power;
}

void Creature::set_toughness(int toughness) {
    m_toughness = toughness;
}

void Creature::set_power_current(int power_c) {
    m_power_current = power_c;
}

void Creature::set_toughness_current(int toughness_c) {
    m_toughness_current = toughness_c;
}

void Creature::set_is_first_turn(bool is_first_turn) {
    m_is_first_turn = is_first_turn;
}

void Creature::add_ability(int ability) {
    if(std::find(m_abilities.begin(), m_abilities.end(), ability) == m_abilities.end()) {
        m_abilities.push_back(ability);
    }
}

void Creature::add_type(int type) {
    if(std::find(m_types.begin(), m_types.end(), type) == m_types.end()) {
        m_types.push_back(type);
    }
}

std::string Creature::to_string() {
    std::string s = "";
    s += SaveParser::begin_card + "\n";
    s += SaveParser::classcard + std::to_string(Card_class::CREATURE) + "\n";
    s += SaveParser::name + get_name() + "\n";
    s += SaveParser::token + std::to_string(get_token()) + "\n";
    s += SaveParser::engaged + (get_engaged() ? "1" : "0") + "\n";

    s += SaveParser::begin_enchantments + "\n";
    for(auto e : get_enchantments())
        s += e->to_string();
    s += SaveParser::end_enchantments + "\n";
    
    s += SaveParser::power_toughness + std::to_string(m_power) + ", " + std::to_string(m_toughness) + ", " + std::to_string(m_power_current) + ", " + std::to_string(m_toughness_current) + "\n";
    s += SaveParser::abilities + list_int_to_string(m_abilities) + "\n";
    s += SaveParser::types + list_int_to_string(m_types) + "\n";
    s += SaveParser::cost + m_cost->to_string() + "\n";
    s += SaveParser::first_turn + (m_is_first_turn ? "1" : "0") + "\n";
    s += SaveParser::end_card + "\n";

    return s;
}

void Creature::print(){
    print_info("Voici les information de " + get_name() + " :");

    std::string token;
    switch (get_token())
    {
    case 0:
        token = get_background_color(Color::White);
    break;
    case 1:
        token = get_background_color(Color::Blue);
    break;
    case 2:
        token = get_background_color(Color::Black);
    break;
    case 3:
        token = get_background_color(Color::Red);
    break;
    case 4:
        token = get_background_color(Color::Green);
    break;
    default:
        break;
    }

    std::string str_types = "";
    std::vector<std::string> types{"Angel", "Beast", "Human_werewolf", "HippoGriff", "Kor_warrior", "Dinosaur", "Humans", "Vampire", "Spider", "Elf", "Troll"}; // TODO : mettre à jour
    for (size_t i = 0; i < m_types.size(); i++){
        if(i == m_types.size() - 1){
            str_types += types[m_types[i]];
        } else {
            str_types += types[m_types[i]] + ", ";
        }
    }

    std::string str_abilities;
    if(m_abilities.empty()){
        str_abilities = " n'a pas de capacités";
    } else{
        str_abilities = " a les capacités suivantes : ";
        std::string abilities[] = {"Flight", "Scope", "Vigilance", "Touch_of_death", "Defender", "Initiative", "Double_initiative", "Haste", 
                "Unblockable", "Life_link", "Threat", "Trampling", "White_protection", "Blue_protection", "Black_protection", "Red_protection", "Green_protection"};
        for (size_t i = 0; i < m_abilities.size(); i++){
            if(i == m_abilities.size() - 1){
                str_abilities += abilities[m_abilities[i]];
            } else {
                str_abilities += abilities[m_abilities[i]] + ", ";
            }
        }
    }

    if(get_token() == 0){
        std::cout << get_color(Color::Black) << token << get_name() << " est une créature de type(s) " << str_types << ". ";
        std::cout<< get_name() << " vous coûte ";
        m_cost->print();
        std::cout << " elle a une force initiale de " << m_power << " et une endurance initiale de " << m_toughness << "." <<std::endl;
        std::cout << "En ce moment elle est dans l'état suivant : " << m_power_current << " de force et " << m_toughness_current << " d'endurance. ";
        std::cout << get_name() << str_abilities << "." << get_color(Color::Reset) << get_background_color(Color::Reset)<< std::endl;
    } else {
        std::cout <<  token << get_name() << " est une créature de type(s) " << str_types << ". ";
        std::cout<< get_name() << " vous coûte ";
        m_cost->print();
        std::cout << " elle a une force initiale de " << m_power << " et une endurance initiale de " << m_toughness << "." <<std::endl;
        std::cout << "En ce moment elle est dans l'état suivant : " << m_power_current << " de force et " << m_toughness_current << " d'endurance. ";
        std::cout << get_name() << str_abilities << "." << get_background_color(Color::Reset)<< std::endl;
    }
}