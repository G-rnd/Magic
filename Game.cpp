#include <iostream>

#include "Game.hpp"
#include "Player.hpp"

Game::Game() {
    std::cout << "[Game] : Construction de " << this << std::endl;
}

Game::~Game() {
    std::cout << "[Game] : Destruction de " << this << std::endl;
}
bool Game::get_player_turn() const {
    return m_player_turn;
}

void Game::set_player_turn(bool b) {
    m_player_turn = b;
}

void Game::next_player_turn() {
    m_player_turn != m_player_turn;
}

std::vector<Player> Game::get_players() const {
    return m_players;
}

std::vector<Player> Game::get_players() const {
    return m_players;
}

Player Game::get_first_player() const {
    return m_players[0];
}

Player Game::get_second_player() const {
    return m_players[1];
}

Player Game::get_current_player() const {
    return m_players[m_player_turn];
}

void Game::start() {
    
}

void Game::choose_name(Player p, std::string name) {
    
}

void Game::choose_deck() {
    
}

void Game::save() {
    
}

void Game::choose_save() {
    
}

void Game::load(std::string filename) {
    
}

void Game::exit() {
    
}

void Game::victory(Player p) {
    
}