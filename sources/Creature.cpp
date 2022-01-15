#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../includes/Creature.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Enchantment.hpp"
#include "../includes/Land.hpp"
#include "../includes/Cost.hpp"

Creature::Creature(int c, std::string name, int token, int power, int toughness, std::vector<int> abilities, std::vector<int> types, Cost* cost): 
Card(c, name, token), BasicCard(c, name, token), m_power(power), m_toughness(toughness), m_power_current(power), m_toughness_current(toughness), m_abilities(abilities), m_types(types), m_cost(cost){
    //std::cout << "[Creature] : Création de " << this << std::endl;
    m_is_first_turn = false;
}

Creature::~Creature(){
    //std::cout << "[Creature] : Destruction de " << this << std::endl;
}

int Creature::get_power() const{
    return m_power;
}

int Creature::get_toughness() const{
    return m_toughness;
}

int Creature::get_power_current() const{
    return m_power_current;
}

int Creature::get_toughness_current() const{
    return m_toughness_current;
}

std::vector<int> Creature::get_abilities() const{
    return m_abilities;
}

std::vector<int> Creature::get_types() const{
    return m_types;
}

Cost* Creature::get_cost() const{
    return m_cost;
}

bool Creature::get_is_first_turn() const{
    return m_is_first_turn;
}

void Creature::set_power(int power){
    m_power = power;
}

void Creature::set_toughness(int toughness){
    m_toughness = toughness;
}

void Creature::set_power_current(int power_c){
    m_power_current = power_c;
}

void Creature::set_toughness_current(int toughness_c){
    m_toughness_current = toughness_c;
}

void Creature::set_is_first_turn(bool is_first_turn){
    m_is_first_turn = is_first_turn;
}

void Creature::add_ability(int ability){
    if(std::find(m_abilities.begin(), m_abilities.end(), ability) == m_abilities.end()){
        m_abilities.push_back(ability);
    }
}

void Creature::add_type(int type){
    if(std::find(m_types.begin(), m_types.end(), type) == m_types.end()){
        m_types.push_back(type);
    }
}

void Creature::engage_lands(std::vector<Land*> lands) const{
    for (auto land : lands){
        land->set_engaged(true);
    }
}

void Creature::print(){
    // TODO : print
}