#include <iostream>
#include <string>
#include <iterator>
#include <iomanip> 

#include "Player.hpp"
#include "Card.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Battlefield.hpp"
#include "Ritual.hpp"
#include "Enchantment.hpp"
#include "SaveParser.hpp"

#include "FonctionsAux.hpp"

Player::Player(std::string name): m_name(name) {
    //std::cout << "[Player] : Création de " << this << std::endl;
    m_battlefield = new Battlefield();
    m_hp = 20;
    m_looser = false;
}

Player::~Player() {
    //std::cout << "[Player] : Denstruction de " << this << std::endl;

    for(auto c : m_deck)
        delete c;
    for(auto c : m_graveyard)
        delete c;
    for(auto c : m_library)
        delete c;
    for(auto c : m_hand)
        delete c;
    delete m_battlefield;
}

Player* Player::get_opponent() const {
    return m_opponent;
}

std::string Player::get_name() const {
    return m_name;
}

int Player::get_hp() const {
    return m_hp;
}

int Player::get_played_land() const {
    return m_played_land;
}

Battlefield* Player::get_battlefield() const {
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

bool Player::get_looser() const {
    return m_looser;
}

std::string Player::to_string() {
    std::string s = "";
    s += SaveParser::begin_player + "\n";
    s += SaveParser::name + m_name + "\n";
    s += SaveParser::hp + std::to_string(m_hp) + "\n";
    s += SaveParser::played_land + std::to_string(m_played_land) + "\n";

    s += SaveParser::begin_library + "\n";
    
    for(auto c : m_library)
        s += c->to_string();
    s += SaveParser::end_library + "\n";

    s += SaveParser::begin_graveyard + "\n";
    for(auto c : m_graveyard)
        s += c->to_string();
    s += SaveParser::end_graveyard + "\n";

    s += SaveParser::begin_hand + "\n";
    for(auto c : m_hand)
        s += c->to_string();
    s += SaveParser::end_hand + "\n";

    s += SaveParser::begin_battlefield + "\n";
    s += m_battlefield->to_string();
    s += SaveParser::end_battlefield + "\n";

    s += SaveParser::end_player + "\n";
    return s;
}

void Player::set_opponent(Player* p) {
    m_opponent = p;
}

void Player::set_battlefield(Battlefield* b) {
    m_battlefield = b;
}

void Player::set_name(std::string s) {
    m_name = s;
}

void Player::set_hp(int i) {
    m_hp = i;
    if(m_hp == 0) m_looser = true;
}

void Player::set_graveyard(const std::vector<Card*>& cards) {
    m_graveyard = cards;
}

void Player::set_library(const std::vector<Card*>& cards) {
    m_library = cards;
}

void Player::set_hand(const std::vector<Card*>& cards) {
    m_hand = cards;
}

void Player::set_looser(bool b) {
    m_looser = b;
}

void Player::set_played_land(int i) {
    m_played_land = i;
}

void Player::add_played_land(int i) {
    m_played_land += i;
}

void Player::add_hand(Card* c) {
    m_hand.push_back(c);
}

void Player::remove_battlefield(Card* c) {
    if (c->is_class(Card_class::ENCHANTEMENT)) {
        m_battlefield->remove_enchantment(dynamic_cast<Enchantment*>(c));
    } else if (c->is_class(Card_class::CREATURE) || c->is_class(Card_class::LAND)) {
        m_battlefield->remove_basic_card(dynamic_cast<BasicCard*>(c));
    }
}

void Player::sort_hand(){

    std::vector<Card*> sort_card;

    for (auto c : m_hand) {
        if(c->is_class(Card_class::LAND))
            sort_card.push_back(c);
    }
    for (auto c : m_hand) {
        if (c->is_class(Card_class::CREATURE))
            sort_card.push_back(c);
    }
    for (auto c : m_hand){
        if(c->is_class(Card_class::ENCHANTEMENT))
            sort_card.push_back(c);
    }
    for (auto c : m_hand){
        if(c->is_class(Card_class::RITUAL))
            sort_card.push_back(c);
    }
    set_hand(sort_card);
}

void Player::draw_card() {
    if (m_library.empty()) {
        m_looser = true;
    } else {
        m_hand.push_back(*m_library.begin());
        remove(*m_library.begin(), m_library);
    }
}

void Player::discard_card(Card* c) {
    remove(c, m_hand);
    m_graveyard.push_back(c);
}

void Player::shuffle_library() {

/*
    std::default_engine_generator generator;
    std::uniform_int_distribution<int> distribution(1, 6);
    int rand = distribution(generator);
*/

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

    if(c->get_token() == Token::White){
        for (auto e : m_battlefield->get_enchantments()){
            if(e->get_token() == Token::White){
                for (auto effect : e->get_effects()) {
                    if(effect == White_enchantment_effects::Win_1_HP_white){
                        m_hp ++;
                        print_info("Vous gagnez 1 point de vie pour avoir joué une carte blanche !");
                    }
                }
            }
        }
    }

    if (c->is_class(Card_class::LAND)) {
        add_played_land(1);
        m_battlefield->place_basic_card(dynamic_cast<BasicCard*>(c));
        remove(c, m_hand);
    } else if(c->is_class(Card_class::RITUAL)){
        Ritual* r = dynamic_cast<Ritual*>(c);
        play_ritual(r);
        remove(c, m_hand);
        m_battlefield->engage_lands(r->get_cost());
    } else if(c->is_class(Card_class::ENCHANTEMENT)){
        Enchantment* e = dynamic_cast<Enchantment*>(c);
        play_enchantment(e);
        remove(c, m_hand);
    } else if(c->is_class(Card_class::CREATURE)){
        Creature* cre = dynamic_cast<Creature*>(c);
        cre->set_is_first_turn(true);
        m_battlefield->place_basic_card(dynamic_cast<BasicCard*>(c));
        remove(c, m_hand);
        m_battlefield->engage_lands(cre->get_cost());
    }
}

void Player::engage_card(BasicCard* bc) {
    bc->set_engaged(true);
}

void Player::disengage_card(BasicCard* bc) {
    bc->set_engaged(false);
}

/*
- Vigilance
- Defender
- Haste
*/
std::vector<Creature*> Player::attack() {

    int i = 0;
    std::vector<Creature*> possible_creatures = {};
    
    std::vector<std::pair<std::string, std::string> > print_creatures = {};
    // List the available creatures
    for (auto creature : m_battlefield->get_available_creatures()) {
        // Etablish abilities
        bool defender_creature = false;
        bool haste_creature = false;

        for (auto ability_creature : creature->get_abilities()) {
            if (ability_creature == Ability::Defender) {
                defender_creature = true;
            } else if (ability_creature == Ability::Haste) {
                haste_creature = true;
            }
        }

        if (!creature->get_engaged()) {
            // Check haste ability
            if (!creature->get_is_first_turn() || haste_creature) {
                // Check defender ability
                if (!defender_creature) {
                    print_creatures.push_back({std::to_string(i), creature->get_name()});
                    possible_creatures.push_back(creature);
                    i++;
                }
            } else {
                // TODO error
            }
        }
    }

    if (possible_creatures.size() == 0) {
        print_info("Aucune action disponible pour cette phase.");
        return possible_creatures;
    }

    std::vector<Creature*> returned_creatures = {};
    std::vector<Creature*> creatures = {};
    for(auto& c : possible_creatures)
        creatures.push_back(c);

    std::string cmd;

    // Interaction with player : choices
    while (true) {
        cls();
        this->print();

        print_actions("Sélectionnez les cartes pour attaquer !", {
            {"<id>", "pour attaquer avec cette carte"},
            {"reset", "pour annuler vos choix"},
            {"valid", "pour valider vos choix"}
        });

        size_t id = 0;
        print_creatures = {};
        for(auto& c : creatures) {
            print_creatures.push_back({std::to_string(id), c->get_name()});
            id++;
        }
        print_list(print_creatures);
        std::getline(std::cin, cmd);

        if (cmd.find("valid") != std::string::npos) {
            id = 0;
            print_creatures = {};
            for(auto& c : returned_creatures) {
                print_creatures.push_back({std::to_string(id), c->get_name()});
                id++;
            }
            if(print_creatures.size() > 0) {
                print_actions("Vous attaquez avec les créatures suivantes :", print_creatures);
                print_info();
            } else {
                print_info("Vous n'attaquez avec aucune créature.");
            }
            break;
        }

        if (cmd.find("reset") != std::string::npos) {    
            returned_creatures = {};
            creatures = {};
            for(auto& c : possible_creatures)
                creatures.push_back(c);
            print_info("Reset réussi.");
        } else {
            try {
                int num = std::stoi(cmd);
                if (num >= (int) creatures.size() || num < 0) {
                    print_info("Id invalide.");
                } else {
                    print_info("Vous venez d'ajouter " + creatures[num]->get_name() + ".");
                    returned_creatures.push_back(creatures[num]);
                    creatures.erase(creatures.begin() + num);
                }
            } catch (std::invalid_argument &e) {
                print_info("Commande invalide.");
            }
        }
    }
    bool vigilance_creature = false;

    for (auto card : returned_creatures) {
        // Check the vigilance ability
        for (auto ability_card : card->get_abilities()) {

            if (ability_card == Ability::Vigilance) {
                vigilance_creature = true;
            }
        }
        if (!vigilance_creature) {
            engage_card(card);
        }
    }
    return returned_creatures;
}

/*
- Flight
- Scope
- Threat
*/

void Player::choose_defenders(std::vector<Creature*> opponents) {

    std::vector<Creature*> availabled_creature = m_battlefield->get_available_creatures();
    std::vector<std::pair<std::string, std::string> > print_creatures = {};

    for (auto opponent : opponents) {

        std::string cmd;
        int i = 0;
        bool quit = false;
        std::vector<Creature*> possible_defenders = {};
        std::vector<Creature*> chosen_defenders = {}; //TODO si des creatures meurent

        // Etablish opponent abilities
        bool threat_opponent = false;
        bool flight_opponent = false;

        for (auto ability_opponent : opponent->get_abilities()) {
            if (ability_opponent == Ability::Flight) {
                flight_opponent = true;
            } else if (ability_opponent == Ability::Threat) {
                threat_opponent = true;
            }
        }

        for (auto creature : availabled_creature) {

            // Etablish creature abilities
            bool scope_creature = false;
            bool flight_creature = false;

            for (auto ability_creature : creature->get_abilities()) {
                if (ability_creature == Ability::Flight) {
                    flight_creature = true;
                } else if (ability_creature == Ability::Scope) {
                    scope_creature = true;
                }
            }

            // Check Flight ability
            if ((flight_opponent && (flight_creature || scope_creature)) || !flight_opponent) {
                possible_defenders.push_back(creature);
                i++;
            }

        }

        std::vector<Creature*> creatures = {};
        for(auto& c : possible_defenders)
            creatures.push_back(c);

        // Interaction with player : choices
        while (!quit) {

            cls();
            m_opponent->print();

            print_actions(m_name + ", selectionnez les cartes pour défendre " + opponent->get_name() + " :", {
            {"<id>", "pour défendre avec cette carte"},
            {"reset", "pour annuler vos choix"},
            {"valid", "pour valider vos choix"} });

            size_t id = 0;
            print_creatures = {};
            for(auto& c : creatures) {
                print_creatures.push_back({std::to_string(id), c->get_name()});
                id++;
            }
            print_list(print_creatures);
            
            std::getline(std::cin, cmd);
        
            if (cmd.find("valid") != std::string::npos) {
                // Check Threat ability is respected
                if (threat_opponent && (chosen_defenders.size() == 1)) {
                    std::cout << opponent->get_name() << " vous Menace, choississez un autre defenseur ou annulez le blocage : " << std::endl;
                } else {
                    quit = true;
                    if (!chosen_defenders.empty()) {
                        for (auto c : chosen_defenders){
                            engage_card(c);
                        }
                        if (chosen_defenders.size() > 1){
                            chosen_defenders = m_opponent->choose_defenders_orders(chosen_defenders, opponent);
                        }
                        // Deflect attack for each opponent with the possible and chosen defender
                        this->deflect_attack(opponent, chosen_defenders);
                    } else {
                        set_hp(m_opponent->get_hp() - opponent->get_power_current()); // TODO : check defeat
                    }
                }
            } else if (cmd.find("reset") != std::string::npos) {
                creatures = possible_defenders;
                chosen_defenders = {};
                print_info("Reset reussi");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > i || num < 0) {
                        print_info("Id invalide");
                    } else {
                        if (contain(creatures[num], chosen_defenders)) {
                            print_info(std::to_string(num) + " deja choisie"); 
                        } else {
                            print_info("Vous venez d'ajouter " + creatures[num]->get_name() + ".");
                            chosen_defenders.push_back(creatures[num]);
                            remove(creatures[num], creatures);
                        }
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande Invalide");    
                }
            }
        }
    }
}

