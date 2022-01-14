#include <iostream>
#include <string>
#include <iterator>

#include "../includes/Player.hpp"
#include "../includes/Card.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Creature.hpp"
#include "../includes/Battlefield.hpp"
#include "../includes/Ritual.hpp"
#include "../includes/Enchantment.hpp"

#include "../includes/FonctionsAux.hpp"

Player::Player(std::string name): m_name(name) {
    std::cout << "[Player] : Création de " << this << std::endl;
    m_hp = 20;
}

Player::~Player() {
    std::cout << "[Player] : Denstruction de " << this << std::endl;
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

void Player::set_opponent(Player* p){
    m_opponent = p;
}

void Player::set_name(std::string s) {
    m_name = s;
}

void Player::set_hp(int i) {
    m_hp = i;
}

void Player::add_played_land(int i) {
    m_played_land += i;
}

void Player::add_hand(Card* c){
    m_hand.push_back(c);
}

void Player::remove_battlefield(Card* c){
    if(instanceof<Enchantment>(c)){
        m_battlefield.remove_enchantment(dynamic_cast<Enchantment*>(c));
    } else if(instanceof<BasicCard>(c)){
        m_battlefield.remove_basic_card(dynamic_cast<BasicCard*>(c));
    }
}

void Player::set_library(std::vector<Card*> cards) {
    m_library = cards;
}

void Player::draw_card() {
    m_hand.push_back(*m_library.begin());
    m_library.erase(element_position(*m_library.begin(), m_library) + m_library.begin());
}

void Player::discard_card(Card* c) {
    m_hand.erase(element_position(c, m_graveyard) + m_hand.begin());
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

    if(instanceof<Land>(c)){
        set_played_land(true);
        m_battlefield.place_basic_card(dynamic_cast<BasicCard*>(c));
        m_hand.erase(element_position(c, m_hand));


    } else if(instanceof<Ritual>(c)){
        play_ritual(*dynamic_cast<Ritual*>(c));
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
std::vector<Creature*> Player::attack() {

    std::cout<< "Taper -1 pour quitter cette étape"<<std::endl;

    int res;
    int i = 1;
    bool quit = false;
    std::vector<Creature*> possible_opponents;
    std::vector<Creature*> chosen_opponents;
    std::vector<Creature*> availabled_creature = m_battlefield.get_available_creatures();
    
    // List the available creatures
    for (auto creature : availabled_creature){

        // Etablish abilities
        bool defender_creature = false;
        bool haste_creature = false;

        for (auto ability_creature : creature->get_abilities()){
            if(ability_creature == Ability::Defender){
                defender_creature = true;
            } else if(ability_creature == Ability::Haste){
                haste_creature = true;
            }
        }

        if(!creature->get_engaged()){
            // Check haste ability
            if(!creature->get_is_first_turn() || haste_creature){
                // Check defender ability
                if(!defender_creature){
                    std::cout<< i <<" - "<< creature->get_name() << " : " << creature->get_power_current() << "/" << creature->get_toughness_current() <<std::endl;
                    possible_opponents.push_back(creature);
                    i++;
                }
            } else{
                // TODO error
            }
        }
       
    }

    // Interaction with player : choices
    while(!quit){
        std::cin>> res;
        if(res == -1){
            quit = true;
        } else if(res == 0){
            chosen_opponents = {};
            std::cout<< "Pour le moment, aucune creature n'attaque "<<std::endl;
        } else if(res > i || res < -1){
            std::cout<< " -!- Creature non disponible -!- "<<std::endl;
        } else{
            chosen_opponents.push_back(possible_opponents[res-1]);
            remove(possible_opponents[res - 1], availabled_creature);
        }
    }

    bool vigilance_creature = false;

    for (auto card : chosen_opponents){
        // Check the vigilance ability
        for (auto ability_card : card->get_abilities()){
            if(ability_card == Ability::Vigilance){
                vigilance_creature = true;
            }
        }
        if(!vigilance_creature){
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
void Player::choose_defenders(std::vector<Creature*> opponents){

    std::cout<< "Taper -1 pour quitter la creature ; Tapez 0 pour reinitialiser vos choix"<<std::endl;
    std::cout<< "Attention ! Choisissez vos defenseurs dans l'ordre souhaité"<<std::endl;

    std::vector<Creature*> availabled_creature = m_battlefield.get_available_creatures();

    for (auto opponent : opponents){

        int res;
        int i = 1;
        bool quit = false;
        std::vector<Creature*> possible_defenders;
        std::vector<Creature*> chosen_defenders;

        // Etablish opponent abilities
        bool threat_opponent = false;
        bool flight_opponent = false;

        for (auto ability_opponent : opponent->get_abilities()){
            if(ability_opponent == Ability::Flight){
                flight_opponent = true;
            } else if(ability_opponent == Ability::Threat){
                threat_opponent = true;
            }
        }


        for (auto creature : availabled_creature){

            // Etablish creature abilities
            bool scope_creature = false;
            bool flight_creature = false;

            for (auto ability_creature : creature->get_abilities()){
                if(ability_creature == Ability::Flight){
                    flight_creature = true;
                } else if(ability_creature == Ability::Scope){
                    scope_creature = true;
                }
            }
            
            // Check Flight ability
            if((flight_opponent && (flight_creature || scope_creature)) || !flight_opponent){
                    std::cout<< i <<" - "<< creature->get_name() << " : " << creature->get_power_current() << "/" << creature->get_toughness_current() <<std::endl;
                    possible_defenders.push_back(creature);
                    i++;
            }
        }

        // Interaction with player : choices
        while(!quit){
            std::cin>> res;
            if(res == -1){
                // Check Threat ability is respected
                if(threat_opponent && (chosen_defenders.size() == 1)){
                    std::cout<< opponent->get_name() << " vous Menace, choississez un autre defenseur ou annulez le blocage : "<<std::endl;
                } else{
                    quit = true;
                }
            } else if(res == 0){
                chosen_defenders = {};
                std::cout<< "Pour le moment, aucune creature ne defend : " << opponent->get_name()<<std::endl;
            } else if(res > i || res < -1){
                std::cout<< " -!- Creature non disponible -!- "<<std::endl;
            } else{
                chosen_defenders.push_back(possible_defenders[res-1]);
                // Remove the chosen defender from the availabled defenders
                remove(possible_defenders[res - 1], availabled_creature);
            }
        }
        
        if(!chosen_defenders.empty()){
            // Deflect attack for each opponent with the possible and chosen defender
            this->deflect_attack(*opponent, chosen_defenders);
        }
        
    }
}


void Player::deflect_attack(Creature opponent, std::vector<Creature*> defenders) {

    for (auto defender : defenders){
        // Check if the opponent is already dead
        if(contain(dynamic_cast<BasicCard*>(&opponent), m_battlefield.get_basic_cards())){
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

    for (auto ability_opponent : opponent.get_abilities()){
        if(ability_opponent == Ability::Initiative){
            initiative_opponent = true;
        } else if(ability_opponent == Ability::Touch_of_death){
            touch_of_death_opponent = true;
        } else if(ability_opponent == Ability::Double_initiative){
            double_initiative_opponent = true;
        } else if(ability_opponent == Ability::Life_link){
            life_link_opponent = true;
        } else if(ability_opponent == Ability::Trampling){
            trampling_opponent = true;
        }
    }
    for (auto ability_defender : defender.get_abilities()){
        if(ability_defender == Ability::Initiative){
            initiative_defender = true;
        } else if(ability_defender == Ability::Touch_of_death){
            touch_of_death_defender = true;
        } else if (ability_defender == Ability::Double_initiative){
            double_initiative_defender = true;
        } else if(ability_defender == Ability::Life_link){
            life_link_defender = true;
        }
    }

    // Check initiative ability and double_initiative
    if((initiative_opponent && !initiative_defender) || (double_initiative_opponent && !double_initiative_defender)){
        defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());
        // Check life_link ability
        if(life_link_opponent){
            if(toughness_defender >= opponent.get_power_current()){
                m_opponent->set_hp(m_opponent->get_hp() + opponent.get_power_current());
            } else{
                m_opponent->set_hp(m_opponent->get_hp() + toughness_defender);
            }
        }
        if(defender.get_toughness_current() <= 0){
            defender_dead = true;
            destroy_card(dynamic_cast<Card*>(&defender));
        }
    } else if ((!initiative_opponent && initiative_defender) || (!double_initiative_opponent && double_initiative_defender)){
        opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
        // Check life_link ability
        if(life_link_defender){
            if(toughness_opponent >= defender.get_power_current()){
                this->set_hp(this->get_hp() + defender.get_power_current());
            } else{
                this->set_hp(this->get_hp() + toughness_opponent);
            }
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
            if(opponent.get_power_current() > 0){
                defender.set_toughness_current(0);
                // TODO : Life link et Touch of death non compatible ?
            } 
        } else if(!initiative_opponent){
            defender.set_toughness_current(defender.get_toughness_current() - opponent.get_power_current());
            // Check Life_link ability
            if(life_link_opponent){
                if(toughness_defender >= opponent.get_power_current()){
                    m_opponent->set_hp(m_opponent->get_hp() + opponent.get_power_current());
                } else{
                    m_opponent->set_hp(m_opponent->get_hp() + toughness_defender);
                }
            }
        }
        if(touch_of_death_defender){
            if(defender.get_power_current() > 0){
                opponent.set_toughness_current(0);
            } 
        }else if(!initiative_defender){
            opponent.set_toughness_current(opponent.get_toughness_current() - defender.get_power_current());
            // Check life_link ability
            if(life_link_defender){
                if(toughness_opponent >= defender.get_power_current()){
                    this->set_hp(this->get_hp() + defender.get_power_current());
                } else{
                    this->set_hp(this->get_hp() + toughness_opponent);
                }
            }
        }

        // If the creatures are dead, deplace them into the graveyard
        if(opponent.get_toughness_current() <= 0){
            opponent_dead = true;
            destroy_card(dynamic_cast<Card*>(&opponent));
        }
        if(defender.get_toughness_current() <= 0){
            defender_dead = true;
            destroy_card(dynamic_cast<Card*>(&defender));
        }

    }

    // Check Trampling ability
    if(!opponent_dead && trampling_opponent){
        this->set_hp(this->get_hp() - opponent.get_power_current());
    }

}

void Player::destroy_card(Card* c) {

    // If c is a BasicCard, we deplace it into the graveyard
    if(instanceof<BasicCard>(c)){
        m_battlefield.set_basic_cards(dynamic_cast<BasicCard*>(c)->remove(m_battlefield.get_basic_cards()));
        m_graveyard.push_back(c);
        // We also deplace the enchantments associated to c
        for (auto e : (dynamic_cast<BasicCard*>(c))->get_enchantments()){
            m_graveyard.push_back(e);
        }
        dynamic_cast<BasicCard*>(c)->reset_enchantments();
    } 
    // If c is a Ritual, we place it into the graveyard
    else if(instanceof<Ritual>(c)){
        m_graveyard.push_back(c);
    }

}

void Player::play_ritual(Ritual r){

    switch(r.get_token()){
        case Token::White :{

            for (auto effect : r.get_effects()){

                switch(effect){

                    // Add 3 HP to the player
                    case White_ritual_effects::More_3_HP :
                        this->set_hp(this->get_hp() + 3);
                    break;

                    // All the creatures of the player win 1 power and 1 toughness for the turn
                    case White_ritual_effects::More_1_1_creature_current :{

                        for (auto bc : m_battlefield.get_basic_cards()){
                            if(instanceof<Creature>(bc)){
                                Creature creature = *dynamic_cast<Creature*>(bc);
                                creature.set_power_current(creature.get_power_current() + 1);
                                creature.set_toughness_current(creature.get_toughness_current() + 1);
                            }
                        }
                    }
                    break;

                    // The player destroy an engaged creature of its opponent
                    case White_ritual_effects::Destroy_engaged_creature :{

                        int i = 1;
                        int res;
                        bool quit = false;
                        std::vector<Creature*> possible_creatures;

                        for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                            if(instanceof<Creature>(bc)){
                                Creature creature = *dynamic_cast<Creature*>(bc);

                                if(creature.get_engaged()){
                                    std::cout<< i << " - " << creature.get_name() <<std::endl;
                                    possible_creatures.push_back(&creature);
                                    i++;
                                }
                            }
                        }

                        while (!quit){
                            std::cin>> res;
                            if(res <= i || res >= 1){
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
                    case White_ritual_effects::Destroy_enchantment :{

                        int i = 1;
                        int res;
                        bool quit = false;
                        std::vector<Enchantment*> possible_enchantments;

                        // Each enchantment on the battlefield of the opponent
                        for (auto e : (m_opponent->get_battlefield()).get_enchantments()){
                            
                            std::cout<< i << " - " << e->get_name() << " global " <<std::endl;
                            possible_enchantments.push_back(e);
                            i++;
                        }
                        // Each enchantment of a basic card on the battlefield of the opponent
                        for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                            for (auto e : bc->get_enchantments()){
                                
                                std::cout<< i << " - " << e->get_name() << " : " << bc->get_name() <<std::endl;
                                possible_enchantments.push_back(e);
                                i++;

                            }
                        }

                        while (!quit){
                            std::cin>> res;
                            if(res <= i || res >= 1){
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

        case Token::Blue  :

            for (auto effect : r.get_effects()){
                
                switch (effect){
                
                // Draw 2 cards
                case Blue_ritual_effects::Draw_2_cards :
                    
                    this->draw_card();
                    this->draw_card();

                break;

                // Return an opponent creature to the hand of the opponent
                case Blue_ritual_effects::Back_hand_creature :{

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_enchantments;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                        
                        if(instanceof<Creature>(bc)){

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature.get_name() <<std::endl;
                            possible_enchantments.push_back(&creature);
                            i++;

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
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

        case Token::Black :{
            
            for (auto effect : r.get_effects()){
                
                switch (effect){

                case Black_ritual_effects::Kill_creature :{

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                        
                        if(instanceof<Creature>(bc)){

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature.get_name() <<std::endl;
                            possible_creatures.push_back(&creature);
                            i++;

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
                            Creature* chosen_creature = possible_creatures[res - 1];
                            destroy_card(chosen_creature);
                            quit = true;
                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }

                }
                break;

                case Black_ritual_effects::Kill_creature_2_power :{

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                        
                        if(instanceof<Creature>(bc)){

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            if(creature.get_power_current() <= 2){
                                std::cout<< i << " - " << creature.get_name() <<std::endl;
                                possible_creatures.push_back(&creature);
                                i++;
                            }

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
                            Creature* chosen_creature = possible_creatures[res - 1];
                            destroy_card(chosen_creature);
                            quit = true;
                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }

                }
                break;

                case Black_ritual_effects::Kill_not_angel :{

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                        
                        if(instanceof<Creature>(bc)){

                            Creature creature = *dynamic_cast<Creature*>(bc);
                            bool is_angel = false;

                            for (auto type_creature : creature.get_types()){
                                if(type_creature == Type::Angel) is_angel = true;
                            }
                            
                            if(!is_angel){
                                std::cout<< i << " - " << creature.get_name() <<std::endl;
                                possible_creatures.push_back(&creature);
                                i++;
                            }

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
                            Creature* chosen_creature = possible_creatures[res - 1];
                            destroy_card(chosen_creature);
                            quit = true;
                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }

                }
                break;

                case Black_ritual_effects::Less_2_2_creature_current :{

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                        
                        if(instanceof<Creature>(bc)){

                            Creature creature = *dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature.get_name() <<std::endl;
                            possible_creatures.push_back(&creature);
                            i++;

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
                            Creature* chosen_creature = possible_creatures[res - 1];
                            
                            if(chosen_creature->get_power_current() < 3){
                                chosen_creature->set_power_current(0);
                            } else{
                                chosen_creature->set_power_current(chosen_creature->get_power_current() - 2);
                            }

                            if(chosen_creature->get_toughness_current() < 3){
                                destroy_card(chosen_creature);
                            } else{
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

        case Token::Red   :{

            for (auto effect : r.get_effects()){

                switch (effect){

                case Red_ritual_effects::Damage_3_creature_or_player :{

                    std::cout<< " Tapez 0 pour infliger 3 dégâts à l'adervsaire "<<std::endl;

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                        
                        if(instanceof<Creature>(bc)){

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature->get_name() <<std::endl;
                            possible_creatures.push_back(creature);
                            i++;

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
                            Creature* chosen_creature = possible_creatures[res - 1];

                            if(chosen_creature->get_toughness_current() < 4){
                                destroy_card(chosen_creature);
                            } else{
                                chosen_creature->set_toughness_current(chosen_creature->get_toughness_current() - 3);
                            }
                            quit = true;

                        } else if(res == 0){

                            m_opponent->set_hp(m_opponent->get_hp() - 3);
                            quit = true;

                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }

                }
                break;
                
                case Red_ritual_effects::Damage_4_creatures :{

                    std::cout<< " Tapez 0 pour reinitialiser vos choix "<<std::endl;

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Creature*> possible_creatures;
                    std::vector<Creature*> chosen_creatures;

                    for (auto bc : (m_opponent->get_battlefield()).get_basic_cards()){
                        
                        if(instanceof<Creature>(bc)){

                            Creature* creature = dynamic_cast<Creature*>(bc);

                            std::cout<< i << " - " << creature->get_name() <<std::endl;
                            possible_creatures.push_back(creature);
                            i++;

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
                            chosen_creatures.push_back(possible_creatures[res - 1]);

                            if(chosen_creatures.size() == 4)
                                quit = true;

                        } else if(res == 0){

                            chosen_creatures = {};
                            std::cout<< "Vos choix sont reinitialisés "<<std::endl;

                        } else {
                            std::cout<< " -- Creature non disponible -- "<<std::endl;
                        }
                    }

                    for (auto creature : chosen_creatures){
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

        case Token::Green :{

            for (auto effect : r.get_effects()){

                switch (effect){

                // We can play 2 lands this turn
                case Green_ritual_effects::Play_another_land :
                    
                    add_played_land(-1);
                
                break;

                case Green_ritual_effects::Take_2_lands_library_shuffle :{

                    std::cout<< " Tapez 0 pour reinitialiser vos choix "<<std::endl;

                    int i = 1;
                    int res;
                    bool quit = false;
                    std::vector<Land*> possible_lands;
                    std::vector<Land*> chosen_lands;

                    for (auto c : m_library){
                        
                        if(instanceof<Land>(c)){

                            Land* land = dynamic_cast<Land*>(c);

                            std::cout<< i << " - " << land->get_name() <<std::endl;
                            possible_lands.push_back(land);
                            i++;

                        }

                    }

                    while (!quit){
                        std::cin>> res;
                        if(res <= i || res >= 1){
                            chosen_lands.push_back(possible_lands[res - 1]);

                            if(chosen_lands.size() == 2)
                                quit = true;

                        } else if(res == 0){

                            chosen_lands = {};
                            std::cout<< "Vos choix sont reinitialisés "<<std::endl;

                        } else {
                            std::cout<< " -- Terrain non disponible -- "<<std::endl;
                        }
                    }

                    for (auto land : chosen_lands){
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

        default           :
            // TODO error
            break;

    }

    destroy_card(dynamic_cast<Card*>(&r));

}

void Player::loose() {
    // TODO
}

void Player::print() const{
    // TODO : print
}
