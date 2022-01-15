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
    return m_player_turn ? get_second_player() : get_first_player();
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

        std::string filename = "data/deck.txt";
        bool ok = false;

        while(!ok) {
            //std::cin >> filename;
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

        // Phase de pioche;
        if (get_current_player()->get_library().size() == 0) {
            victory(*m_players[!m_player_turn]);
            return;
        } else {
            std::cout << "Vous piochez la carte : " << get_current_player()->get_library()[0]->get_name() << std::endl;
            get_current_player()->draw_card();

            std::cout << "Entrée pour continuer." << std::endl;
            std::cin.get(); 
        }

        // Phase de désengagement
        for(auto c : get_current_player()->get_battlefield().get_basic_cards()) {
            get_current_player()->get_battlefield().disengage_card(c);
        }

        // Phase principale
        Game::main_phase();
        
        // PHASE DE COMBAT

        
        // PHASE DE COMBAT
        
        std::vector<Creature*> chosen_opponent =  p->attack();
        std::vector<Creature*> chosen_blockabled_opponent = p->attack();

        // supprimer les cartes imblocables des cartes blocables
        for (auto creature : chosen_blockabled_opponent){

            bool unblockable_opponent = false;

            for (auto ability_creature : creature->get_abilities()){
                if(ability_creature == Ability::Unblockable) unblockable_opponent = true;
            }

            if(unblockable_opponent) remove(creature, chosen_blockabled_opponent);

        }

        // demander à l'adervsaire s'il veut défendre l'attaque
        std::cout<< get_current_player()->get_opponent()->get_name() << ", voulez-vous bloquer ces créatures ? oui / non"<<std::endl;
        int i = 1;
        for (auto creature : chosen_blockabled_opponent){
            std::cout<< i <<" - "<< creature->get_name()<<std::endl;            
        }

        bool quit = false;
        std::string res;
        while(!quit){
            std::cin>> res;
            if(res == "oui"){
                get_current_player()->get_opponent()->choose_defenders(chosen_blockabled_opponent);
                quit = true;
            } else if(res == "non"){
                
                // directement attaquer l'adervsaire
                for (auto creature : chosen_blockabled_opponent){
                    get_current_player()->get_opponent()->set_hp(get_current_player()->get_opponent()->get_hp() - creature->get_power_current());
                }

            } else{
                std::cout << "Commande Invalide" << std::endl;
            }

        }
      
        // PHASE SECONDAIRE
        Game::main_phase();

      
        // FIN DE TOUR

        while(get_current_player()->get_hand().size() > 7){

            std::cout<<"Défaussez des cartes, il doit vous rester 7"<<std::endl;
            
            int i = 1;
            std::vector<Card*> possible_cards;
            std::vector<Card*> chosen_cards;

            for (auto card : p->get_hand()){
                std::cout<< i << " - " << card->get_name()<<std::endl;
                possible_cards.push_back(card);
                i++;
            }

        }
      
        m_player_turn = !m_player_turn;

    // TODO à envelver
        break;
    }
}


void Game::main_phase() {
    while(true) {
        std::system("clear");
        std::cout << "Selectionnez une carte à placer :" << std::endl;
        std::cout << "<id>      : pour jouer une carte." << std::endl;
        std::cout << "info <id> : pour avoir des informations sur une carte." << std::endl;
        std::cout << "end       : pour arrêter la phase principale." << std::endl << std::endl;

        std::vector<Card*> hand{};
        for(Card* c : get_current_player()->get_hand()) {
            hand.push_back(c);
        }
        int hand_size = hand.size();
        
        // Vérifications 
        for(int i = 0; i < hand_size; i++) {
            // Une carte est jouable si:
            if(hand[i]->is_class(Card_class::CREATURE) && p->get_battlefield().is_playable(hand[i]) || hand[i]->is_class(Card_class::LAND) && (p->get_played_land() <= 0)
                ||  hand[i]->is_class(Card_class::RITUAL) && p->get_battlefield().is_playable(hand[i]) || hand[i]->is_class(Card_class::ENCHANTEMENT) && p->get_battlefield().is_playable(hand[i]) ) {
                std::cout << std::setfill(' ') << std::setw (hand_size / 10)  << i << " - " << hand[i]->get_name() << std::endl;

            } else {
                hand.erase(hand.begin() + i);
                i--;
                hand_size--;        
            }
        }

        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd.find("end") != std::string::npos)
            return;
        else {
            try {
                int num = std::stoi(cmd);
                if (num >= hand_size) {
                    std::cout << "Id invalide" << std::endl;

                    std::cout << "Entrée pour continuer." << std::endl;
                    std::getline(std::cin, cmd);
                }
                else
                    // todo à vérifier si ça marche bien quand il y aura la nouvelle version des instanceof
                    get_current_player()->play_card(hand[num]);
            }
            catch (std::invalid_argument &e) {
                // TODO faire un truc plus propre
                std::string info = "info ";
                if (std::mismatch(info.begin(), info.end(), cmd.begin()).first == info.end()) {
                    try {
                        int num = std::stoi(cmd.substr(5));
                        if (num >= hand_size) {
                            std::cout << "Id invalide" << std::endl;
                            std::cout << "Entrée pour continuer." << std::endl;
                            std::getline(std::cin, cmd);
                        } else {
                            hand[num]->print();
                            std::cout << "Entrée pour continuer." << std::endl;
                            std::getline(std::cin, cmd);
                        }            
                    }            
                    catch (std::invalid_argument &e) {
                        std::cout << "Id invalide" << std::endl;
                    }  
                } else
                    std::cout << "Commande Invalide" << std::endl;
            } 
        }
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