// l'attaquant choisi l'ordre du blocage
std::vector<Creature*> Player::choose_defenders_orders(std::vector<Creature*> defenders, Creature* opponent){

    std::vector<std::pair<std::string, std::string> > print_creatures = {};

    std::string cmd;
    int i = 0;
    bool quit = false;
    std::vector<Creature*> possible_defenders = defenders;
    std::vector<Creature*> chosen_defenders = {};

    while(!possible_defenders.empty()){

        cls();
        this->print();

        print_actions(m_name + ", selectionnez l'ordre des defenseurs que " + opponent->get_name() + " va attaquer :", {
        {"<id>", "pour selectionner cette carte"},
        {"reset", "pour annuler vos choix"},
        {"valid", "pour valider vos choix"} });

        size_t id = 0;
        print_creatures = {};
        for(auto& c : possible_defenders) {
            print_creatures.push_back({std::to_string(id), c->get_name()});
            id++;
        }
        print_list(print_creatures);

        // Interaction with player : choices
        while (!quit) {
            std::getline(std::cin, cmd);
            
        
            if (cmd.find("valid") != std::string::npos) {
                if(chosen_defenders.size() != defenders.size()){
                    print_info("Vous n'avez pas selectionne toutes les creatures !");
                } else {
                    quit = true;
                    size_t id = 0;
                    std::vector<std::pair<std::string, std::string> > print_creatures = {};
                    for(auto& c : chosen_defenders) {
                        print_creatures.push_back({std::to_string(id), c->get_name()});
                        id++;
                    }
                    print_actions("Voici l'ordre dans lequel " + m_opponent->get_name() + " va vous defendre :", print_creatures);
                }
            } else if (cmd.find("reset") != std::string::npos) {
                    chosen_defenders = {};
                    print_info("Reset reussi");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > i || num < 0) {
                        print_info("Id invalide");
                    } else {
                        if (contain(possible_defenders[num], chosen_defenders)) {
                            print_info(std::to_string(num) + " deja choisie"); 
                        } else {
                            print_info("Vous venez d'ajouter " + possible_defenders[num]->get_name() + ".");
                            chosen_defenders.push_back(possible_defenders[num]);
                            remove(possible_defenders[num], possible_defenders);
                        }
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande Invalide");
                }
            }
        }

    }

    return chosen_defenders;
    

}

