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

#include "FonctionsAux.hpp"

Player::Player(std::string name): m_name(name) {
    //std::cout << "[Player] : Création de " << this << std::endl;
    m_hp = 20;
    m_looser = false;
}

Player::~Player() {
    //std::cout << "[Player] : Denstruction de " << this << std::endl;
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

bool Player::get_looser() const {
    return m_looser;
}

void Player::set_opponent(Player* p) {
    m_opponent = p;
}

void Player::set_name(std::string s) {
    m_name = s;
}

void Player::set_hp(int i) {
    m_hp = i;
    if(m_hp == 0) loose();
}

void Player::set_library(std::vector<Card*> cards) {
    m_library = cards;
}

void Player::set_hand(std::vector<Card*> cards) {
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
        m_battlefield.remove_enchantment(dynamic_cast<Enchantment*>(c));
    } else if (c->is_class(Card_class::CREATURE) || c->is_class(Card_class::LAND)) {
        m_battlefield.remove_basic_card(dynamic_cast<BasicCard*>(c));
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
        loose();
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

    std::vector<Card*> m_library_copy = m_library;
    int i_lib = 0;
    while(m_library_copy.size() != 0){
        int j = rand() % m_library_copy.size();
        m_library[i_lib] = m_library_copy[j];
        m_library_copy.erase(m_library_copy.begin() + j);
        i_lib++;
    }
    
    print_info("Bibliothèque mélangée.");
}

void Player::play_card(Card* c) {

    if (c->is_class(Card_class::LAND)) {
        add_played_land(1);
        m_battlefield.place_basic_card(dynamic_cast<BasicCard*>(c));
        remove(c, m_hand);
    } else if(c->is_class(Card_class::RITUAL)){
        Ritual* r = dynamic_cast<Ritual*>(c);
        play_ritual(*r);
        m_battlefield.engage_lands(r->get_cost());
    } else if(c->is_class(Card_class::ENCHANTEMENT)){

    } else if(c->is_class(Card_class::CREATURE)){
        Creature* cre = dynamic_cast<Creature*>(c);
        cre->set_is_first_turn(true);
        m_battlefield.place_basic_card(dynamic_cast<BasicCard*>(c));
        remove(c, m_hand);
        m_battlefield.engage_lands(cre->get_cost());
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

    std::cout << "Selectionnez les cartes pour attaquer :" << std::endl;
    std::cout << "<id>      : pour attaquer avec cette carte." << std::endl;
    std::cout << "reset     : pour annuler vos choix." << std::endl;
    //std::cout << "info <id> : pour avoir des informations sur une carte." << std::endl;
    std::cout << "valid      : pour valider vos choix." << std::endl << std::endl;

    int i = 0;
    std::string cmd;
    bool quit = false;
    std::vector<Creature*> possible_opponents;
    std::vector<Creature*> chosen_opponents;
    
    // List the available creatures
    for (auto creature : m_battlefield.get_available_creatures()) {

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
                    std::cout << i << " - " << creature->get_name() << std::endl;
                    possible_opponents.push_back(creature);
                    i++;
                }
            } else {
                // TODO error
            }
        }
    }

    // Interaction with player : choices
    while (!quit) {
        std::getline(std::cin, cmd);

        if (cmd.find("valid") != std::string::npos) {
            quit = true;
        } else if (cmd.find("reset") != std::string::npos) {
            chosen_opponents = {};
            std::cout<< "Reset reussi" <<std::endl;
        } else {
            try {
                int num = std::stoi(cmd);
                if (num > i || num < 0) {
                    std::cout << "Id invalide" << std::endl;

                    std::cout << "Entrée pour continuer." << std::endl;
                    std::getline(std::cin, cmd);
                } else {
                    if(contain(possible_opponents[num], chosen_opponents)){
                        std::cout << num << " deja choisie" << std::endl; 
                    } else {
                        std::cout<< "Vous venez d'ajouter " << possible_opponents[num]->get_name() << "." <<std::endl; 
                        chosen_opponents.push_back(possible_opponents[num]);
                    }
                }
            } catch (std::invalid_argument &e) {
                std::cout << "Commande Invalide" << std::endl;    
            }
        }
    }

    bool vigilance_creature = false;

    for (auto card : chosen_opponents) {
        // Check the vigilance ability
        for (auto ability_card : card->get_abilities()) {
            if (ability_card == Ability::Vigilance) {
                vigilance_creature = true;
            }
        }
        if (!vigilance_creature) {
            card->set_engaged(true);
        }
    }
    return chosen_opponents;
}

