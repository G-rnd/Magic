#include <iostream>
#include <string>

#include "../includes/Player.hpp"
#include "../includes/Card.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Creature.hpp"
#include "../includes/Battlefield.hpp"
#include "../includes/Ritual.hpp"
#include "../includes/Enchantment.hpp"

#include "FonctionsAux.cpp"

Player::Player(Game* game, std::string name): m_game(game), m_name(name) {
    std::cout << "[Player] : Création de " << this << std::endl;
    m_hp = 20;
}

Player::~Player() {
    std::cout << "[Player] : Denstruction de " << this << std::endl;
}

std::string Player::get_name() const {
    return m_name;
}

int Player::get_hp() const {
    return m_hp;
}

bool Player::get_played_land() const {
    return m_played_land;
}

Battlefield Player::get_battlefield() const {
    return m_battlefield;
}

std::vector<Card*> Player::get_deck() const {
    return m_deck;
}

std::vector<Card*> Player::get_graveyard() const {
    return m_graveyard;
}

std::vector<Card*> Player::get_library() const {
    return m_library;
}

std::vector<Card*> Player::get_hand() const {
    return m_hand;
}

void Player::set_name(std::string s) {
    m_name = s;
}

void Player::set_ph(int i) {
    m_hp = i;
}

void Player::set_played_land(bool b) {
    m_played_land = b;
}

void Player::draw_card() {

    if(m_library.empty()){
        this->loose();
    } else{
        m_hand.push_back(*m_library.begin());
        m_library.erase(element_position(*m_library.begin(), m_library));
    }

}

void Player::discard_card(Card* c) {
    m_hand.erase(element_position(c, m_graveyard));
    m_graveyard.push_back(c);
}

void Player::shuffle_library() {

    std::vector<Card*> m_library_copy = m_library;
    int i_lib = 0;
    while(!m_library_copy.empty()){
        int j = rand() % m_library_copy.size();
        m_library[i_lib] = m_library_copy[j];
        m_library_copy.erase(m_library_copy.begin() + j);
        i_lib++;
    }

}

void Player::play_card(Card* c) {
    // TODO : à compléter player::play_card()

    if(instanceof<Creature>(c)){
        
    } else if(instanceof<Land>(c)){

    } else if(instanceof<Ritual>(c)){

    } else if(instanceof<Enchantment>(c)){

    }
}

void Player::engage_card(BasicCard* bc) {
    bc->set_engaged(true);
}

void Player::disengage_card(BasicCard* bc) {
    bc->set_engaged(false);
}

void Player::attack(std::vector<Creature> c) {

    for (auto card : c){
        if(!(card.get_engaged() && card.get_is_first_turn())){
            card.set_engaged(true);
        } else {
            // TODO : erreur
        }
    }

}

// TODO : /!\ entrer dans defenders les creature dans l'ordre choisi par celui qui attaque
void Player::deflect_attack(Creature opponent, std::vector<Creature> defenders) {

    for (int i = 0; i <= defenders.size(); i++){
        // Check if the opponent is already dead
        if(contain(dynamic_cast<BasicCard*>(&opponent), m_battlefield.get_basic_cards())){
            battle_creature(opponent, defenders[i]);
        }
    }

}

void Player::battle_creature(Creature opponent, Creature defender) {

    // Count the damages
    opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
    defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());

    // If the creatures are dead, deplace them into the graveyard
    if(opponent.get_toughness_current() <= 0){
        destroy_card(dynamic_cast<Card*>(&opponent));
    }
    if(defender.get_toughness_current() <= 0){
        destroy_card(dynamic_cast<Card*>(&defender));
    }
}

void Player::destroy_card(Card* c) {

    // If c is a BasicCard, we deplace it into the graveyard
    if(instanceof<BasicCard*>(c)){
        m_battlefield.set_basic_cards(dynamic_cast<BasicCard*>(c)->remove(m_battlefield.get_basic_cards()));
        m_graveyard.push_back(c);
        // We also deplace the enchantments associated to c
        for (auto e : (dynamic_cast<BasicCard*>(c))->get_enchantments()){
            m_graveyard.push_back(&e);
        }
    } 
    // If c is a Ritual, we place it into the graveyard
    else if(instanceof<Ritual>(c)){
        m_graveyard.push_back(c);
    }

}

void Player::loose() {
    m_game->end(*this);
}

void Player::print() const{
    // TODO : print
}
