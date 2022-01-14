#include <iostream>
#include <algorithm>
#include <string>

#include "../includes/Battlefield.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Creature.hpp"
#include "../includes/Land.hpp"
#include "../includes/Enchantment.hpp"

#include "FonctionsAux.cpp"

Battlefield::Battlefield() {
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard*> Battlefield::get_basic_cards() const{
    return m_basic_cards;
}

std::vector<Enchantment> Battlefield::get_enchantments() const{
    return m_enchantments;
}

void Battlefield::set_basic_cards(std::vector<BasicCard*> bc){
    // TODO : à verifier que ça fonctionne
    m_basic_cards = bc;
}

void Battlefield::set_enchantments(std::vector<Enchantment> e){
    // TODO : à verifier que ça fonctionne
    m_enchantments = e;
}

std::vector<Creature> Battlefield::get_engaged_creatures() {
    std::vector<Creature> v = {};

    for (auto card : m_basic_cards) {
        if(instanceof<Creature>(card) && card->get_engaged()) {
            v.push_back(*dynamic_cast<Creature*>(card));
        }
    }
    
    return v;
}

std::vector<Creature> Battlefield::get_available_creatures() {
    std::vector<Creature> v = {};

    for (auto card : m_basic_cards) {
        if(instanceof<Creature>(card) && !card->get_engaged()){
            Creature c = *dynamic_cast<Creature*>(card);
            if(!c.get_is_first_turn()){
                v.push_back(c);
            }
        }
    }

    return v;
}
std::vector<Land> Battlefield::get_available_lands() {
    std::vector<Land> v = {};
    
    for (auto card : m_basic_cards) {
        if(instanceof<Land>(card) && !card->get_engaged()){
            v.push_back(*dynamic_cast<Land*>(card));
        }
    }
    
    return v;
}

void Battlefield::place_basic_card(BasicCard* bc) {
    if(bc != nullptr)
        m_basic_cards.push_back(bc);
}

void Battlefield::disengage_card(BasicCard* bc){
    bc->set_engaged(false);
}

// Renvoie vrai si le terrain comporte assez de cartes terrains pour jouer la carte c
bool Battlefield::is_playable(Creature creature) {
    bool res = false;
    Cost c = Cost(*(creature.get_cost()));
    std::vector<Land> lands = get_available_lands();
    for(int i = 0; i < lands.size() && !c.is_null(); i++) {
        // Si la carte n'a pas besoin de terrains typés
        if (c.is_color_null()) {
            c.set_any(c.get_any() - lands[i].get_value());
            lands.erase(lands.begin() + i);
            i--;
        } else {
            // TODO changer les == 0 par == Color::white etc

            // On commence par compter les terrains typés
            if (lands[i].get_token() == 0) {
                c.set_white(c.get_white() - lands[i].get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i].get_token() == 1) {
                c.set_black(c.get_black() - lands[i].get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i].get_token() == 2) {
                c.set_red(c.get_red() - lands[i].get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i].get_token() == 3) {
                c.set_green(c.get_green() - lands[i].get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i].get_token() == 4) {
                c.set_blue(c.get_blue() - lands[i].get_value());
                lands.erase(lands.begin() + i);
            }
        }
    }
    return c.is_null();
}

void Battlefield::print() {
    //TODO : print
}
