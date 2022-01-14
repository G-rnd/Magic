#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iomanip> 

#include "../includes/Game.hpp"
#include "../includes/Player.hpp"
#include "CardParser.hpp"
#include "FonctionsAux.cpp"


#include "../includes/FonctionsAux.hpp"

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
    m_player_turn = !m_player_turn;
}

std::vector<Player*> Game::get_players() const {
    return m_players;
}

Player* Game::get_first_player() const {
    return m_players[0];
}

Player* Game::get_second_player() const {
    return m_players[1];
}

Player* Game::get_current_player() const {
    return m_players[m_player_turn];
}

void Game::start() {
    std::string p_name_1;
    std::string p_name_2;

    std::cout<< "Bienvenue dans votre partie : "<<std::endl;

    std::cout<< "Le nom du premier joueur : "<<std::endl;
    std::cin>> p_name_1;
    Player p1(p_name_1);

    std::cout<< "Le nom du second joueur : "<<std::endl;
    std::cin>> p_name_2;
    Player p2(p_name_2);

    p1.set_opponent(&p2);
    p2.set_opponent(&p1);

    m_players.push_back(&p1);
    m_players.push_back(&p2);

    // Choix des decks
    for(int i = 0; i < 2; i++) {
        if (i == 0)
            std::cout << "Choisir un deck pour le premier joueur :" << std::endl;
        if (i == 1)
            std::cout << "Choisir un deck pour le second joueur :" << std::endl;
        
        std::string path = "data/";
        
        int nb_decks = 0;
        for (const auto & file : std::filesystem::directory_iterator(path)) {
            std::cout << file.path() << std::endl;
            nb_decks++;
        }        
        if (nb_decks == 0) {
            std::cout << "Erreur: Aucun deck n'est disponible !" << std::endl;
            return;
        }

        std::string filename = "";
        bool ok = false;

        while(!ok) {
            std::cin >> filename;
            std::ifstream ifile;
            ifile.open(filename);

            if(ifile) {
                m_players[i]->set_library(CardParser::parse(filename));
                ok = true;
            }
        }
        // Phase de pioche initiale
        for(auto index = 0; index < 7; index++) {
            m_players[i]->draw_card();
        }
    }
    // Phases de jeu
    while (true) {
        Player* p = get_current_player();

        // reset des stats des cartes 
        for (auto p : m_players) {
            for (auto bc : p->get_battlefield().get_basic_cards()) {
                if (instanceof<Creature*>(bc)) {
                    Creature c = *(dynamic_cast<Creature*>(bc));
                    c.set_power_current(c.get_power());
                    c.set_toughness_current(c.get_toughness());
                }
            }
        }
        std::cout << "A" << std::endl;

        // Phase de pioche;
                
        if (p->get_library().size() == 0) {
            std::cout << "B1" << std::endl;
            victory(*m_players[!m_player_turn]);
            return;
        } else {
            std::cout << "B2" << std::endl;
            std::cout << "Vous piochez la carte : " << p->get_library()[0]->get_name() << std::endl;
            p->draw_card();
        }
        std::cout << "B" << std::endl;
        
        // Phase de désengagement
        for(auto c : p->get_battlefield().get_basic_cards()) {
            p->get_battlefield().disengage_card(c);
        }

        // Phase principale
        Game::main_phase(*p);
        
        // PHASE DE COMBAT
        


        m_player_turn = !m_player_turn;

    
        break;
    }

    // TODO : à compléter game::start()

}


void Game::main_phase(Player p) {
    while(true) {
        std::system("clear");
        std::cout << "Selectionnez une carte à placer :" << std::endl;
        std::cout << "<id>      : pour jouer une carte." << std::endl;
        std::cout << "info <id> : pour avoir des informations sur une carte." << std::endl;
        std::cout << "end       : pour arrêter la phase principale." << std::endl << std::endl;

        std::vector<Card*> hand;
        for(auto c : p.get_hand())
            hand.push_back(c);
        int hand_size = hand.size();

        // Vérifications 
        for(int i = 0; i < hand_size; i++) {
            // Une carte est jouable si:
            // 
            // TODO modifier le played_land
            if (instanceof<Creature>(hand[i]) && p.get_battlefield().is_playable(*dynamic_cast<Creature*>(hand[i])) || instanceof<Land>(hand[i]) && !p.get_played_land() || instanceof<SpecialCard>(hand[i]))
                std::cout << std::setfill(' ') << std::setw (hand_size / 10)  << i << " - " << hand[i]->get_name() << std::endl;
            else {
                hand.erase(hand.begin() + i);
                i--;
            }
        }

        std::string cmd;
        std::cin >> cmd;
        if (cmd.find("end") != std::string::npos)
            return;
        else if (cmd.find("info ") != std::string::npos) {
            // info sur une carte
            int num;
            try {
                // 5: taille de "info "
                num = std::stoi(cmd.substr(5));
                    
                if (num > hand_size)
                    std::cout << "Id invalide" << std::endl;
                else {
                    p.get_hand()[num]->print();
                }            
            }            
            catch (std::invalid_argument &e) {
                std::cout << "Id invalide" << std::endl;
            }        
        } else {
            // selection d'une carte
            int num;
            try {
                num = std::stoi(cmd);
                    
                if (num > hand_size)
                    std::cout << "Id invalide" << std::endl;
                else {
                    Card* c = hand[num];
                    // Joue la carte
                    p.play_card(hand[num]);
                }
            }            
            catch (std::invalid_argument &e) {
                std::cout << "Id invalide" << std::endl;
            }
        }
        std::cout << "Entrée pour continuer." << std::endl;
        std::cin.ignore(32767,'\n');
    }
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

void Game::victory(Player p) {
    // TODO
}
