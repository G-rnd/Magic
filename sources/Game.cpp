#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iomanip> 

#include "Game.hpp"
#include "Player.hpp"
#include "CardParser.hpp"
#include "SaveParser.hpp"
#include "FonctionsAux.hpp"

Game::Game() {
    m_player_turn = false;
    m_phase = 0;
}

Game::~Game() {
    for(auto p : m_players)
        if (p != nullptr)
            delete p;
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

void Game::set_players(Player* p1, Player* p2) {
    m_players.clear();
    m_players.push_back(p1);
    m_players.push_back(p2);
}

void Game::set_phase(size_t i) {
    if (i < 3)
        m_phase = i;
}

size_t Game::get_phase() const {
    return m_phase;
}

void Game::next_phase() {
    m_phase = (m_phase + 1) % 3;
}

void Game::start() {
    std::string p_name_1;
    std::string p_name_2;

    cls();
    print_actions("Bienvenue dans votre partie !", {}, "Saisir le nom du premier joueur");

    std::cin >> p_name_1;
    Player* p1 = new Player(p_name_1);
    
    cls();
    print_actions("Bienvenue dans votre partie !", {}, "Saisir le nom du second joueur");
    std::cin >> p_name_2;
    Player* p2 = new Player(p_name_2);
    
    cls();

    p1->set_opponent(p2);
    p2->set_opponent(p1);

    set_players(p1, p2);

    // Choix des decks, mélange des bibliothèques et pioche initiale
    std::string path = "data/complet";

    std::vector<std::pair<std::string, std::string > > available_decks = {};

    for (const auto & file : std::filesystem::directory_iterator(path))
        available_decks.push_back({"", (file.path()).string().substr(path.size())});
    for(size_t i = 0; i < available_decks.size(); i++) {
        available_decks[i].first = (available_decks.size() > 10) ? ((i/10 == 1) ? " " : "") : "" + std::to_string(i);
    }

    if (available_decks.size() == 0) {
        cls();
        print_info("Erreur : Aucun deck n'est disponible !");
        return;
    }
    for (size_t i = 0; i < 2; i++) {
        while (true) {
            cls();
            if (i == 0)
                print_actions("Choisir un deck pour le premier joueur", available_decks, "", false, " - ");
            else
                print_actions("Choisir un deck pour le second joueur", available_decks, "", false, " - ");

            try {
                std::string cmd;
                std::cin >> cmd;
                int id = stoi(cmd);

                if (id < 0 || id >= (int) available_decks.size())
                    print_info("Id invalide !");
                else {
                    std::string filename = path + available_decks[id].second;

                    std::ifstream ifile;
                    ifile.open(filename);

                    if (ifile) {
                        m_players[i]->set_library(CardParser::parse(filename));
                        break;
                    } else {
                        print_info("Erreur: le fichier n'existe plus !");
                    }
                }
            } catch (std::invalid_argument& e) {
                print_info("Id invalide !");
            }
        }
        
        // Mélange de la bibliothèque
        m_players[i]->shuffle_library();
        print_info("Bibliothèque mélangée.");

        // Phase de pioche initiale
        for(auto index = 0; index < 7; index++) {
            m_players[i]->draw_card();
        }
    }

    phases();
}

void Game::phases() {
    // Phases de jeu
    while (true) {
        if (m_phase == 0) {
            cls();
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
                cls();
                print_info("Vous piochez la carte : " + get_current_player()->get_library()[0]->get_name());
                get_current_player()->draw_card();
            }

            // Phase de désengagement
            for (auto c : get_current_player()->get_battlefield()->get_basic_cards()) {
                get_current_player()->get_battlefield()->disengage_card(c);
            }

            // Phase principale
            if (Game::main_phase(true))
                return;
            
            // check if a player has lost
            if (check_defeat())
                return;
            
            next_phase();
        }

        if (m_phase == 1) {
            // Phase de combat
            Game::combat_phase();

            // check if a player has lost
            if (check_defeat())
                return;
            
            next_phase();
        }

        if (m_phase == 2) {
            // Phase secondaire
            if (Game::main_phase(false))
                return;

            // check if a player has lost
            if (check_defeat())
                return;

            // Phase end turn
            Game::turn_end_phase();

            get_current_player()->print();
            m_player_turn = !m_player_turn;

            next_phase();
        }
    }
}

