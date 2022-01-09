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
    Player p1(this, p_name_1);

    std::cout<< "Le nom du second joueur : "<<std::endl;
    std::cin>> p_name_2;
    Player p2(this, p_name_2);

    m_players.push_back(p1);
    m_players.push_back(p2);
    
    // TODO : à compléter game::start()

}

void Game::choose_name(Player p, std::string name) {
    // TODO
}

void Game::choose_deck() {
    // TODO
}

void Game::save() {
    // TODO
}

void Game::choose_save() {
    // TODO
}

void Game::load(std::string filename) {
    // TODO
}

void Game::exit() {
    // TODO
}

void Game::end(Player looser) {
    
    if(looser.get_name() == m_players[0].get_name()){
        std::cout<< m_players[1].get_name() << " est victorieux ! "<<std::endl;
    } else {
        std::cout<< m_players[0].get_name() << " est victorieux ! "<<std::endl;
    }

    // TODO : fin du jeu
    
}