/*
- Flight
- Scope
- Threat
*/
// TODO : Unblockable creatures must be removed of the vector
void Player::choose_defenders(std::vector<Creature*> opponents) {

    std::cout << "Selectionnez les cartes pour défendre :" << std::endl;
    std::cout << "<id>      : pour défendre avec cette carte." << std::endl;
    std::cout << "reset     : pour annuler vos choix." << std::endl;
    //std::cout << "info <id> : pour avoir des informations sur une carte." << std::endl;
    std::cout << "end       : pour ne pas defendre cette carte." << std::endl << std::endl;
    // TODO : validation
    std::cout<< "Attention ! Choisissez vos defenseurs dans l'ordre souhaité"<<std::endl<<std::endl;

    std::vector<Creature*> availabled_creature = m_battlefield.get_available_creatures();

    for (auto opponent : opponents) {

        std::string cmd;
        int i = 0;
        bool quit = false;
        std::vector<Creature*> possible_defenders;
        std::vector<Creature*> chosen_defenders;

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
                std::cout << i << " - " << creature->get_name() << " : " << creature->get_power_current() << "/" << creature->get_toughness_current() << std::endl;
                possible_defenders.push_back(creature);
                i++;
            }
        }

        // Interaction with player : choices
        while (!quit) {
            std::getline(std::cin, cmd);
            
            if (cmd.find("end") != std::string::npos) {
                // Check Threat ability is respected
                if (threat_opponent && (chosen_defenders.size() == 1)) {
                    std::cout << opponent->get_name() << " vous Menace, choississez un autre defenseur ou annulez le blocage : " << std::endl;
                } else {
                    quit = true;
                }
            } else if (cmd.find("reset") != std::string::npos) {
                chosen_defenders = {};
                std::cout<< "Reset reussi" <<std::endl;
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > i || num < 0) {
                        std::cout << "Id invalide" << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else {
                        if (contain(possible_defenders[num], chosen_defenders)) {
                            std::cout << num <<" deja choisie" << std::endl; 
                        } else {
                            std::cout<< "Vous venez d'ajouter " << possible_defenders[num]->get_name() << "."<<std::endl;
                            chosen_defenders.push_back(possible_defenders[num]);
                        }
                    }
                } catch (std::invalid_argument &e) {
                    std::cout << "Commande Invalide" << std::endl;    
                }
            }
        }

        if (!chosen_defenders.empty()) {
            // Deflect attack for each opponent with the possible and chosen defender
            this->deflect_attack(*opponent, chosen_defenders);
        }
    }
}