bool Game::check_defeat() {
    // check if a player has lost
    if (get_current_player()->get_looser()) {
        victory(get_current_player()->get_opponent());
        return true;
    } else if ((get_current_player()->get_opponent())->get_looser()) {
        victory(get_current_player());
        return true;
    }
    return false;
}

bool Game::main_phase(bool first) {
    cls();
    std::string debut = "Debut de la phase ";
    debut += (first) ? "principale !" : "secondaire !";
    print_actions(debut);
    print_info();

    while (true) {
        cls();
        get_current_player()->print();

        std::string s = "arrêter la phase ";
        s += (first) ? "principale" : "secondaire"; 
        print_actions("Selectionnez une carte à placer", {
            {"<id>", "jouer une carte"},
            {"info <id>", "afficher les caractéristiques d'une carte"},
            {"save <filename>", "sauvegarde la partie en cours"},
            {"exit", "quitte la partie en cours"},
            {"end ", s}
        });

        std::vector<Card*> hand{};
        for (Card* c : get_current_player()->get_hand()) {
            hand.push_back(c);
        }

        std::vector<std::pair<std::string, std::string> > available_cards = {};
        int hand_size = hand.size();
        // Vérifications 
        for (int i = 0; i < hand_size; i++) {
            if ((hand[i]->is_class(Card_class::CREATURE) && get_current_player()->get_battlefield()->is_playable(hand[i])) 
            || (hand[i]->is_class(Card_class::LAND) && (get_current_player()->get_played_land() <= 0))
            || (hand[i]->is_class(Card_class::RITUAL) && get_current_player()->get_battlefield()->is_playable(hand[i]))
            || (hand[i]->is_class(Card_class::ENCHANTEMENT) && get_current_player()->get_battlefield()->is_playable(hand[i]))) {
                available_cards.push_back({std::to_string(i), hand[i]->get_name()});
            } else {
                hand.erase(hand.begin() + i);
                i--;
                hand_size--;        
            }
        }

        if (hand_size == 0) {
            print_info("Aucune action disponible pour cette phase.");
            return false;
        }

        print_list(available_cards);

        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd.find("end") != std::string::npos)
            return false;
        else if (cmd.find("exit") != std::string::npos) {
            exit();
            return true;
        }
        else if (cmd.find("save") != std::string::npos) {
            save(cmd.substr(cmd.find("save ")+5));
            print_info("Partie sauvagardée sous le nom " + cmd.substr(cmd.find("save ")+5) + ".");
        }
        else {
            try {
                int num = std::stoi(cmd);
                if (num >= hand_size) {
                    print_info("L'id saisit est invalide, veuillez saisir un id disponible dans la liste des id ci-dessus.");
                }
                else {
                    cls();
                    print_info("Vous placez la carte : " + hand[num]->get_name());
                    get_current_player()->play_card(hand[num]);
                }
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
                            print_info();
                        }            
                    } catch (std::invalid_argument &e) {
                        print_info("help : info <id>, id est un chiffre disponible dans la liste des ids ci-dessus.");
                    }  
                } else
                    print_info("Commande non reconnue.");
            } 
        }
    }
    return false;
}