void Player::deflect_attack(Creature* opponent, std::vector<Creature*> defenders) {

    for (auto defender : defenders) {
        // Check if the opponent is already dead
        if(contain(dynamic_cast<BasicCard*>(opponent), m_opponent->get_battlefield()->get_basic_cards())) {
            battle_creature(opponent, defender);
        }
    }
}

/*
- Touch_of_death
- Initiative
- Double initiative
- Trampling
- Life link
*/
// this est le joueur ayant joué defender
void Player::battle_creature(Creature* opponent, Creature* defender) {

    bool opponent_dead = false;
    bool defender_dead = false;

    int toughness_defender = defender->get_toughness_current();
    int toughness_opponent = opponent->get_toughness_current();

    // Etablish abilities
    bool touch_of_death_defender = false;
    bool touch_of_death_opponent = false;

    bool initiative_defender = false;
    bool initiative_opponent = false;
    
    bool double_initiative_defender = false;
    bool double_initiative_opponent = false;

    bool life_link_defender = false;
    bool life_link_opponent = false;

    bool trampling_opponent = false;

    for (auto ability_opponent : opponent->get_abilities()) {
        if (ability_opponent == Ability::Initiative) {
            initiative_opponent = true;
        } else if (ability_opponent == Ability::Touch_of_death) {
            touch_of_death_opponent = true;
        } else if (ability_opponent == Ability::Double_initiative) {
            double_initiative_opponent = true;
        } else if (ability_opponent == Ability::Life_link) {
            life_link_opponent = true;
        } else if (ability_opponent == Ability::Trampling) {
            trampling_opponent = true;
        }
    }

    for (auto ability_defender : defender->get_abilities()) {
        if (ability_defender == Ability::Initiative) {
            initiative_defender = true;
        } else if (ability_defender == Ability::Touch_of_death) {
            touch_of_death_defender = true;
        } else if (ability_defender == Ability::Double_initiative) {
            double_initiative_defender = true;
        } else if (ability_defender == Ability::Life_link) {
            life_link_defender = true;
        }
    }

    // Check initiative ability and double_initiative
    if ((initiative_opponent && !initiative_defender) || (double_initiative_opponent && !double_initiative_defender)) {
        defender->set_toughness_current(defender->get_toughness_current() - opponent->get_power_current());
        // Check life_link ability
        if (life_link_opponent) {
            if (toughness_defender >= opponent->get_power_current()) {
                m_opponent->set_hp(m_opponent->get_hp() + opponent->get_power_current());
            } else {
                m_opponent->set_hp(m_opponent->get_hp() + toughness_defender);
            }
        }
        if (defender->get_toughness_current() <= 0) {
            defender_dead = true;
            destroy_card(defender);
        }
    } else if ((!initiative_opponent && initiative_defender) || (!double_initiative_opponent && double_initiative_defender)) {
        opponent->set_toughness_current(opponent->get_toughness_current() - defender->get_power_current());
        // Check life_link ability
        if (life_link_defender) {
            if (toughness_opponent >= defender->get_power_current()) {
                this->set_hp(this->get_hp() + defender->get_power_current());
            } else {
                this->set_hp(this->get_hp() + toughness_opponent);
            }
        }
        if (opponent->get_toughness_current() <= 0) {
            opponent_dead = true;
            m_opponent->destroy_card(opponent);
        }
    }
    // If the battle continue
    if (!opponent_dead && !defender_dead) {

        // Check touch_of_death ability
        if (touch_of_death_opponent) {
            if (opponent->get_power_current() > 0) {
                defender->set_toughness_current(0);
                // TODO : Life link et Touch of death non compatible ?
            } 
        } else if (!initiative_opponent) {
            defender->set_toughness_current(defender->get_toughness_current() - opponent->get_power_current());
            // Check Life_link ability
            if (life_link_opponent) {
                if (toughness_defender >= opponent->get_power_current()) {
                    m_opponent->set_hp(m_opponent->get_hp() + opponent->get_power_current());
                } else {
                    m_opponent->set_hp(m_opponent->get_hp() + toughness_defender);
                }
            }
        }
        if (touch_of_death_defender) {
            if (defender->get_power_current() > 0) {
                opponent->set_toughness_current(0);
            } 
        } else if (!initiative_defender) {
            opponent->set_toughness_current(opponent->get_toughness_current() - defender->get_power_current());
            // Check life_link ability
            if (life_link_defender) {
                if (toughness_opponent >= defender->get_power_current()) {
                    this->set_hp(this->get_hp() + defender->get_power_current());
                } else {
                    this->set_hp(this->get_hp() + toughness_opponent);
                }
            }
        }

        // If the creatures are dead, deplace them into the graveyard
        if (opponent->get_toughness_current() <= 0) {
            opponent_dead = true;
            m_opponent->destroy_card(opponent);
        }
        if (defender->get_toughness_current() <= 0) {
            defender_dead = true;
            destroy_card(defender);
        }
    }

    // Check Trampling ability
    if (!opponent_dead && trampling_opponent) {
        this->set_hp(this->get_hp() - opponent->get_power_current()); //TODO change power change
    }
}

void Player::destroy_card(Card* c) {

    // If c is a BasicCard, we deplace it into the graveyard
    if (c->is_class(Card_class::LAND) || c->is_class(Card_class::CREATURE)) {
        m_battlefield->set_basic_cards(dynamic_cast<BasicCard*>(c)->remove(m_battlefield->get_basic_cards()));
        m_graveyard.push_back(c);
        // We also deplace the enchantments associated to c
        for (auto e : (dynamic_cast<BasicCard*>(c))->get_enchantments()) {
            m_graveyard.push_back(e);
        }
        dynamic_cast<BasicCard*>(c)->reset_enchantments();

        // Check the enchantment : remove 1 hp to the opponent when a creature die
        if(c->is_class(Card_class::CREATURE)){
            for (auto ench : m_battlefield->get_enchantments()){
                for (auto effect : ench->get_effects()){
                    if(effect == Black_enchantment_effects::Less_HP_death_creature){
                        m_opponent->set_hp(m_opponent->get_hp() - 1);
                        print_info("Une creature est morte, " + m_opponent->get_name() + " perd un point de vie !");
                    }
                }
            }
            for (auto ench : m_opponent->get_battlefield()->get_enchantments()){
                for (auto effect : ench->get_effects()){
                    if(effect == Black_enchantment_effects::Less_HP_death_creature){
                        m_hp--;
                        print_info("Une creature est morte, " + m_name + " perd un point de vie !");
                    }
                }
              }
        }

    } 
    // If c is a Ritual, we place it into the graveyard
    else if (c->is_class(Card_class::RITUAL)) {
        m_graveyard.push_back(c);
    }
}

