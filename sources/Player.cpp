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

Player::Player() {
    std::cout << "[Player] : Création de " << this << std::endl;
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
    // TODO
    Card *c = *(m_library.begin());

}

void Player::discard_card(Card* c) {
    m_hand.erase(element_position(c, m_graveyard));
    m_graveyard.push_back(c);
}

void Player::shuffle_library() {
    // TODO
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
    // TODO
}

void Player::attack(Creature c) {
    // TODO
}

void Player::deflect_attack(Creature opponent, std::vector<Creature> defenders) {
    // TODO
}

void Player::battle_creature(Creature opponent, Creature defender) {
    // TODO
}

void Player::destroy_card(Card* c) {
    m_battlefield.remove_basic_card(dynamic_cast<BasicCard*>(c));
    m_graveyard.push_back(c);
}

void Player::loose() {
    // TODO
}

void Player::print() const{
    // TODO : print
}