void Game::combat_phase() {
    cls();
    print_actions("Debut de la phase de combat !");
    print_info();
    cls();
    
    std::vector<Creature*> chosen_opponent = get_current_player()->attack();
    cls();
    
    if (!chosen_opponent.empty()) {
        get_current_player()->print();

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
        
        while (true) {
            // demander à l'adervsaire s'il veut défendre l'attaque
            if (!chosen_blockabled_opponent.empty()) {
                print_actions(get_current_player()->get_opponent()->get_name() + ", voulez-vous bloquer ces créatures ?", {
                    {"0", "Choisir de bloquer les créatures"},
                    {"1", "Choisir de ne pas bloquer les créatures"}
                });
                std::vector<std::pair<std::string, std::string> > creature_list = {};
                int i = 0;
                for (auto creature : chosen_blockabled_opponent) {
                    creature_list.push_back({std::to_string(i), creature->get_name()});
                    i++;   
                }
                print_list(creature_list);
            }
            
            std::string cmd;
            std::getline(std::cin, cmd);

            try {
                int res = stoi(cmd);
                if (res != 0 && res != 1)
                    print_info("Commande invalide.");
                else if (res == 0) {
                    get_current_player()->get_opponent()->choose_defenders(chosen_blockabled_opponent);
                    break;
                } else {
                    for (auto creature : chosen_blockabled_opponent) {
                        print_info(get_current_player()->get_opponent()->get_name() + " perd " + std::to_string(creature->get_power_current()) + " PV !");
                        get_current_player()->get_opponent()->set_hp(get_current_player()->get_opponent()->get_hp() - creature->get_power_current());
                    }
                    print_info("Retour aux actions de " + get_current_player()->get_name() + ".");
                    break;
                }
            } catch (std::invalid_argument& e) {
                print_info("Commande invalide.");
            }
        }
    }
}

void Game::turn_end_phase() {
    if ((get_current_player()->get_hand()).size() > 7) {
        cls();
        print_actions("Phase de défausse de " + get_current_player()->get_name() + ":");
        print_info();
        
        std::vector<Card*> chosen_cards = {};
        std::vector<Card*> possible_cards;
        for (auto card : get_current_player()->get_hand()) {
            possible_cards.push_back(card);
        }

        bool quit = false;
        while(!quit) {
            while (true) {
                cls();
                print_actions("Vous devez défausser " + std::to_string(get_current_player()->get_hand().size() - 7)+ " cartes !", {
                    {"<id>", "pour défausser une carte."},
                    {"reset", "pour réinitialiser vos choix"}
                });
            
                int i = 0;
                std::vector<std::pair<std::string, std::string> > print_cards = {};

                for(auto card : possible_cards) {
                    print_cards.push_back({std::to_string(i), card->get_name()});
                    i++;
                }
                print_list(print_cards);
                
                std::string cmd;
                std::cin >> cmd;
                    
                if(cmd.find("reset") != std::string::npos) {
                    chosen_cards = {};
                    possible_cards.clear();
                    for (auto card : get_current_player()->get_hand()) {
                        possible_cards.push_back(card);
                    }
                    print_info("Reset reussi !");
                } else {
                    try {
                        int num = std::stoi(cmd);
                        if (num > i || num < 0) {
                            print_info("Id invalide.");
                        } else {
                            print_info("Carte choisie : " + possible_cards[num]->get_name());
                            chosen_cards.push_back(possible_cards[num]);
                            possible_cards.erase(possible_cards.begin() + num);
                        }
                    } catch (std::invalid_argument &e) {
                        print_info("Commande invalide.");
                    }
                }

                if (possible_cards.size() <= 7)
                    break;
            }

            while(true) {
                cls();
                std::vector<std::pair<std::string, std::string> > print_cards = {};
                int i = 0;

                for(auto& c : chosen_cards) {
                    print_cards.push_back({std::to_string(i), c->get_name()});
                    i++;
                }

                print_actions("Voici les cartes que vous voulez défausser :", {
                    {"0","retourer au choix des cartes à défausser"},
                    {"1", "valider votre choix"}
                });
                print_list(print_cards);
                try {    
                    std::string cmd;
                    std::getline(std::cin, cmd);

                    int num = std::stoi(cmd);
                    if (num != 0 && num != 1) {
                        print_info("Id invalide.");
                    } else {
                        if (num == 1) {
                            for (auto card : chosen_cards)
                            get_current_player()->discard_card(card);
                            print_info("Défausse réussie !");
                            quit = true;
                        } else {
                            chosen_cards = {};
                            possible_cards.clear();
                            for (auto card : get_current_player()->get_hand()) {
                                possible_cards.push_back(card);
                            }
                            print_info("Retour aux choix des cartes à défausser.");

                        }
                        break;
                    }
                } catch (std::invalid_argument &e) {
                        print_info("Id invalide.");
                }
            }
        }
    }
}