void Player::play_ritual(Ritual* r) {

    switch (r->get_token()) {
        case Token::White: {

            for (auto effect : r->get_effects()) {

                switch (effect) {

                    // Add 3 HP to the player
                    case White_ritual_effects::More_3_HP:{
                        this->set_hp(this->get_hp() + 3);
                        print_info("Vous venez de gagner 3 points de vie en jouant " + r->get_name());
                    }
                    break;

                    // All the creatures of the player win 1 power and 1 toughness for the turn
                    case White_ritual_effects::More_1_1_creature_current: {
                        for (auto bc : m_battlefield->get_basic_cards()) {
                            if (bc->is_class(Card_class::CREATURE)) {
                                Creature* creature = dynamic_cast<Creature*>(bc);
                                creature->set_power_current(creature->get_power_current() + 1);
                                creature->set_toughness_current(creature->get_toughness_current() + 1);
                                print_info(creature->get_name() + " gagne 1 / 1 pour ce tour-ci");
                            }
                        }
                    }
                    break;

                    // The player destroy an engaged creature of its opponent
                    case White_ritual_effects::Destroy_engaged_creature: {

                        int i = 0;
                        std::vector<Creature*> possible_creatures;

                        print_info("Selectionnez une creature pour le detruire de la partie :");

                        for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                            if (bc->is_class(Card_class::CREATURE)) {
                                Creature* creature = dynamic_cast<Creature*>(bc);

                                if (creature->get_engaged()) {
                                    std::cout<< i << " - " << creature->get_name() <<std::endl;
                                    possible_creatures.push_back(creature);
                                    i++;
                                }
                            }
                        }

                        std::string cmd;

                        while (true) {
                            std::getline(std::cin, cmd);

                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide.");
                                } else {
                                    Creature* chosen_creature = possible_creatures[num];
                                    // TODO : tester si le delete fonctionne
                                    m_opponent->get_battlefield()->remove_basic_card(chosen_creature);
                                    print_info("Vous venez de détruire " + chosen_creature->get_name() + ".");
                                    delete chosen_creature;
                                    break;
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande invalide.");
                            }
                        }
                    }
                    break;

                    // The player destroy an enchantment of its opponent
                    case White_ritual_effects::Destroy_enchantment: {

                        int i = 0;
                        std::vector<Enchantment*> possible_enchantments;

                        print_info("Selectionnez un enchantement pour le detruire de la partie :");

                        // Each enchantment on the battlefield of the opponent
                        for (auto e : m_opponent->get_battlefield()->get_enchantments()) {
                            
                            std::cout<< i << " - " << e->get_name() << " global " <<std::endl;
                            possible_enchantments.push_back(e);
                            i++;
                        }
                        // Each enchantment of a basic card on the battlefield of the opponent
                        for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                            for (auto e : bc->get_enchantments()) {
                                
                                std::cout<< i << " - " << e->get_name() << " : " << bc->get_name() <<std::endl;
                                possible_enchantments.push_back(e);
                                i++;

                            }
                        }

                        std::string cmd;

                        while (true) {
                            std::getline(std::cin, cmd);

                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide.");
                                } else {
                                    Enchantment* chosen_enchantment = possible_enchantments[num];
                                    // TODO : tester si delete fonctionne
                                    m_opponent->get_battlefield()->remove_enchantment(chosen_enchantment);
                                    print_info("Vous venez de détruire " + chosen_enchantment->get_name() + ".");
                                    delete chosen_enchantment;
                                    break;
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande invalide.");
                            }
                        }
                    }
                    break;

                    default :
                        // TODO error
                        break;

                }
            }
        }
        break;   

        case Token::Blue:

            for (auto effect : r->get_effects()) {
                
                switch (effect) {
                
                // Draw 2 cards
                case Blue_ritual_effects::Draw_2_cards:
                    
                    this->draw_card();
                    this->draw_card();

                break;

                // Return an opponent creature to the hand of the opponent
                case Blue_ritual_effects::Back_hand_creature: {

                    int i = 0;
                    std::vector<Creature*> possible_creatures;

                    print_info("Selectionnez une creature pour la renvoyer dans la main de votre adversaire :");

                    for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature->get_name() <<std::endl;
                            possible_creatures.push_back(creature);
                            i++;
                        }
                    }

                    std::string cmd;

                    while (true) {
                        std::getline(std::cin, cmd);

                        try {
                            int num = std::stoi(cmd);
                            if (num > i || num < 0) {
                                print_info("Id invalide.");
                            } else {
                                Creature* chosen_creature = possible_creatures[num];
                                m_opponent->add_hand(chosen_creature);
                                m_opponent->remove_battlefield(chosen_creature);
                                print_info("Vous venez de retourner " + chosen_creature->get_name() + " dans la main de votre adversaire.");
                                break;
                            }
                        } catch (std::invalid_argument &e) {
                            print_info("Commande invalide.");
                        }
                    }
                }
                break;
                
                default:
                    break;
                }
            }
            break;

        case Token::Black: {
            
            for (auto effect : r->get_effects()) {
                
                switch (effect){

                case Black_ritual_effects::Kill_creature: {

                    int i = 0;
                    std::vector<Creature*> possible_creatures;

                    print_info("Selectionnez une creature pour la tuer : ");

                    for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature->get_name() <<std::endl;
                            possible_creatures.push_back(creature);
                            i++;
                        }
                    }

                    std::string cmd;

                    while (true) {
                        std::getline(std::cin, cmd);

                        try {
                            int num = std::stoi(cmd);
                            if (num > i || num < 0) {
                                print_info("Id invalide.");
                            } else {
                                Creature* chosen_creature = possible_creatures[num];
                                destroy_card(chosen_creature);
                                print_info("Vous venez de placer " + chosen_creature->get_name() + " dans le cimetière de votre adversaire ! ");
                                break;
                            }
                        } catch (std::invalid_argument &e) {
                            print_info("Commande invalide.");
                        }
                    }
                }
                break;

                case Black_ritual_effects::Kill_creature_2_power: {

                    int i = 0;
                    std::vector<Creature*> possible_creatures;

                    print_info("Selectionnez une creature avec plus de 2 de force pour la tuer : ");

                    for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            if (creature->get_power_current() <= 2) {
                                std::cout<< i << " - " << creature->get_name() <<std::endl;
                                possible_creatures.push_back(creature);
                                i++;
                            }
                        }
                    }

                    std::string cmd;

                    while (true) {
                        std::getline(std::cin, cmd);

                        try {
                            int num = std::stoi(cmd);
                            if (num > i || num < 0) {
                                print_info("Id invalide.");
                            } else {
                                Creature* chosen_creature = possible_creatures[num];
                                destroy_card(chosen_creature);
                                print_info("Vous venez de placer " + chosen_creature->get_name() + " dans le cimetière de votre adversaire ! ");
                                break;
                            }
                        } catch (std::invalid_argument &e) {
                            print_info("Commande invalide.");
                        }
                    }

                    
                }
                break;

                case Black_ritual_effects::Kill_not_angel: {

                    int i = 0;
                    std::vector<Creature*> possible_creatures;

                    print_info("Selectionnez une creature qui n'est pas un Ange pour la tuer : ");

                    for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);
                            bool is_angel = false;

                            for (auto type_creature : creature->get_types()) {
                                if(type_creature == Type::Angel) is_angel = true;
                            }
                            
                            if (!is_angel) {
                                std::cout << i << " - " << creature->get_name() << std::endl;
                                possible_creatures.push_back(creature);
                                i++;
                            }
                        }
                    }

                    std::string cmd;

                    while (true) {
                        std::getline(std::cin, cmd);

                        try {
                            int num = std::stoi(cmd);
                            if (num > i || num < 0) {
                                print_info("Id invalide.");
                            } else {
                                Creature* chosen_creature = possible_creatures[num];
                                destroy_card(chosen_creature);
                                print_info("Vous venez de placer " + chosen_creature->get_name() + " dans le cimetière de votre adversaire ! ");
                                break;
                            }
                        } catch (std::invalid_argument &e) {
                            print_info("Commande invalide.");
                        }
                    }
                }
                break;

                case Black_ritual_effects::Less_2_2_creature_current: {

                    int i = 0;
                    std::vector<Creature*> possible_creatures;

                    print_info("Selectionnez une creature pour lui infligez -2 / -2 :");

                    for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature->get_name() <<std::endl;
                            possible_creatures.push_back(creature);
                            i++;
                        }
                    }

                    std::string cmd;

                    while (true) {
                        std::getline(std::cin, cmd);

                        try {
                            int num = std::stoi(cmd);
                            if (num > i || num < 0) {
                                print_info("Id invalide.");
                            } else {
                                Creature* chosen_creature = possible_creatures[num];

                                if (chosen_creature->get_power_current() < 3) {
                                    chosen_creature->set_power_current(0);
                                } else {
                                    chosen_creature->set_power_current(chosen_creature->get_power_current() - 2);
                                }

                                if (chosen_creature->get_toughness_current() < 3) {
                                    destroy_card(chosen_creature);
                                } else {
                                    chosen_creature->set_toughness_current(chosen_creature->get_toughness_current() - 2);
                                }

                                print_info("Vous venez d'affecter -2 / -2 à " + chosen_creature->get_name() + ".");
                                break;
                            }
                        } catch (std::invalid_argument &e) {
                            print_info("Commande invalide.");
                        }
                    }
                }
                break;
        
                default:
                    break;
                }
            }
        }
        break;

        case Token::Red: {

            for (auto effect : r->get_effects()) {

                switch (effect) {

                case Red_ritual_effects::Damage_3_creature_or_player: {

                    std::string cmd;

                    while (true) {

                        print_actions("Faites votre choix !", {
                            {"crea", "pour infliger 3 dégâts à une créature"},
                            {"player", "pour infliger 3 dégâts à votre adversaire"} });
                            
                        std::getline(std::cin, cmd);

                        if (cmd == "player") {
                            m_opponent->set_hp(m_opponent->get_hp() - 3);
                            print_info("Vous venez d'infliger 3 dégâts à votre adversaire ! ");
                            break;
                        } else if (cmd == "crea") {
                            int i = 0;
                            std::vector<Creature*> possible_creatures;

                            for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                                
                                if (bc->is_class(Card_class::CREATURE)) {

                                    Creature* creature = dynamic_cast<Creature*>(bc);

                                    std::cout<< i << " - " << creature->get_name() <<std::endl;
                                    possible_creatures.push_back(creature);
                                    i++;
                                }
                            }

                            while (true) {
                                std::getline(std::cin, cmd);

                                try {
                                    int num = std::stoi(cmd);
                                    if (num > i || num < 0) {
                                        print_info("Id invalide.");
                                    } else {
                                        Creature* chosen_creature = possible_creatures[num];

                                        if (chosen_creature->get_toughness_current() < 4) {
                                            destroy_card(chosen_creature);
                                        } else {
                                            chosen_creature->set_toughness_current(chosen_creature->get_toughness_current() - 3);
                                        }
                                        print_info("Vous venez d'infliger 3 dégâts à " + chosen_creature->get_name()  + " ! ");
                                        break;
                                    }
                                } catch (std::invalid_argument &e) {
                                    print_info("Commande invalide.");
                                }
                            }
                            
                            break;
                        } else {
                            print_info("Commande invalide.");
                        }
                        
                    }

                }
                break;
                
                case Red_ritual_effects::Damage_4_creatures: {

                    int i = 0;
                    std::vector<Creature*> possible_creatures;
                    std::vector<Creature*> chosen_creatures;

                    print_actions(m_name + ", selectionnez au plus 4 créatures pour leur infliger 1 dégâts (doublons acceptés) :", {
                            {"<id>", "pour choisir cette carte"},
                            {"reset", "pour annuler vos choix"},
                            {"valid", "pour valider vos choix"} });

                    for (auto bc : m_opponent->get_battlefield()->get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout << i << " - " << creature->get_name() << std::endl;
                            possible_creatures.push_back(creature);
                            i++;
                        }
                    }

                    std::string cmd;

                    while (true) {
                            
                        std::getline(std::cin, cmd);
                    
                        if (cmd.find("valid") != std::string::npos) {
                            for (auto creature : chosen_creatures) {
                                print_info("Vous infligez 1 dégât à " + creature->get_name() + " ! ");
                                creature->set_toughness_current(creature->get_toughness_current() - 1);
                            }
                            break;
                        } else if (cmd.find("reset") != std::string::npos) {
                            chosen_creatures = {};
                            print_info("Reset reussi");
                        } else {
                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide");
                                } else {
                                    if (contain(possible_creatures[num], chosen_creatures)) {
                                        print_info(std::to_string(num) + " deja choisie"); 
                                    } else {
                                        chosen_creatures.push_back(possible_creatures[num]);
                                        print_info("Vous avez ajouté " + possible_creatures[num-1]->get_name() + " !");

                                    }
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande Invalide");    
                            }
                        }
                    }

                }
                break;

                default:
                    break;
                
                }   
            }

        }
        break;

        case Token::Green: {

            for (auto effect : r->get_effects()) {

                switch (effect) {

                // We can play 2 lands this turn
                case Green_ritual_effects::Play_another_land:
                    
                    print_info("Vous pouvez jouer un terrain de plus ce tour-ci ! ");
                    add_played_land(-1);
                
                break;

                case Green_ritual_effects::Take_2_lands_library_shuffle: {

                    cls();
                    print();

                    int i = 0;
                    std::vector<Land*> possible_lands;
                    std::vector<Land*> chosen_lands;

                    print_actions(m_name + ", selectionnez 2 terrains de votre bibliotheque :", {
                            {"<id>", "pour choisir cette carte"},
                            {"reset", "pour annuler vos choix"},
                            {"valid", "pour valider vos choix"} });

                    for (auto c : m_library) {
                        
                        if (c->is_class(Card_class::LAND)) {

                            Land* land = dynamic_cast<Land*>(c);

                            std::cout<< i << " - " << land->get_name() <<std::endl;
                            possible_lands.push_back(land);
                            i++;
                        }
                    }

                    std::string cmd;

                    while (true) {
                            
                        std::getline(std::cin, cmd);
                    
                        if (cmd.find("valid") != std::string::npos) {
                            if(chosen_lands.size() < 2){
                                print_info("Vous n'avez pas choisi 2 terrains, continuez ! ");
                            } else {
                                for (auto land : chosen_lands) {
                                    print_info("Vous ajoutez " + land->get_name() + " dans votre main !");
                                    m_hand.push_back(land);
                                    remove( dynamic_cast<Card*>(land), m_library);
                                }
                                shuffle_library();
                                break;
                            }
                        } else if (cmd.find("reset") != std::string::npos) {
                            chosen_lands = {};
                            print_info("Reset reussi");
                        } else {
                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide");
                                } else {
                                    if(chosen_lands.size() == 2){
                                        print_info("Vous avez deja fait 2 choix, tapez valid ou reset.");
                                    } else {
                                        chosen_lands.push_back(possible_lands[num]);  
                                        print_info("Vous avez ajouté " + possible_lands[num]->get_name() + " !");   
                                    }                       
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande Invalide");    
                            }
                        }
                    }

                }
                break;
                
                default:
                    break;
                }
            }
        }
        break;

        default:
            // TODO error
            break;
    }
    destroy_card(r);
}

