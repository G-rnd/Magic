#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Creature.hpp"
#include "BasicCard.hpp"
#include "../SpecialCards/Enchantment.hpp"
#include "Land.hpp"
#include "../../Cost.hpp"

Creature::Creature(std::string name, int token, bool engaged, bool is_first_turn, int power, int toughness, std::vector<int> abilities, std::vector<int> types, Cost* cost): 
BasicCard(name, token, engaged, is_first_turn), m_power(power), m_toughness(toughness), m_power_current(power), m_toughness_current(toughness), m_abilities(abilities), m_types(types), m_cost(cost){}

Creature::~Creature(){}

int Creature::get_power() const{
    return this->m_power;
}

int Creature::get_toughness() const{
    return this->m_toughness;
}

int Creature::get_power_current() const{
    return this->m_power_current;
}

int Creature::get_toughness_current() const{
    return this->m_toughness_current;
}

std::vector<int> Creature::get_abilities() const{
    return this->m_abilities;
}

std::vector<int> Creature::get_types() const{
    return this->m_types;
}

Cost* Creature::get_cost() const{
    return this->m_cost;
}

void Creature::set_power(int power){
    this->m_power = power;
}

void Creature::set_toughness(int toughness){
    this->m_toughness = toughness;
}

void Creature::set_power_current(int power_c){
    this->m_power_current = power_c;
}

void Creature::set_toughness_current(int toughness_c){
    this->m_toughness_current = toughness_c;
}

void Creature::add_ability(int ability){
    if(std::find(this->m_abilities.begin(), this->m_abilities.end(), ability) == this->m_abilities.end()){
        this->m_abilities.push_back(ability);
    }
}

void Creature::add_type(int type){
    if(std::find(this->m_types.begin(), this->m_types.end(), type) == this->m_types.end()){
        this->m_types.push_back(type);
    }
}

void Creature::engage_lands(std::vector<Land*> lands) const{
    for (auto land : lands){
        land->set_engaged(true);
    }   
}