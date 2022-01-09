#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../includes/Creature.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Enchantment.hpp"
#include "../includes/Land.hpp"
#include "../includes/Cost.hpp"

Creature::Creature(std::string name, int token, int power, int toughness, std::vector<int> abilities, std::vector<int> types, Cost* cost): 
Card(name, token), BasicCard(name, token), m_power(power), m_toughness(toughness), m_power_current(power), m_toughness_current(toughness), m_abilities(abilities), m_types(types), m_cost(cost){
    std::cout << "[Creature] : Création de " << this << std::endl;
    m_is_first_turn = false;
}

Creature::~Creature(){
    std::cout << "[Creature] : Destruction de " << this << std::endl;
}

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

bool Creature::get_is_first_turn() const{
    return m_is_first_turn;
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

void Creature::set_is_first_turn(bool is_first_turn){
    this->m_is_first_turn = is_first_turn;
}

void Creature::add_ability(int ability){
    if(std::find(this->m_abilities.begin(), this->m_abilities.end(), ability) == this->m_abilities.end()){
        m_abilities.push_back(ability);
    }
}

void Creature::add_type(int type){
    if(std::find(this->m_types.begin(), this->m_types.end(), type) == this->m_types.end()){
        m_types.push_back(type);
    }
}

void Creature::engage_lands(Vector<Land> lands) const{
    for (int i = 0; i < lands.size(); i++){
        lands[i].set_engaged(true);
    }
}

void Creature::print(){

}