void Player::play_enchantment(Enchantment* e){
    
    switch (e->get_token()) {
        case Token::White: {

            for (auto effect : e->get_effects()) {

                switch (effect) {

                    case White_enchantment_effects::Win_1_HP_white:{
                        m_battlefield->place_enchantment(e);
                        print_info("Pour chaque carte blanche jouée, vous gagnerez un point de vie !");
                    }
                    break;

                    case White_enchantment_effects::Flight_Life_link:{

                        int i = 0;
                        std::vector<Creature*> possible_creatures;
                        std::vector<std::pair<std::string, std::string>> print_creatures;

                        std::string cmd;

                        while (true) {

                            cls();
                            print();

                            print_actions(m_name + ", selectionnez votre creature à enchanter pour lui ajouter Vol et Lien de vie :", {
                                {"<id>", "pour choisir cette carte"} });

                            for (auto card : m_battlefield->get_basic_cards()){
                                if(card->is_class(Card_class::CREATURE)){
                                    Creature* creature = dynamic_cast<Creature*>(card);
                                    print_creatures.push_back({std::to_string(i), creature->get_name()});
                                    possible_creatures.push_back(creature);
                                    i++;
                                }
                            }

                            print_list(print_creatures);
                                
                            std::getline(std::cin, cmd);
                        
                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide");
                                } else {
                                    possible_creatures[num]->add_enchantment(e);
                                    possible_creatures[num]->add_ability(Ability::Flight);
                                    possible_creatures[num]->add_ability(Ability::Life_link);
                                    print_info(possible_creatures[num]->get_name() + " a obtenue les capacités : Vol et Lien de vie ! ");
                                    break;
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande Invalide");    
                            }
                            
                        }                        

                    }
                    break;

                    default:
                    break;
                } 
            }
        }
        break;

        case Token::Blue:{
                    
            for (auto effect : e->get_effects()) {

                switch (effect) {
                
                    case Blue_enchantment_effects::Control_creature:{

                        int i = 0;
                        std::vector<Creature*> possible_creatures;
                        std::vector<std::pair<std::string, std::string>> print_creatures;

                        std::string cmd;

                        while (true) {

                            cls();
                            print();

                            print_actions(m_name + ", selectionnez votre creature à enchanter pour la controler :", {
                                {"<id>", "pour choisir cette carte"} });

                            for (auto card : m_opponent->get_battlefield()->get_basic_cards()){
                                if(card->is_class(Card_class::CREATURE)){
                                    Creature* creature = dynamic_cast<Creature*>(card);
                                    print_creatures.push_back({std::to_string(i), creature->get_name()});
                                    possible_creatures.push_back(creature);
                                    i++;
                                }
                            }

                            print_list(print_creatures);
                                
                            std::getline(std::cin, cmd);
                        
                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide");
                                } else {
                                    possible_creatures[num]->add_enchantment(e);
                                    m_opponent->remove_battlefield(possible_creatures[num]);
                                    m_battlefield->place_basic_card(possible_creatures[num]);
                                    print_info(possible_creatures[num]->get_name() + " est sous votre controle desormais ! ");
                                    break;
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande Invalide");    
                            }
                            
                        }   
                    }                     

                }
            }
        }
        break;

        case Token::Black:{
                    
            for (auto effect : e->get_effects()) {

                switch (effect) {
                
                    case Black_enchantment_effects::Less_HP_death_creature :{
                        m_battlefield->place_enchantment(e);
                        print_info("Chaque fois qu'une creature meurt, l'avdersaire perd un point de vie !");
                    }                     

                }
            }
        }
        break;

        case Token::Red:{
                    
            for (auto effect : e->get_effects()) {

                switch (effect) {
                
                    case Red_enchantment_effects::More_1_0_attack_creatures :{
                        m_battlefield->place_enchantment(e);
                        print_info("A chaque phase d'attaque, vos creatures gagneront 1 / 0 !");
                    }                     

                }
            }
        }
        break;

        case Token::Green:{
                    
            for (auto effect : e->get_effects()) {

                switch (effect) {
                
                    // The enchanted land give 1 more resources
                    case Green_enchantment_effects::More_1_land :{
                        
                        int i = 0;
                        std::vector<Land*> possible_lands;
                        std::vector<std::pair<std::string, std::string>> print_lands;

                        std::string cmd;

                        while (true) {

                            cls();
                            print();

                            print_actions(m_name + ", selectionnez votre land à enchanter pour la controler :", {
                                {"<id>", "pour choisir cette carte"} });

                            for (auto card : m_opponent->get_battlefield()->get_basic_cards()){
                                if(card->is_class(Card_class::LAND)){
                                    Land* land = dynamic_cast<Land*>(card);
                                    print_lands.push_back({std::to_string(i), land->get_name()});
                                    possible_lands.push_back(land);
                                    i++;
                                }
                            }

                            print_list(print_lands);
                                
                            std::getline(std::cin, cmd);
                        
                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide");
                                } else {
                                    possible_lands[num]->add_enchantment(e);
                                    possible_lands[num]->set_value(possible_lands[num]->get_value() + 1);
                                    print_info(possible_lands[num]->get_name() + " donne une ressource de plus ! ");
                                    break;
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande Invalide");    
                            }
                            
                        }

                    }
                    break;

                    // The enchanted creature win X / X, X the number of your green lands
                    case Green_enchantment_effects::More_G_G_creature :{
                        
                        int i = 0;
                        std::vector<Creature*> possible_creatures;
                        std::vector<std::pair<std::string, std::string>> print_creatures;
                        int green_lands = 0;

                        std::string cmd;

                        while (true) {

                            cls();
                            print();

                            print_actions(m_name + ", selectionnez votre land à enchanter pour la controler :", {
                                {"<id>", "pour choisir cette carte"} });

                            for (auto card : m_opponent->get_battlefield()->get_basic_cards()){
                                if(card->is_class(Card_class::CREATURE)){
                                    Creature* creature = dynamic_cast<Creature*>(card);
                                    print_creatures.push_back({std::to_string(i), creature->get_name()});
                                    possible_creatures.push_back(creature);
                                    i++;
                                } else if(card->is_class(Card_class::LAND)){
                                    Land *land = dynamic_cast<Land*>(card);
                                    if(land->get_token() == Token::Green){
                                        green_lands++;
                                    }
                                }
                            }

                            print_list(print_creatures);
                                
                            std::getline(std::cin, cmd);
                        
                            try {
                                int num = std::stoi(cmd);
                                if (num > i || num < 0) {
                                    print_info("Id invalide");
                                } else {
                                    possible_creatures[num]->add_enchantment(e);
                                    possible_creatures[num]->set_power_current(possible_creatures[num]->get_power_current() + green_lands);
                                    possible_creatures[num]->set_toughness_current(possible_creatures[num]->get_toughness_current() + green_lands);
                                    possible_creatures[num]->set_power(possible_creatures[num]->get_power() + green_lands);
                                    possible_creatures[num]->set_toughness(possible_creatures[num]->get_toughness() + green_lands);
                                    print_info(possible_creatures[num]->get_name() + " gagne " + std::to_string(green_lands) + " / " + std::to_string(green_lands) + " car vous controlez " + std::to_string(green_lands) + " forets !");
                                    break;
                                }
                            } catch (std::invalid_argument &e) {
                                print_info("Commande Invalide");    
                            }
                        }
                    }
                    break;                    

                }
            }
        }
        break;
        
        default:
        break;
    }
}


