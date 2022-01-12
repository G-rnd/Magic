#include <iostream>
#include <string>

#include "../includes/Player.hpp"
#include "../includes/Card.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Creature.hpp"
#include "../includes/Battlefield.hpp"
#include "../includes/Ritual.hpp"
#include "../includes/Enchantment.hpp"

#include "FonctionsAux.cpp"

Player::Player(std::string name): m_name(name) {
    std::cout << "[Player] : Création de " << this << std::endl;
    m_hp = 20;
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

void Player::set_name(std::string s) {
    m_name = s;
}

void Player::set_hp(int i) {
    m_hp = i;
}

void Player::set_played_land(bool b) {
    m_played_land = b;
}

void Player::draw_card() {
    m_hand.push_back(*m_library.begin());
    m_library.erase(element_position(*m_library.begin(), m_library));
}

void Player::discard_card(Card* c) {
    m_hand.erase(element_position(c, m_graveyard));
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

}

void Player::play_card(Card* c) {
    // TODO : à compléter player::play_card()

    if(instanceof<Creature>(c)){
        
    } else if(instanceof<Land>(c)){

    } else if(instanceof<Ritual>(c)){

    } else if(instanceof<Enchantment>(c)){

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
std::vector<Creature> Player::attack() {

    std::cout<< "Taper -1 pour quitter cette étape"<<std::endl;

    int res;
    int i = 1;
    bool quit = false;
    std::vector<Creature> possible_opponents;
    std::vector<Creature> chosen_opponents;

    // Etablish abilities
    bool defender_creature = false;
    bool haste_creature = false;
    bool vigilance_creature = false;
    
    // List the available creatures
    for (auto creature : m_battlefield.get_available_creatures()){

        for (auto ability_creature : creature.get_abilities()){
            if(ability_creature = Ability::Defender){
                defender_creature = true;
            } else if(ability_creature = Ability::Haste){
                haste_creature = true;
            }
        }

        if(!creature.get_engaged()){
            // Check haste ability
            if(!creature.get_is_first_turn() || (creature.get_is_first_turn() && haste_creature)){
                // Check defender ability
                if(!defender_creature){
                    std::cout<< i <<" - "<< creature.get_name() <<std::endl;
                    possible_opponents.push_back(creature);
                    i++;
                }
            } else{
                // TODO error
            }
        }
       
    }

    while(!quit){
        std::cin>> res;
        if(res == -1){
            quit = true;
        } else if(res > i){
            std::cout<< " -!- Creature non disponible -!- "<<std::endl;
        } else{
            chosen_opponents.push_back(possible_opponents[res-1]);
        }
    }

    for (auto card : chosen_opponents){
        // Check the vigilance ability
        for (auto ability_card : card.get_abilities()){
            if(ability_card = Ability::Vigilance){
                vigilance_creature = true;
            }
        }
        if(!vigilance_creature){
            card.set_engaged(true);
        }
    }

    return chosen_opponents;

}

/*
- Flight
- Scope
*/
// TODO : Unblockable creatures must be removed of the vector
void Player::choose_defenders(std::vector<Creature> opponents, Player other_player){

    std::cout<< "Taper -1 pour quitter la creature"<<std::endl;
    std::cout<< "Attention ! Choisissez dans l'ordre vos defenseurs"<<std::endl;

    for (auto opponent : opponents){

        int res;
        int i = 1;
        bool quit = false;
        std::vector<Creature> possible_defenders;
        std::vector<Creature> chosen_defenders;

        for (auto ability_opponent : opponent.get_abilities()){

            if(ability_opponent = Ability::Flight){
                for (auto creature : m_battlefield.get_available_creatures()){
                    for (auto ability_creature : creature.get_abilities()){
                        if((ability_creature = Ability::Flight) || (ability_creature = Ability::Scope)){
                            std::cout<< i <<" - "<< creature.get_name() <<std::endl;
                            possible_defenders.push_back(creature);
                            i++;
                        }
                    }
                }
            } 

        }

        while(!quit){
            std::cin>> res;
            if(res == -1){
                quit = true;
            } else if(res > i){
                std::cout<< " -!- Creature non disponible -!- "<<std::endl;
            } else{
                chosen_defenders.push_back(possible_defenders[res-1]);
            }
        }

        // Deflect attack for each opponent with the possible and chosen defender
        this->deflect_attack(opponent, chosen_defenders, other_player);
        }
}

void Player::deflect_attack(Creature opponent, std::vector<Creature> defenders, Player other_player) {

    for (auto defender : defenders){
        // Check if the opponent is already dead
        if(contain(dynamic_cast<BasicCard*>(&opponent), m_battlefield.get_basic_cards())){
            battle_creature(opponent, defender, other_player);
        }
    }
    
    
}

/*
- Touch_of_death
- Initiative
- Double initiative
*/
// other_player est le joueur ayant joué opponent
void Player::battle_creature(Creature opponent, Creature defender, Player other_player) {

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

    for (auto ability_opponent : opponent.get_abilities()){
        if(ability_opponent = Ability::Initiative){
            initiative_opponent = true;
        } else if(ability_opponent = Ability::Touch_of_death){
            touch_of_death_opponent = true;
        } else if(ability_opponent = Ability::Double_initiative){
            double_initiative_opponent = true;
        } else if(ability_opponent = Ability::Life_link){
            life_link_opponent = true;
        }
    }
    for (auto ability_defender : defender.get_abilities()){
        if(ability_defender = Ability::Initiative){
            initiative_defender = true;
        } else if(ability_defender = Ability::Touch_of_death){
            touch_of_death_defender = true;
        } else if (ability_defender = Ability::Double_initiative){
            double_initiative_defender = true;
        } else if(ability_defender = Ability::Life_link){
            life_link_defender = true;
        }
    }

    // Check initiative ability and double_initiative
    if((initiative_opponent && !initiative_defender) || (double_initiative_opponent && !double_initiative_defender)){
        defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());
        // Check life_link ability
        if(life_link_opponent){
            other_player.set_hp(other_player.get_hp() + (toughness_defender - opponent.get_power_current()));
        }
        if(defender.get_toughness_current() <= 0){
            defender_dead = true;
            destroy_card(dynamic_cast<Card*>(&defender));
        }
    } else if ((!initiative_opponent && initiative_defender) || (!double_initiative_opponent && double_initiative_defender)){
        opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
        // Check life_link ability
        if(life_link_defender){
            this->set_hp(this->get_hp() + (toughness_opponent - defender.get_power_current()));
        }
        if(opponent.get_toughness_current() <= 0){
            opponent_dead = true;
            destroy_card(dynamic_cast<Card*>(&opponent));
        }
    }
    // If the battle continue
    if(!opponent_dead && !defender_dead){
        // Check touch_of_death ability
        if(touch_of_death_opponent){
            if(defender.get_toughness_current() < (defender.get_toughness_current() - opponent.get_power_current())){
                defender.set_toughness_current(0);
            } else{
                defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());
                // Check Life_link ability
                if(life_link_opponent){
                    other_player.set_hp(other_player.get_hp() + (toughness_defender - opponent.get_power_current()));
                }
            }
        }
        if(touch_of_death_defender){
            if(opponent.get_toughness_current() < (opponent.get_toughness_current() - defender.get_power_current())){
                opponent.set_toughness_current(0);
            } else{
                opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
            }
            // Check life_link ability
            if(life_link_defender){
                this->set_hp(this->get_hp() + (toughness_opponent - defender.get_power_current()));
            }
        }

        // Battle
        if((!initiative_defender && !initiative_opponent) || (initiative_opponent && initiative_defender)){
            if(!touch_of_death_opponent && !touch_of_death_opponent){
                defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());
                opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
                // Check life_link ability
                if(life_link_defender){
                    this->set_hp(this->get_hp() + (toughness_opponent - defender.get_power_current()));
                }
                if(life_link_opponent){
                    other_player.set_hp(other_player.get_hp() + (toughness_defender - opponent.get_power_current()));
                }
            }
        }

        // If the creatures are dead, deplace them into the graveyard
        if(opponent.get_toughness_current() <= 0){
            destroy_card(dynamic_cast<Card*>(&opponent));
        }
        if(defender.get_toughness_current() <= 0){
            destroy_card(dynamic_cast<Card*>(&defender));
        }
    }
}

void Player::destroy_card(Card* c) {

    // If c is a BasicCard, we deplace it into the graveyard
    if(instanceof<BasicCard*>(c)){
        m_battlefield.set_basic_cards(dynamic_cast<BasicCard*>(c)->remove(m_battlefield.get_basic_cards()));
        m_graveyard.push_back(c);
        // We also deplace the enchantments associated to c
        for (auto e : (dynamic_cast<BasicCard*>(c))->get_enchantments()){
            m_graveyard.push_back(&e);
        }
    } 
    // If c is a Ritual, we place it into the graveyard
    else if(instanceof<Ritual>(c)){
        m_graveyard.push_back(c);
    }

}

void Player::loose() {
    // TODO
}

void Player::print() const{
    // TODO : print
}