void Game::save(const std::string& name) {
    std::string path = "saves/" + name + ".txt";

    std::ofstream file;
    file.open(path, std::ios::out | std::ofstream::trunc);
    file << to_string();
    file.close();
}

std::string Game::to_string() {
    std::string s = "";
    s += SaveParser::begin_game + "\n";
    s += SaveParser::player_turn + std::to_string(m_player_turn) + "\n";
    s += SaveParser::phase + std::to_string(m_phase) + "\n";
    for(auto p : m_players)
        s += p->to_string();
    
    s += SaveParser::end_game + "\n";
    return s;
}

void Game::choose_save(std::string& data) {
    Game* g = SaveParser::load(data);
    if (g != nullptr) {
        g->phases();
    }
    delete g;
}

void Game::load() {
    std::string path = "saves/";

    std::vector<std::pair<std::string, std::string > > available_saves = {};

    for (const auto & file : std::filesystem::directory_iterator(path))
        available_saves.push_back({"", (file.path()).string().substr(path.size())});
    for(size_t i = 0; i < available_saves.size(); i++) {
        available_saves[i].first = (available_saves.size() > 10) ? ((i/10 == 1) ? " " : "") : "" + std::to_string(i);
    }

    if (available_saves.size() == 0) {
        cls();
        print_info("Aucune partie n'a été sauvegardée.");
        return;
    }
    available_saves.push_back({"back","retourner au menu."});

    while (true) {
        cls();
        print_actions("Voici la liste des parties sauvagardées", available_saves, "Saisir l'id de la partie", false);

        std::string cmd;
        std::getline(std::cin, cmd);

        try {
            int id = stoi(cmd);

            if (id < 0 || id >= (int) available_saves.size() - 1) {
                print_info("Id invalide !");
                continue;
            } else {
                std::string filename = path + available_saves[id].second;

                std::ifstream ifile;
                ifile.open(filename);

                if (ifile) {
                    std::string read_file = "";
                    std::getline(std::ifstream(filename), read_file, '\0');
                    choose_save(read_file);
                    break;
                } else {
                    print_info("Erreur: le fichier n'existe plus !");
                }
            }
        } catch (std::invalid_argument& e) {
            if (cmd.find("back") != std::string::npos) {
                print_info("Retour au menu principal.");
                break;
            } else
                print_info("Commande invalide !");
        }
    }
}

void Game::exit() {
    while (true) {
        cls();
        print_actions("Voulez-vous sauvagarder la partie avant de la quitter ?", {{"0", "non"}, {"1", "oui"}}, "Saisir un nombre");

        std::string cmd;
        std::cin.clear();
        std::getline(std::cin, cmd);
        try {
            int res = stoi(cmd);
            switch (res) {
                case 0:
                    print_info("Vous quittez la partie.");
                    return;
                case 1: {
                    cls();
                    print_actions("Saisir un nom pour la sauvagarde", {{"<file>", "nom du fichier"}});
                    std::cin.clear();
                    std::getline(std::cin, cmd);
                    save(cmd);
                    print_info("Vous quittez la partie.");
                    return;
                }
                default:
                    print_err("Commande invalide.");
                    break;


            }
        } catch(std::invalid_argument& e) {
            print_err("Commande invalide.");
        }
    }

}

void Game::victory(Player* p) {
    print_info("Bravo " + p->get_name() + "tu as vaincu " + p->get_opponent()->get_name() + " !");
}

void Game::print_title_screen() {
    cls();
    print_actions("Bienvenue dans une nouvelle édition de Magic The Gathering !", {
        { "play",         "pour lancer une nouvelle partie" },
        { "load",         "pour charger une de vos parties" },
        { "deck-builder", "pour créer votre propre deck" },
        { "exit",         "pour quitter le jeu." }
    }, "Choisissez votre mode de jeu :");
}