void Player::print() {
    std::cout << std::endl;
    std::cout << m_opponent->get_name() << " : " << m_opponent->get_hp() << " PV" << std::endl;

    m_opponent->get_battlefield()->print();

    std::cout<<std::endl;

    std::cout<< std::setfill('=') << std::setw(186) << "=" << std::endl << std::endl; 

    m_battlefield->print();

    std::cout << std::setfill('-') << std::setw(186) << "-" << std::endl;

    std::cout<<"Ma main"<<std::endl << std::endl;

    print_hand();

    std::cout << m_name << " : " << m_hp << " PV" << std::endl;
    std::cout << "Nb de cartes restantes dans ma bibliothèque : " << m_library.size() << std::endl;
}

void Player::print_hand(){

    sort_hand();

    std::string white_effects[] = {"Win_1_HP_white", "Flight_Life_link"};
    std::string blue_effects[]  = {"Control_creature"};
    std::string black_effects[] = {"Less_HP_death_creature"};
    std::string red_effects[]   = {"More_1_0_attack_creatures"};
    std::string green_effects[] = {"More_1_land", "More_G_G_creature"};

    std::string white_ritual_effects[] = { "More_3_HP", "More_1_1_creature_current", "Destroy_engaged_creature", "Destroy_enchantment" };
    std::string blue_ritual_effects[]  = { "Draw_2_cards", "Back_hand_creature" };
    std::string black_ritual_effects[] = { "Kill_creature", "Kill_creature_2_power", "Kill_not_angel", "Less_2_2_creature_current"};
    std::string red_ritual_effects[]   = { "Damage_3_creature_or_player", "Damage_4_creatures"};
    std::string green_ritual_effects[] = { "Play_another_land", "Take_2_lands_library_shuffle"};

    std::string abilities[] = {"Flight", "Scope", "Vigilance", "Touch_of_death", "Defender", "Initiative", "Double_initiative", "Haste", 
            "Unblockable", "Life_link", "Threat", "Trampling", "White_protection", "Blue_protection", "Black_protection", "Red_protection", "Green_protection"};

    std::string delimiter = "     ";
    std::string empty_case = "                 ";
    int wid = 15; // width of a card
    std::string top_card_basic  = "┌─────────────────┐";
    std::string down_card_basic = "└─────────────────┘";

    size_t num_card;
    int n = (m_hand.size() % 8 == 0) ? 0 : 1; // gérer le nb de lignes

    for (size_t i = 0; i < (m_hand.size() / 8) + n ; i++) {

        // print header of the card
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout<< top_card_basic;

            if (num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;
        
        // print les numéros de cartes && type de carte
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;
            int diff = num_card > 9 ? 2:1;

            // print the token on the first line background
            std::string token;
            switch (m_hand[num_card]->get_token())
            {
            case 0:
                token = get_background_color(Color::White);
            break;
            case 1:
                token = get_background_color(Color::Blue);
            break;
            case 2:
                token = get_background_color(Color::Black);
            break;
            case 3:
                token = get_background_color(Color::Red);
            break;
            case 4:
                token = get_background_color(Color::Green);
            break;
            default:
                break;
            }
            
            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {
                if(m_hand[num_card]->get_token() == 0){
                    std::cout << "│ "<< get_color(Color::Black) << token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Creature" << get_color(Color::Reset) << get_background_color(Color::Reset) << " │";
                } else {
                    std::cout << "│ "<< token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Creature" << get_background_color(Color::Reset) << " │";
                }
            } else if (m_hand[num_card]->is_class(Card_class::LAND)) {
                if(m_hand[num_card]->get_token() == 0){
                    std::cout << "│ "<< get_color(Color::Black) << token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Land" << get_color(Color::Reset) << get_background_color(Color::Reset) << " │";
                } else {
                    std::cout << "│ "<< token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Land" << get_background_color(Color::Reset) << " │";
                } 
            } else if (m_hand[num_card]->is_class(Card_class::RITUAL)) {
                if(m_hand[num_card]->get_token() == 0){
                    std::cout << "│ "<< get_color(Color::Black) << token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Ritual" << get_color(Color::Reset) << get_background_color(Color::Reset) << " │";
                } else {
                    std::cout << "│ "<< token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Ritual" << get_background_color(Color::Reset) << " │";
                } 
            } else if (m_hand[num_card]->is_class(Card_class::ENCHANTEMENT)) {
                if(m_hand[num_card]->get_token() == 0){
                    std::cout << "│ "<< get_color(Color::Black) << token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Enchantment" << get_color(Color::Reset) << get_background_color(Color::Reset) << " │";
                } else {
                    std::cout << "│ "<< token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Enchantment" << get_background_color(Color::Reset) << " │";
                } 
            }

            if(num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print nom de carte
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout << "│ " << centered_string(m_hand[num_card]->get_name(), wid) << " │";

            if(num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print cost pour creature
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::LAND)) {

                std::cout << "│ " << std::setw(wid) << " " << " │";
            } else {
                Cost* cost;
                if(m_hand[num_card]->is_class(Card_class::CREATURE)) {
                    Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                    cost = creature->get_cost();
                } else if (m_hand[num_card]->is_class(Card_class::RITUAL) || m_hand[num_card]->is_class(Card_class::ENCHANTEMENT)) {
                    SpecialCard* sp = dynamic_cast<SpecialCard*>(m_hand[num_card]);
                    cost = sp->get_cost();
                }

                std::cout<< "│ " << std::setw((wid - 11)/2) << std::setfill(' ') << " ";

                std::cout<< cost->get_any() << " ";
                std::cout<< get_background_color(Color::White) << get_color(Color::Black) << cost->get_white() << get_color(Color::Reset) << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Blue) << cost->get_blue() << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Black) << cost->get_black() << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Red) << cost->get_red() << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Green) << cost->get_green() << get_background_color(Color::Reset);
                
                std::cout<< std::setw((wid - 11)/2) << std::setfill(' ') << " " << " │";
            }

            if(num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // print type pour creature, vide pour les autres
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                std::vector<std::string> types{"Angel", "Beast", "Human_werewolf", "HippoGriff", "Kor_warrior", "Dinosaur", "Humans", "Vampire", "Spider", "Elf", "Troll"}; // TODO : mettre à jour
                std::vector<int> creat_type = creature->get_types();
                std::string s = "";

                if(creat_type.empty()){
                    std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
                } else if(creat_type.size() == 1){
                    std::cout<< "│ " << centered_string(types[creat_type[0]], wid) << " │";
                } else if(creat_type.size() == 2){
                    s = types[creat_type[0]] + types[creat_type[1]];
                    std::cout<< "│ " << centered_string(s, wid) << " │";
                } else {
                    // TODO
                }

                
            } else {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
            }

            if (num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // print first effect for specialcard, empty for the others
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::RITUAL)) {

                SpecialCard* sp = dynamic_cast<SpecialCard*>(m_hand[num_card]);

                if ((sp->get_effects()).empty()) {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
                } else {

                    int effect = (sp->get_effects())[0];

                    switch (sp->get_token()){

                        case Token::White:
                            std::cout<< "│ "<< centered_string(white_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Blue:
                            std::cout<< "│ "<< centered_string(blue_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Black:
                            std::cout<< "│ "<< centered_string(black_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Red:
                            std::cout<< "│ "<< centered_string(red_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Green:
                            std::cout<< "│ "<< centered_string(green_ritual_effects[effect], wid) << " │";
                            break;
                    
                        default:
                            break;
                    }
                }

            } else if (m_hand[num_card]->is_class(Card_class::ENCHANTEMENT)){
                SpecialCard* sp = dynamic_cast<SpecialCard*>(m_hand[num_card]);

                if ((sp->get_effects()).empty()) {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
                } else {

                    int effect = (sp->get_effects())[0];

                    switch (sp->get_token()){

                        case Token::White:
                            std::cout<< "│ "<< centered_string(white_effects[effect], wid) << " │";
                            break;

                        case Token::Blue:
                            std::cout<< "│ "<< centered_string(blue_effects[effect], wid) << " │";
                            break;

                        case Token::Black:
                            std::cout<< "│ "<< centered_string(black_effects[effect], wid) << " │";
                            break;

                        case Token::Red:
                            std::cout<< "│ "<< centered_string(red_effects[effect], wid) << " │";
                            break;

                        case Token::Green:
                            std::cout<< "│ "<< centered_string(green_effects[effect], wid) << " │";
                            break;
                    
                        default:
                            break;
                    }
                }
            } else {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
            }

            if (num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // print second effect for specialcard, 2 abilities for creature, empty for the others
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::RITUAL)) {

                SpecialCard* sp = dynamic_cast<SpecialCard*>(m_hand[num_card]);

                if ((sp->get_effects()).size() < 2) {
                    std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
                } else {

                    int effect = (sp->get_effects())[1];

                    switch (sp->get_token()){

                        case Token::White:
                            std::cout<< "│ "<< centered_string(white_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Blue:
                            std::cout<< "│ "<< centered_string(blue_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Black:
                            std::cout<< "│ "<< centered_string(black_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Red:
                            std::cout<< "│ "<< centered_string(red_ritual_effects[effect], wid) << " │";
                            break;

                        case Token::Green:
                            std::cout<< "│ "<< centered_string(green_ritual_effects[effect], wid) << " │";
                            break;
                    
                        default:
                            break;
                    }
                }

            } else if (m_hand[num_card]->is_class(Card_class::ENCHANTEMENT)){
                SpecialCard* sp = dynamic_cast<SpecialCard*>(m_hand[num_card]);

                if ((sp->get_effects()).size() < 2) {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
                } else {

                    int effect = (sp->get_effects())[1];

                    switch (sp->get_token()){

                        case Token::White:
                            std::cout<< "│ "<< centered_string(white_effects[effect], wid) << " │";
                            break;

                        case Token::Blue:
                            std::cout<< "│ "<< centered_string(blue_effects[effect], wid) << " │";
                            break;

                        case Token::Black:
                            std::cout<< "│ "<< centered_string(black_effects[effect], wid) << " │";
                            break;

                        case Token::Red:
                            std::cout<< "│ "<< centered_string(red_effects[effect], wid) << " │";
                            break;

                        case Token::Green:
                            std::cout<< "│ "<< centered_string(green_effects[effect], wid) << " │";
                            break;
                    
                        default:
                            break;
                    }
                }
            } else if (m_hand[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                std::vector<int> abilities_crea =  creature->get_abilities();
                std::string s = "";
                
                if(abilities_crea.empty()){
                    std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
                } else if(abilities_crea.size() == 1){
                    std::cout<< "│ " << centered_string(abilities[abilities_crea[0]], wid) << " │";
                } else {
                    s = abilities[abilities_crea[0]] + " " +  abilities[abilities_crea[1]];
                    std::cout<< "│ " << centered_string(s, wid) << " │";
                }
            } else {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
            }

            if (num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print abilities
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                std::vector<int> abilities_crea =  creature->get_abilities();
                std::string s = "";
                
                if(abilities_crea.size() == 3){
                    std::cout<< "│ " << centered_string(abilities[abilities_crea[2]], wid) << " │";
                } else if(abilities_crea.size() > 3){
                    s = abilities[abilities_crea[2]] + " " +  abilities[abilities_crea[3]];
                    std::cout<< "│ " << centered_string(s, wid) << " │";
                } else {
                    std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
                }
                
            } else {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
            }

            if(num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print power/toughness for creatures and value for lands
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                int pow = creature->get_power_current();
                int tough = creature->get_toughness_current();

                std::string s = std::to_string(pow) + " / " + std::to_string(tough);

                std::cout<< "│ " << std::setw(wid) << s << " │";

            } else if (m_hand[num_card]->is_class(Card_class::LAND)){
                Land* land = dynamic_cast<Land*>(m_hand[num_card]);
                
                std::cout<< "│ " << std::setw(wid) << land->get_value() << " │";
            } else {
                std::cout<< "│ " << std::setw(wid) << std::setfill(' ') << " " << " │";
            }

            if (num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // print bottom of the card
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout<< down_card_basic;

            if (num_card == m_hand.size() - 1)
                break;
            std::cout << delimiter;
        } 

        std::cout << std::endl;
        std::cout << std::endl;
    }
}
