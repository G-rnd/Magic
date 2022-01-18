#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iomanip> 

#include "Game.hpp"
#include "Player.hpp"
#include "CardParser.hpp"
#include "FonctionsAux.hpp"

Game::Game() {
    m_player_turn = false;
    //std::cout << "[Game] : Création de " << this << std::endl;
    m_player_turn = false;
}

Game::~Game() {
    //std::cout << "[Game] : Destruction de " << this << std::endl;
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

    std::cout << "Bienvenue dans votre partie." << std::endl << std::endl;;

    std::cout << "Le nom du premier joueur :" << std::endl;
    std::cin >> p_name_1;
    Player p1(p_name_1);

    std::cout << "Le nom du second joueur :" << std::endl;
    std::cin >> p_name_2;
    Player p2(p_name_2);

    p1.set_opponent(&p2);
    p2.set_opponent(&p1);

    m_players.push_back(&p1);
    m_players.push_back(&p2);

    // Choix des decks, mélange des bibliothèques et pioche initiale
    for (size_t i = 0; i < 2; i++) {
        switch (i) {
            case 0:
                std::cout << "Choisir un deck pour le premier joueur :" << std::endl;
                break;
            case 1:
                std::cout << "Choisir un deck pour le second joueur :" << std::endl;
                break;
            default:
                break;
        }
        
        std::string path = "data/";
        
        size_t nb_decks = 0;
        for (const auto & file : std::filesystem::directory_iterator(path)) {
            std::cout << file.path() << std::endl;
            nb_decks++;
        }        
        if (nb_decks == 0) {
            print_info("Erreur : Aucun deck n'est disponible !");
            return;
        }

        std::string filename = "";

        while(true) {
            std::cin >> filename;
            std::cin.ignore();
            std::ifstream ifile;
            ifile.open(filename);

            if (ifile) {
                m_players[i]->set_library(CardParser::parse(filename));
                break;
            }
        }
        // Mélange de la bibliothèque
        m_players[i]->shuffle_library();

        // Phase de pioche initiale
        for(auto index = 0; index < 7; index++) {
            m_players[i]->draw_card();
        }
    }
    
    // Phases de jeu
    while (true) {
        print_info("C'est au tour de " + get_current_player()->get_name() + " de jouer !");
        get_current_player()->set_played_land(0);

        // reset des stats des cartes 
        for (auto p : m_players) {
            for (auto bc : p->get_battlefield()->get_basic_cards()) {
                if (bc->is_class(Card_class::CREATURE)){
                    Creature* c = dynamic_cast<Creature*>(bc);
                    c->set_power_current(c->get_power());
                    c->set_toughness_current(c->get_toughness());
                    c->set_is_first_turn(false);
                }
            }
        }

        // Phase de pioche
        if (get_current_player()->get_library().size() == 0) {
            victory(*get_current_player()->get_opponent());
            return;
        } else {
            print_info("Vous piochez la carte : " + get_current_player()->get_library()[0]->get_name());
            get_current_player()->draw_card();
        }

        // Phase de désengagement
        for (auto c : get_current_player()->get_battlefield()->get_basic_cards()) {
            get_current_player()->get_battlefield()->disengage_card(c);
        }

        // Phase principale
        Game::main_phase(true);
        
        // check if a player has lost
        if (check_defeat())
            return;
        
        // Phase de combat
        Game::combat_phase();

        // check if a player has lost
        if (check_defeat())
            return;
        
        // Phase secondaire
        Game::main_phase(false);

        // check if a player has lost
        if (check_defeat())
            return;

        // Phase end turn
        Game::turn_end_phase();

        get_current_player()->print();
        m_player_turn = !m_player_turn;
    }
}

bool Game::check_defeat() {
    // check if a player has lost
    if (get_current_player()->get_looser()) {
        victory(*get_current_player()->get_opponent());
        return true;
    } else if ((get_current_player()->get_opponent())->get_looser()) {
        victory(*get_current_player());
        return true;
    }
    return false;
}