void Player::deflect_attack(Creature opponent, std::vector<Creature*> defenders) {

    for (auto defender : defenders) {
        // Check if the opponent is already dead
        if(contain(dynamic_cast<BasicCard*>(&opponent), m_battlefield.get_basic_cards())) {
            battle_creature(opponent, *defender);
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
void Player::battle_creature(Creature opponent, Creature defender) {

    bool opponent_dead = false;
    bool defender_dead = false;

    int toughness_defender = defender.get_toughness_current();
    int toughness_opponent = opponent.get_toughness_current();

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

    for (auto ability_opponent : opponent.get_abilities()) {
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

    for (auto ability_defender : defender.get_abilities()) {
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
        defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());
        // Check life_link ability
        if (life_link_opponent) {
            if (toughness_defender >= opponent.get_power_current()) {
                m_opponent->set_hp(m_opponent->get_hp() + opponent.get_power_current());
            } else {
                m_opponent->set_hp(m_opponent->get_hp() + toughness_defender);
            }
        }
        if (defender.get_toughness_current() <= 0) {
            defender_dead = true;
            destroy_card(&defender);
        }
    } else if ((!initiative_opponent && initiative_defender) || (!double_initiative_opponent && double_initiative_defender)) {
        opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
        // Check life_link ability
        if (life_link_defender) {
            if (toughness_opponent >= defender.get_power_current()) {
                this->set_hp(this->get_hp() + defender.get_power_current());
            } else {
                this->set_hp(this->get_hp() + toughness_opponent);
            }
        }
        if (opponent.get_toughness_current() <= 0) {
            opponent_dead = true;
            destroy_card(&opponent);
        }
    }
    // If the battle continue
    if (!opponent_dead && !defender_dead) {

        // Check touch_of_death ability
        if (touch_of_death_opponent) {
            if (opponent.get_power_current() > 0) {
                defender.set_toughness_current(0);
                // TODO : Life link et Touch of death non compatible ?
            } 
        } else if (!initiative_opponent) {
            defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());
            // Check Life_link ability
            if (life_link_opponent) {
                if (toughness_defender >= opponent.get_power_current()) {
                    m_opponent->set_hp(m_opponent->get_hp() + opponent.get_power_current());
                } else {
                    m_opponent->set_hp(m_opponent->get_hp() + toughness_defender);
                }
            }
        }
        if (touch_of_death_defender) {
            if (defender.get_power_current() > 0) {
                opponent.set_toughness_current(0);
            } 
        } else if (!initiative_defender) {
            opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
            // Check life_link ability
            if (life_link_defender) {
                if (toughness_opponent >= defender.get_power_current()) {
                    this->set_hp(this->get_hp() + defender.get_power_current());
                } else {
                    this->set_hp(this->get_hp() + toughness_opponent);
                }
            }
        }

        // If the creatures are dead, deplace them into the graveyard
        if (opponent.get_toughness_current() <= 0) {
            opponent_dead = true;
            destroy_card(&opponent);
        }
        if (defender.get_toughness_current() <= 0) {
            defender_dead = true;
            destroy_card(&defender);
        }
    }

    // Check Trampling ability
    if (!opponent_dead && trampling_opponent) {
        this->set_hp(this->get_hp() - opponent.get_power_current());
    }
}

void Player::destroy_card(Card* c) {

    // If c is a BasicCard, we deplace it into the graveyard
    if (c->is_class(Card_class::LAND) || c->is_class(Card_class::CREATURE)) {
        m_battlefield.set_basic_cards(dynamic_cast<BasicCard*>(c)->remove(m_battlefield.get_basic_cards()));
        m_graveyard.push_back(c);
        // We also deplace the enchantments associated to c
        for (auto e : (dynamic_cast<BasicCard*>(c))->get_enchantments()) {
            m_graveyard.push_back(e);
        }
        dynamic_cast<BasicCard*>(c)->reset_enchantments();
    } 
    // If c is a Ritual, we place it into the graveyard
    else if (c->is_class(Card_class::RITUAL)) {
        m_graveyard.push_back(c);
    }
}

void Player::play_ritual(Ritual r) {

    switch (r.get_token()) {
        case Token::White: {

            for (auto effect : r.get_effects()) {

                switch (effect) {

                    // Add 3 HP to the player
                    case White_ritual_effects::More_3_HP:
                        this->set_hp(this->get_hp() + 3);
                        break;

                    // All the creatures of the player win 1 power and 1 toughness for the turn
                    case White_ritual_effects::More_1_1_creature_current: {

                        for (auto bc : m_battlefield.get_basic_cards()) {
                            if (bc->is_class(Card_class::CREATURE)) {
                                Creature creature = *dynamic_cast<Creature*>(bc);
                                creature.set_power_current(creature.get_power_current() + 1);
                                creature.set_toughness_current(creature.get_toughness_current() + 1);
                            }
                        }
                    }
                    break;

                    // The player destroy an engaged creature of its opponent
                    case White_ritual_effects::Destroy_engaged_creature: {

                        int i = 1;
                        int res;
                        bool quit = false;
                        std::vector<Creature*> possible_creatures;

                        for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                            if (bc->is_class(Card_class::CREATURE)) {
                                Creature creature = *dynamic_cast<Creature*>(bc);

                                if (creature.get_engaged()) {
                                    std::cout<< i << " - " << creature.get_name() <<std::endl;
                                    possible_creatures.push_back(&creature);
                                    i++;
                                }
                            }
                        }

                        while (!quit) {
                            std::cin >> res;
                            if (res <= i || res >= 1) {
                                Creature* chosen_creature = possible_creatures[res - 1];
                                // TODO : tester si delete suppr des listes de battlefield
                                delete chosen_creature;
                                quit = true;
                            } else {
                                std::cout<< " -- Creature non disponible -- "<<std::endl;
                            }
                        }
                    }
                    break;

                    // The player destroy an enchantment of its opponent
                    case White_ritual_effects::Destroy_enchantment: {

                        int i = 1;
                        int res;
                        bool quit = false;
                        std::vector<Enchantment*> possible_enchantments;

                        // Each enchantment on the battlefield of the opponent
                        for (auto e : (m_opponent->get_battlefield()).get_enchantments()) {
                            
                            std::cout<< i << " - " << e->get_name() << " global " <<std::endl;
                            possible_enchantments.push_back(e);
                            i++;
                        }
                        // Each enchantment of a basic card on the battlefield of the opponent
                        for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                            for (auto e : bc->get_enchantments()) {
                                
                                std::cout<< i << " - " << e->get_name() << " : " << bc->get_name() <<std::endl;
                                possible_enchantments.push_back(e);
                                i++;

                            }
                        }

                        while (!quit) {
                            std::cin>> res;
                            if(res <= i || res >= 1) {
                                Enchantment* chosen_enchantment = possible_enchantments[res - 1];
                                // TODO : tester si delete suppr des listes
                                delete chosen_enchantment;
                                quit = true;
                            } else {
                                std::cout<< " -- Enchantement non disponible -- "<<std::endl;
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

            for (auto effect : r.get_effects()) {
                
                switch (effect) {
                
                // Draw 2 cards
                case Blue_ritual_effects::Draw_2_cards:
                    
                    this->draw_card();
                    this->draw_card();

                break;

                // Return an opponent creature to the hand of the opponent
                case Blue_ritual_effects::Back_hand_creature: {

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_enchantments;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature.get_name() <<std::endl;
                            possible_enchantments.push_back(&creature);
                            i++;
                        }
                    }

                    while (!quit) {
                        std::cin>> res;
                        if (res <= i || res >= 1) {
                            Creature* chosen_enchantment = possible_enchantments[res - 1];
                            m_opponent->add_hand(chosen_enchantment);
                            m_opponent->remove_battlefield(chosen_enchantment);
                            quit = true;
                        } else {
                            std::cout<< " -- Enchantement non disponible -- "<<std::endl;
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
            
            for (auto effect : r.get_effects()) {
                
                switch (effect){

                case Black_ritual_effects::Kill_creature: {

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature.get_name() <<std::endl;
                            possible_creatures.push_back(&creature);
                            i++;
                        }
                    }

                    while (!quit) {
                        std::cin>> res;
                        if (res <= i || res >= 1) {
                            Creature* chosen_creature = possible_creatures[res - 1];
                            destroy_card(chosen_creature);
                            quit = true;
                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }
                }
                break;

                case Black_ritual_effects::Kill_creature_2_power: {

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            if (creature.get_power_current() <= 2) {
                                std::cout<< i << " - " << creature.get_name() <<std::endl;
                                possible_creatures.push_back(&creature);
                                i++;
                            }
                        }
                    }

                    while (!quit) {
                        std::cin >> res;
                        if (res <= i || res >= 1) {
                            Creature* chosen_creature = possible_creatures[res - 1];
                            destroy_card(chosen_creature);
                            quit = true;
                        } else {
                            std::cout << " -- Creature non disponible -- " << std::endl;
                        }
                    }
                }
                break;

                case Black_ritual_effects::Kill_not_angel: {

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature creature = *dynamic_cast<Creature*>(bc);
                            bool is_angel = false;

                            for (auto type_creature : creature.get_types()) {
                                if(type_creature == Type::Angel) is_angel = true;
                            }
                            
                            if (!is_angel) {
                                std::cout << i << " - " << creature.get_name() << std::endl;
                                possible_creatures.push_back(&creature);
                                i++;
                            }
                        }
                    }

                    while (!quit) {
                        std::cin>> res;
                        if (res <= i || res >= 1) {
                            Creature* chosen_creature = possible_creatures[res - 1];
                            destroy_card(chosen_creature);
                            quit = true;
                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }
                }
                break;

                case Black_ritual_effects::Less_2_2_creature_current: {

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature.get_name() <<std::endl;
                            possible_creatures.push_back(&creature);
                            i++;
                        }
                    }

                    while (!quit) {
                        std::cin >> res;
                        if (res <= i || res >= 1) {
                            Creature* chosen_creature = possible_creatures[res - 1];
                            
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

                            quit = true;
                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
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

            for (auto effect : r.get_effects()) {

                switch (effect) {

                case Red_ritual_effects::Damage_3_creature_or_player: {

                    std::cout << " Tapez 0 pour infliger 3 dégâts à l'adervsaire " << std::endl;

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature->get_name() <<std::endl;
                            possible_creatures.push_back(creature);
                            i++;
                        }
                    }

                    while (!quit) {
                        std::cin>> res;
                        if (res <= i || res >= 1) {
                            Creature* chosen_creature = possible_creatures[res - 1];

                            if (chosen_creature->get_toughness_current() < 4) {
                                destroy_card(chosen_creature);
                            } else {
                                chosen_creature->set_toughness_current(chosen_creature->get_toughness_current() - 3);
                            }
                            quit = true;

                        } else if(res == 0) {

                            m_opponent->set_hp(m_opponent->get_hp() - 3);
                            quit = true;

                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }
                }
                break;
                
                case Red_ritual_effects::Damage_4_creatures: {

                    std::cout << " Tapez 0 pour reinitialiser vos choix " << std::endl;

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;
                    std::vector<Creature*> chosen_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()) {
                        
                        if (bc->is_class(Card_class::CREATURE)) {

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout << i << " - " << creature->get_name() << std::endl;
                            possible_creatures.push_back(creature);
                            i++;
                        }
                    }

                    while (!quit) {
                        std::cin >> res;
                        if (res <= i || res >= 1) {
                            chosen_creatures.push_back(possible_creatures[res - 1]);

                            if (chosen_creatures.size() == 4)
                                quit = true;

                        } else if (res == 0) {

                            chosen_creatures = {};
                            std::cout<< "Vos choix sont reinitialisés "<<std::endl;

                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }

                    for (auto creature : chosen_creatures) {
                        creature->set_toughness_current(creature->get_toughness_current() - 1);
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

            for (auto effect : r.get_effects()) {

                switch (effect) {

                // We can play 2 lands this turn
                case Green_ritual_effects::Play_another_land:
                    
                    add_played_land(-1);
                
                break;

                case Green_ritual_effects::Take_2_lands_library_shuffle: {

                    std::cout << " Tapez 0 pour reinitialiser vos choix " << std::endl;

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Land*> possible_lands;
                    std::vector<Land*> chosen_lands;

                    for (auto c : m_library) {
                        
                        if (c->is_class(Card_class::LAND)) {

                            Land* land = dynamic_cast<Land*>(c);

                            std::cout<< i << " - " << land->get_name() <<std::endl;
                            possible_lands.push_back(land);
                            i++;
                        }
                    }

                    while (!quit) {
                        std::cin>> res;
                        if(res <= i || res >= 1) {
                            chosen_lands.push_back(possible_lands[res - 1]);

                            if(chosen_lands.size() == 2)
                                quit = true;

                        } else if (res == 0) {

                            chosen_lands = {};
                            std::cout<< "Vos choix sont reinitialisés "<<std::endl;

                        } else {
                            std::cout<< " -- Terrain non disponible -- "<<std::endl;
                        }
                    }

                    for (auto land : chosen_lands) {
                        m_hand.push_back(land);
                        remove( dynamic_cast<Card*>(land), m_library);
                    }
                    shuffle_library();
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
    destroy_card(&r);
}

void Player::loose() {
    // TODO : set_looser
    m_looser = true;
}

void Player::print(){
    
    std::cout << std::endl;
    std::cout << m_opponent->get_name() << " : " << m_opponent->get_hp() << " PV" << std::endl;

    m_opponent->get_battlefield().print();

    std::cout<<std::endl;

    std::cout<< std::setfill('=') << std::setw(147) << "=" << std::endl << std::endl; 

    m_battlefield.print();

    std::cout << std::setfill('-') << std::setw(147) << "-" << std::endl;

    std::cout<<"Ma main"<<std::endl << std::endl;

    print_hand();

    std::cout << m_name << " : " << m_hp << " PV" << std::endl;
    std::cout << "Nb de cartes restantes dans ma bibliothèque : " << m_library.size() << std::endl;

    std::string s;
    std::cout << "Entrée pour continuer." << std::endl;
    std::getline(std::cin, s);
}

void Player::print_hand(){

    sort_hand();

    std::string tokens[5] = {"White", "Blue", "Black", "Red", "Green"};

    std::string white_effects[] = {"Win_1_HP_white", "Flight_Life_link"};
    std::string blue_effects[]  = {"Control_creature"};
    std::string black_effects[] = {"Less_HP_death_creature"};
    std::string red_effects[]   = {"More_1_0_attack_creatures"};
    std::string green_effects[] = {"More_1_land", "More_G_G_creature"};
    
    std::string delimiter = "     ";
    std::string empty_case = "              ";
    int num_card;
    int n = (m_hand.size() % 8 == 0) ? 0 : 1; // gérer le nb de lignes

    for (int i = 0; i < ((int) m_hand.size() / 8) + n ; i++) {

        // print les numéros de cartes
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;
            std::cout << "[" << std::setfill(' ') << std::setw(12) << num_card << "]";

            if(num_card == ((int) m_hand.size() - 1)) 
                break;
            std::cout << delimiter;
        }
        std::cout << std::endl;
        
        // print type de carte
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {
                std::cout << "[" << std::setw(12) << "Creature" << "]";
            } else if (m_hand[num_card]->is_class(Card_class::LAND)) {
                std::cout << "[" << std::setw(12) << "Land" << "]";
            } else if (m_hand[num_card]->is_class(Card_class::ENCHANTEMENT)) {
                std::cout << "[" << std::setw(12) << "Enchantement" << "]";
            } else if (m_hand[num_card]->is_class(Card_class::RITUAL)) {
                std::cout << "[" << std::setw(12) << "Rituel" << "]";
            } 

            if (num_card == (int) m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print nom de carte
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::string s = (m_hand[num_card]->get_name()).substr(0, 12);

            std::cout<< "[" << std::setw(12) << s << "]";

            if (num_card == (int) m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print token
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            int token = m_hand[num_card]->get_token();
            std::cout << "[" << std::setw(12) << tokens[token] << "]";

            if (num_card == (int) m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        //print cost
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::LAND)) {

                Land* land = dynamic_cast<Land*>(m_hand[num_card]);
                std::cout<< "[" << std::setw(12) << land->get_value() << "]";
            } else {

                Cost* cost;
                if (m_hand[num_card]->is_class(Card_class::CREATURE)) {
                    cost = (dynamic_cast<Creature*>(m_hand[num_card]))->get_cost();
                } else {
                    cost = (dynamic_cast<Creature*>(m_hand[num_card]))->get_cost();
                }

                std::string s = "";

                if (!cost->is_any_null())
                    s += std::to_string(cost->get_any()) + "*";

                if (!cost->is_white_null())
                    s += std::to_string(cost->get_white()) + "W";

                if (!cost->is_blue_null())
                    s += std::to_string(cost->get_blue()) + "B";

                if (!cost->is_black_null())
                    s += std::to_string(cost->get_black()) + "N";

                if (!cost->is_red_null())
                    s += std::to_string(cost->get_red()) + "R";

                if (!cost->is_green_null())
                    s += std::to_string(cost->get_green()) + "G";
                    
                std::cout << "[" << std::setw(12) << s << "]";
            }

            if (num_card == (int) m_hand.size() - 1)
              break;
            std::cout << delimiter;
        }
        std::cout << std::endl;

        // print type pour creature, value pour land et effects pour les specialCard
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                std::string types[1] = {"Angel"};
                std::vector<int> creat_type =  creature->get_types();
                std::string s = "";
                for (auto t : creat_type) {
                    s += types[t].substr(0, 3);
                    s += "-";
                }
                s = s.substr(0, s.size() - 2);
                std::cout<< "[" << std::setw(12) << s.substr(0,12) << "]";

            } else if (m_hand[num_card]->is_class(Card_class::LAND)) {
                std::cout<< empty_case;
            } else {
                SpecialCard *sc = dynamic_cast<SpecialCard*>(m_hand[num_card]);
                if ((sc->get_effects()).empty()) {
                    std::cout<< empty_case;
                } else {

                    int effect = (sc->get_effects())[0];

                    switch (sc->get_token()) {

                        case Token::White:
                            std::cout << "[" << std::setw(12) << white_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Blue:
                            std::cout << "[" << std::setw(12) << blue_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Black:
                            std::cout << "[" << std::setw(12) << black_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Red:
                            std::cout << "[" << std::setw(12) << red_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Green:
                            std::cout << "[" << std::setw(12) << green_effects[effect].substr(0, 12) << "]";
                            break;
                    
                        default:
                            break;
                    }
                }
            }

            if (num_card == (int) m_hand.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // print power/toughness
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                int pow = creature->get_power_current();
                int tough = creature->get_toughness_current();

                std::string s = std::to_string(pow) + " / " + std::to_string(tough);
        
                std::cout << "[" << std::setw(12) << s.substr(0,12) << "]";

            } else if (m_hand[num_card]->is_class(Card_class::LAND)) {
                std::cout << empty_case;
            } else {
                SpecialCard *sc = dynamic_cast<SpecialCard*>(m_hand[num_card]);
                if ((sc->get_effects()).size() < 2) {
                    std::cout<< empty_case;
                } else {

                    int effect = (sc->get_effects())[1];

                    switch (sc->get_token()){

                        case Token::White:
                            std::cout << "[" << std::setw(12) << white_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Blue:
                            std::cout << "[" << std::setw(12) << blue_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Black:
                            std::cout << "[" << std::setw(12) << black_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Red:
                            std::cout << "[" << std::setw(12) << red_effects[effect].substr(0, 12) << "]";
                            break;

                        case Token::Green:
                            std::cout << "[" << std::setw(12) << green_effects[effect].substr(0, 12) << "]";
                            break;
                    
                        default:
                            break;
                    }
                }
            }

            if (num_card == (int) m_hand.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // print abilities
        for (int j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_hand[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_hand[num_card]);
                std::string abilities[] = {"Flight", "Scope", "Vigilance", "Touch_of_death", "Defender", "Initiative", "Double_initiative", "Haste", 
                           "Unblockable", "Life_link", "Threat", "Trampling", "White_protection", "Blue_protection", "Black_protection", "Red_protection", "Green_protection"};
                std::vector<int> abilities_crea =  creature->get_abilities();
                std::string s = "";
                for (auto a : abilities_crea) {
                    s += abilities[a].substr(0, 3);
                    s += "-";
                }
                s = s.substr(0, s.size() - 2);
                std::cout << "[" << std::setw(12) << s.substr(0,12) << "]";
            } else {
                std::cout << empty_case;
            }

            if (num_card == (int) m_hand.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;
        std::cout << std::endl;
    }
}
