#include <iostream>
#include <string>

#include "Player.hpp"
#include "Card.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Battlefield.hpp"
#include "Vector.hpp"

Player::Player() {
    std::cout << "[Player] : Construction de " << this << std::endl;
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
Vector<Card> Player::get_deck() const {
    return m_deck;
}
Vector<Card> Player::get_graveyard() const {
    return m_graveyard;
}
Vector<Card> Player::get_library() const {
    return m_library;
}
Vector<Card> Player::get_hand() const {
    return m_hand;
}

Player::set_name(std::string s) {
    m_name = s;
}
Player::set_ph(int i) {
    m_hp = i;
}
Player::set_played_land(bool b) {
    m_played_land = b;
}

void Player::draw_card() {
    
}

void Player::discard_card(Card c) {
    
}

void Player::shuffle_library() {
    
}

void Player::play_card(Card c) {
    
}

void Player::engage_card(BasicCard bc) {
    bc->set_engaged(true);
}

void Player::disengage_card(BasicCard bc) {
    
}

void Player::attack(Creature c) {
    
}

void Player::deflect_attack(Creature opponent, Vector<Creature> defenders) {
    
}

void Player::battle_creature(Creature opponent, Creature defender) {
    
}

void Player::destroy_card(Card c) {
    
}

void Player::loose() {
    
}
