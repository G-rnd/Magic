#include <iostream>
#include <vector>

#include "../includes/Game.hpp"
#include "../includes/Player.hpp"

Game::Game() {
    std::cout << "[Game] : Création de " << this << std::endl;
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
    std::string p_name_1;
    std::string p_name_2;

    std::cout<< "Bienvenue dans votre partie : "<<std::endl;

    std::cout<< "Le nom du premier joueur : "<<std::endl;
    std::cin>> p_name_1;
    Player p1;
    p1.set_name(p_name_1);
    p1.set_ph(20);

    std::cout<< "Le nom du second joueur : "<<std::endl;
    std::cin>> p_name_2;
    Player p2;
    p2.set_name(p_name_2);
    p2.set_ph(20);

    m_players.push_back(p1);
    m_players.push_back(p2);
    
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