void Game::main_phase(bool first) {

    while (true) {
        std::system("clear");

        get_current_player()->print();

        std::cout << "Selectionnez une carte à placer :" << std::endl;
        std::cout << "<id>      : pour jouer une carte." << std::endl;
        std::cout << "info <id> : pour avoir des informations sur une carte." << std::endl;
        std::cout << "end       : pour arrêter la phase " << ((first) ? "principale": "secondaire") << "." << std::endl << std::endl;

        std::vector<Card*> hand{};
        for (Card* c : get_current_player()->get_hand()) {
            hand.push_back(c);
        }
        int hand_size = hand.size();
        // Vérifications 
        for (int i = 0; i < hand_size; i++) {

            /* Cas possibles pour une carte jouable:
                - une créature jouable
                - un terrain et le nombre de terrains déjà placés <= 0
                - un rituel jouable
                - un enchantement jouable
             */
            if ((hand[i]->is_class(Card_class::CREATURE) && get_current_player()->get_battlefield()->is_playable(hand[i])) 
            || (hand[i]->is_class(Card_class::LAND) && (get_current_player()->get_played_land() <= 0))
            || (hand[i]->is_class(Card_class::RITUAL) && get_current_player()->get_battlefield()->is_playable(hand[i]))
            || (hand[i]->is_class(Card_class::ENCHANTEMENT) && get_current_player()->get_battlefield()->is_playable(hand[i]))) {
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
        else if (hand_size > 0) {
            try {
                int num = std::stoi(cmd);
                if (num >= hand_size) {
                    print_info("L'id saisit est invalide, veuillez saisir un id disponible dans la liste des id ci-dessus.");
                }
                else
                    get_current_player()->play_card(hand[num]);
            }
            catch (std::invalid_argument &e) {
                std::string info = "info ";
                if (std::mismatch(info.begin(), info.end(), cmd.begin()).first == info.end()) {
                    try {
                        int num = std::stoi(cmd.substr(5));
                        if (num >= hand_size) {
                            print_info("L'id saisit est invalide, veuillez saisir un id disponible dans la liste des ids ci-dessus.");
                        } else {
                            // todo afficher les cartes
                            hand[num]->print();
                            std::cout << "Entrée pour continuer." << std::endl;
                            std::getline(std::cin, cmd);
                        }            
                    } catch (std::invalid_argument &e) {
                        print_info("help : info <id>, id est un chiffre disponible dans la liste des ids ci-dessus.");
                    }  
                } else
                    print_info("Commande non reconnue.");
            } 
        } else {
            print_info("Aucune action disponible, veuillez saisir \"end\" pour terminer cette phase");
        }
    }
}

void Game::combat_phase() {

    std::vector<Creature*> chosen_opponent =  get_current_player()->attack();

    get_current_player()->print();

    if (!chosen_opponent.empty()) {

        std::vector<Creature*> chosen_blockabled_opponent = chosen_opponent;
        
        // supprimer les cartes imblocables des cartes blocables
        for (auto creature : chosen_blockabled_opponent) {

            bool unblockable_opponent = false;

            for (auto ability_creature : creature->get_abilities()) {
                if(ability_creature == Ability::Unblockable) unblockable_opponent = true;
            }

            if (unblockable_opponent)
                remove(creature, chosen_blockabled_opponent);

        }
        
        // demander à l'adervsaire s'il veut défendre l'attaque
        if (!chosen_blockabled_opponent.empty()) {

            std::cout << get_current_player()->get_opponent()->get_name() << ", voulez-vous bloquer ces créatures ? " << std::endl;
            std::cout << "oui       : pour choisir vos bloqueurs." << std::endl;
            //std::cout << "info <id> : pour avoir des informations sur une carte." << std::endl;
            std::cout << "non       : pour laisser l'attaque se faire." << std::endl << std::endl;
            int i = 0;
            for (auto creature : chosen_blockabled_opponent){
                std::cout << i << " - " << creature->get_name() << std::endl;            
            }
        }

        bool quit = false;
        std::string cmd;
        while (!quit) {
            std::getline(std::cin, cmd);
            if (cmd.find("oui") != std::string::npos) {
                get_current_player()->get_opponent()->choose_defenders(chosen_blockabled_opponent);
                quit = true;
            } else if (cmd.find("non") != std::string::npos) {
                
                // directement attaquer l'adervsaire
                for (auto creature : chosen_blockabled_opponent)
                    get_current_player()->get_opponent()->set_hp(get_current_player()->get_opponent()->get_hp() - creature->get_power_current());

            } else
                std::cout << "Commande Invalide" << std::endl;
        }
    }
}

void Game::turn_end_phase() {

    while ((get_current_player()->get_hand()).size() > 7) {

        std::cout <<"Défaussez des cartes, il doit vous rester 7."<<std::endl;
        std::cout << "<id>      : pour defausser une carte." << std::endl;
        //std::cout << "info <id> : pour avoir des informations sur une carte." << std::endl;
        std::cout << "reset     : pour reinitialiser vos choix." << std::endl << std::endl;
        std::cout << "valid     : pour valider votre choix." << std::endl << std::endl;

        int i = 0;
        std::vector<Card*> possible_cards;
        std::vector<Card*> chosen_cards;

        for (auto card : get_current_player()->get_hand()) {
            std::cout<< i << " - " << card->get_name()<<std::endl;
            possible_cards.push_back(card);
            i++;
        }

        bool quit = false;
        std::string cmd;
        while (!quit) {
            std::getline(std::cin, cmd);
            
            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if(cmd.find("reset") != std::string::npos) {
                chosen_cards = {};
                std::cout<< "Reset reussi" <<std::endl;
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > i || num < 0) {
                        print_info("Id invalide");
                    } else {
                        if (contain(possible_cards[num], chosen_cards)) {
                            std::cout << num <<" deja defaussee." << std::endl; 
                        } else {
                            chosen_cards.push_back(possible_cards[num]);
                        }
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide.");
                }
            }
        }
        for (auto card : chosen_cards)
            get_current_player()->discard_card(card);
    }
}

void Game::save() {
    // TODO
}

void Game::choose_save() {
    // TODO
}

void Game::load() {
    // TODO
}

void Game::exit() {
    // TODO
}

void Game::victory(Player p) {
    std::string text = "Bravo " + p.get_name() + " tu as vaincu " + p.get_opponent()->get_name() + " !";
    print_info(text);
}
