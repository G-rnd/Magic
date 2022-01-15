#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip> 

#include "../includes/Battlefield.hpp"
#include "../includes/BasicCard.hpp"
#include "../includes/Creature.hpp"
#include "../includes/Land.hpp"
#include "../includes/Enchantment.hpp"

#include "../includes/FonctionsAux.hpp"

Battlefield::Battlefield() {
    //std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    //std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard*> Battlefield::get_basic_cards() const{
    return m_basic_cards;
}

std::vector<Enchantment*> Battlefield::get_enchantments() const{
    return m_enchantments;
}

void Battlefield::set_basic_cards(std::vector<BasicCard*> bc){
    m_basic_cards = bc;
}

void Battlefield::set_enchantments(std::vector<Enchantment*> e){
    m_enchantments = e;
}

void Battlefield::sort_basic_cards(){

    std::vector<BasicCard*> bc_sort;

    for (auto bc : m_basic_cards){
        if(bc->is_class(Card_class::LAND)){
            bc_sort.push_back(bc);
        }
    }
    for (auto bc : m_basic_cards){
        if(bc->is_class(Card_class::CREATURE)){
            bc_sort.push_back(bc);
        }
    }
    
    set_basic_cards(bc_sort);

}

void Battlefield::remove_basic_card(BasicCard* bc){
    remove(bc, m_basic_cards);
}

void Battlefield::remove_enchantment(Enchantment* e){
    remove(e, m_enchantments);
}

std::vector<Creature*> Battlefield::get_engaged_creatures() {
    std::vector<Creature*> v = {};

    for (auto card : m_basic_cards) {
        if(card->is_class(Card_class::CREATURE) && card->get_engaged()) {
            v.push_back(dynamic_cast<Creature*>(card));
        }
    }
    
    return v;
}

std::vector<Creature*> Battlefield::get_available_creatures() {
    std::vector<Creature*> v = {};

    for (auto card : m_basic_cards) {
        if(card->is_class(Card_class::CREATURE) && !card->get_engaged()){
            Creature* c = dynamic_cast<Creature*>(card);
            if(!c->get_is_first_turn()){
                v.push_back(c);
            }
        }
    }

    return v;
}
std::vector<Land*> Battlefield::get_available_lands() {
    std::vector<Land*> v = {};
    
    for (auto card : m_basic_cards) {
        if(card->is_class(Card_class::LAND) && !card->get_engaged()){
            v.push_back(dynamic_cast<Land*>(card));
        }
    }
    
    return v;
}

void Battlefield::place_basic_card(BasicCard* bc) {
    if(bc != nullptr)
        m_basic_cards.push_back(bc);
    // TODO : engager les terrains
}

void Battlefield::disengage_card(BasicCard* bc){
    bc->set_engaged(false);
}

// Renvoie vrai si le terrain comporte assez de cartes terrains pour jouer la carte c
bool Battlefield::is_playable(Card* card) {

    Cost c = Cost(0, 0, 0, 0, 0, 0);

    switch (card->get_class()){

    case Card_class::CREATURE :
        c = *(dynamic_cast<Creature*>(card)->get_cost());
    break;

    case Card_class::RITUAL :
        c = *(dynamic_cast<Ritual*>(card)->get_cost());
    break;

    case Card_class::ENCHANTEMENT :
        c = *(dynamic_cast<Enchantment*>(card)->get_cost());
    break;
    
    default:
        break;
    }

    std::vector<Land*> lands = get_available_lands();
    for(long unsigned int i = 0; i < lands.size() && !c.is_null(); i++) {
        // Si la carte n'a pas besoin de terrains typés
        if (c.is_color_null()) {
            c.set_any(c.get_any() - lands[i]->get_value());
            lands.erase(lands.begin() + i);
            i--;
        } else {
            // On commence par compter les terrains typés
            switch(lands[i]->get_token()){

                case Token::White:
                    c.set_white(c.get_white() - lands[i]->get_value());
                break;

                case Token::Blue:
                    c.set_blue(c.get_blue() - lands[i]->get_value());
                break;

                case Token::Black:
                    c.set_black(c.get_black() - lands[i]->get_value());
                break;

                case Token::Red:
                    c.set_red(c.get_red() - lands[i]->get_value());
                break;

                case Token::Green:
                    c.set_green(c.get_green() - lands[i]->get_value()); 
                break;

                default:
                    break;

            }
            lands.erase(lands.begin() + i);
        }
    }
    return c.is_null();
}

void Battlefield::engage_lands(Cost* c){

    std::vector<Land*> lands = get_available_lands();
    std::vector<int> needed = {c->get_white(), c->get_blue(), c->get_black(), c->get_red(), c->get_green(), c->get_any()};

    for (auto land : lands){
        int value = land->get_value();
        if(needed[land->get_token()] >= value){
            std::cout<<"BB"<<std::endl;
            land->set_engaged(true);
            //remove(land, lands);
            needed[land->get_token()] = needed[land->get_token()] - value; 
        } else if(needed[5] >= value){ //any needed
            std::cout<<"CC"<<std::endl;
            land->set_engaged(true);
            //remove(land, lands);
            needed[5] = needed[5] - value; 
        }
    }
/*
    //Check if more lands are needed
    bool needed_more = false;
    for (size_t i = 0; i < needed.size(); i++){
        needed_more = needed_more || (needed[i] != 0);
    }
    
    if(needed_more){
        for (auto land : lands){
            if(needed[land->get_token()] >= 1){
                land->set_engaged(true);
                remove(land, lands);
                needed[land->get_token()] = needed[land->get_token()] - land->get_value(); 
            } else if(needed[5] >= 1){ //any needed
                land->set_engaged(true);
                remove(land, lands);
                needed[5] -= needed[5] - land->get_value(); 
            }
        }   
    }*/

}

void Battlefield::print() {

    sort_basic_cards();

    std::string tokens[5] = {"White", "Blue", "Black", "Red", "Green"};
    
    std::string delimiter = "     ";
    std::string empty_case = "              ";
    int num_card;
    int n = (m_basic_cards.size() % 8 == 0) ? 0:1 ; // gérer le nb de lignes

    // print les basics_cards
    for (int i = 0; i < ((int) m_basic_cards.size() / 8) + n ; i++){

        // print les numéros de cartes
        for (int j = 0; j < 8; j++){

            num_card = i*8 + j;

            //std::cout<< m_basic_cards[num_card] << " : " << num_card << std::endl;

            if(m_basic_cards[num_card]->get_enchantments().empty()){
                if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                    std::cout<< "[*" << std::setfill(' ') << std::setw(10) << num_card << "*]";
                } else{
                    std::cout<< "[" << std::setfill(' ') << std::setw(12) << num_card << "]";
                }
            } else {
                if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                    std::cout<< "{*" << std::setfill(' ') << std::setw(10) << num_card << "*}";
                } else{
                    std::cout<< "{" << std::setfill(' ') << std::setw(12) << num_card << "}";
                }
            }

            if(num_card == ((int) m_basic_cards.size() - 1)) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print type de carte
        for (int j = 0; j < 8; j++){

            num_card = i*8 + j;

            if(m_basic_cards[num_card]->is_class(Card_class::CREATURE)){

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                if(creature->get_enchantments().empty()){
                    if(creature->get_engaged() || creature->get_is_first_turn()){
                        std::cout<< "[*" << std::setw(10) << "Creature" << "*]";
                    } else{
                        std::cout<< "[" << std::setw(12) << "Creature" << "]";
                    }
                } else {
                    if(m_basic_cards[num_card]->get_engaged() || (creature->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "{*" << std::setw(10) << "Creature" << "*}";
                    } else{
                        std::cout<< "{" << std::setw(12) << "Creature" << "}";
                    }
                }

            } else if(m_basic_cards[num_card]->is_class(Card_class::LAND)){

                Land* land = dynamic_cast<Land*>(m_basic_cards[num_card]);
                if(land->get_enchantments().empty()){
                    if(land->get_engaged()){
                        std::cout<< "[*" << std::setw(10) << "Land " << "*]";
                    } else{
                        std::cout<< "[" << std::setw(12) << "Land" << "]";
                    }
                } else {
                    if(m_basic_cards[num_card]->get_engaged()){
                        std::cout<< "{*" << std::setw(10) << "Land" << "*}";
                    } else{
                        std::cout<< "{" << std::setw(12) << "Land" << "}";
                    }
                }

            }

            if(num_card == (int) m_basic_cards.size() - 1) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print nom de carte
        for (int j = 0; j < 8; j++){

            num_card = i*8 + j;

            std::string s = (m_basic_cards[num_card]->get_name()).substr(0, 12);

            if(m_basic_cards[num_card]->get_enchantments().empty()){
                if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                    std::cout<< "[*" << std::setw(10) << s.substr(0, 10) << "*]";
                } else{
                    std::cout<< "[" << std::setw(12) <<  s << "]";
                }
            } else {
                if(m_basic_cards[num_card]->get_engaged()){
                    std::cout<< "{*" << std::setw(10) <<  s.substr(0, 10) << "*}";
                } else{
                    std::cout<< "{" << std::setw(12) <<  s << "}";
                }
            }

            if(num_card == (int) m_basic_cards.size() - 1) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print token
        for (int j = 0; j < 8; j++){

            num_card = i*8 + j;

            int token = m_basic_cards[num_card]->get_token();
            if(m_basic_cards[num_card]->get_enchantments().empty()){
                if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                    std::cout<< "[*" << std::setw(10) << tokens[token] << "*]";
                } else{
                    std::cout<< "[" << std::setw(12) << tokens[token] << "]";
                }
            } else {
                if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                    std::cout<< "{*" << std::setw(10) << tokens[token]  << "*}";
                } else{
                    std::cout<< "{" << std::setw(12) << tokens[token]  << "}";
                }
            }

            if(num_card == (int) m_basic_cards.size() - 1) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print type pour creature et value pour land
        for (int j = 0; j < 8; j++){

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE) ){

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                std::string types[1] = {"Angel"};
                std::vector<int> creat_type =  creature->get_types();
                std::string s = "";
                for (auto t : creat_type)
                {
                    s += types[t].substr(0, 3);
                    s += "-";
                }
                s = s.substr(0, s.size() - 2);
        
                if(m_basic_cards[num_card]->get_enchantments().empty()){
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "[*" << std::setw(10) << s.substr(0, 10) << "*]";
                    } else{
                        std::cout<< "[" << std::setw(12) << s.substr(0,12) << "]";
                    }
                } else {
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "{*" << std::setw(10) << s.substr(0, 10)  << "*}";
                    } else{
                        std::cout<< "{" << std::setw(12) << s.substr(0,12) << "}";
                    }
                }

            } else if(m_basic_cards[num_card]->is_class(Card_class::LAND) ){

                Land* land = dynamic_cast<Land*>(m_basic_cards[num_card]);
                int val = land->get_value();

                if(m_basic_cards[num_card]->get_enchantments().empty()){
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "[*" << std::setw(10) << val << "*]";
                    } else{
                        std::cout<< "[" << std::setw(12) << val << "]";
                    }
                } else {
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "{*" << std::setw(10) << val  << "*}";
                    } else{
                        std::cout<< "{" << std::setw(12) << val << "}";
                    }
                }

            }

            if(num_card == (int) m_basic_cards.size() - 1) break;
            std::cout << delimiter;

        }

        std::cout<<std::endl;

        // print power/toughness
        for (int j = 0; j < 8; j++){

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE) ){

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                int pow = creature->get_power_current();
                int tough = creature->get_toughness_current();

                std::string s = std::to_string(pow) + " / " + std::to_string(tough);
        
                if(m_basic_cards[num_card]->get_enchantments().empty()){
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "[*" << std::setw(10) << s.substr(0, 10) << "*]";
                    } else{
                        std::cout<< "[" << std::setw(12) << s.substr(0,12) << "]";
                    }
                } else {
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "{*" << std::setw(10) << s.substr(0, 10)  << "*}";
                    } else{
                        std::cout<< "{" << std::setw(12) << s.substr(0,12) << "}";
                    }
                }

            } else{
                std::cout<< empty_case;
            }

            if(num_card == (int) m_basic_cards.size() - 1) break;
            std::cout << delimiter;

        }

        std::cout<<std::endl;

        // print abilities
        for (int j = 0; j < 8; j++){

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE) ){

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                std::string abilities[] = {"Flight", "Scope", "Vigilance", "Touch_of_death", "Defender", "Initiative", "Double_initiative", "Haste", 
                           "Unblockable", "Life_link", "Threat", "Trampling", "White_protection", "Blue_protection", "Black_protection", "Red_protection", "Green_protection"};
                std::vector<int> abilities_crea =  creature->get_abilities();
                std::string s = "";
                for (auto a : abilities_crea)
                {
                    s += abilities[a].substr(0, 3);
                    s += "-";
                }
                s = s.substr(0, s.size() - 2);
        
                if(m_basic_cards[num_card]->get_enchantments().empty()){
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "[*" << std::setw(10) << s.substr(0, 10) << "*]";
                    } else{
                        std::cout<< "[" << std::setw(12) << s.substr(0,12) << "]";
                    }
                } else {
                    if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE) && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                        std::cout<< "{*" << std::setw(10) << s.substr(0, 10)  << "*}";
                    } else{
                        std::cout<< "{" << std::setw(12) << s.substr(0,12) << "}";
                    }
                }

            } else {
                std::cout<< empty_case;
            }

            if(num_card == (int) m_basic_cards.size() - 1) break;
            std::cout << delimiter;

        }


        std::cout<<std::endl;
        std::cout<<std::endl;

    }

    int bc_size = m_basic_cards.size();
    int m = ( (int) m_enchantments.size() % 8) == 0 ? 0:1;

    std::string white_effects[] = {"Win_1_HP_white", "Flight_Life_link"};
    std::string blue_effects[]  = {"Control_creature"};
    std::string black_effects[] = {"Less_HP_death_creature"};
    std::string red_effects[]   = {"More_1_0_attack_creatures"};
    std::string green_effects[] = {"More_1_land", "More_G_G_creature"};

    // print les enchantements
    for (int i = 0; i < ( (int) m_enchantments.size() / 8) + m ; i++){

        // print les numéros de cartes
        for (int j = 0; j < 8; j++){

            num_card = bc_size + i*8 + j;

            std::cout<< "[" << std::setw(12) << num_card << "]";

            

            if(num_card == ((int) m_enchantments.size() - 1 + bc_size)) break;
            std::cout << delimiter;

        }
        
        std::cout<<std::endl;

        // print type de carte
        for (int j = 0; j < 8; j++){

            num_card = bc_size + i*8 + j;

            std::cout<< "[" << std::setw(12) << "Enchantment" << "]";

            if(num_card == (int) m_enchantments.size() - 1 + bc_size) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print nom de carte
        for (int j = 0; j < 8; j++){

            num_card = bc_size + i*8 + j;

            std::string s = (m_enchantments[j]->get_name()).substr(0, 12);

            std::cout<< "[" << std::setw(12) <<  s << "]";

            if(num_card == (int) m_enchantments.size() - 1 + bc_size) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print token
        for (int j = 0; j < 8; j++){

            num_card = bc_size + i*8 + j;

            int token = m_enchantments[j]->get_token();

            std::cout<< "[" << std::setw(12) << tokens[token] << "]";

            if(num_card == (int) m_enchantments.size() - 1 + bc_size) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print first effect
        for (int j = 0; j < 8; j++){

            num_card = bc_size + i*8 + j;

            if((m_enchantments[j]->get_effects()).empty()){
                std::cout<< empty_case;
            } else{

                int effect = (m_enchantments[j]->get_effects())[0];

                switch (m_enchantments[j]->get_token()){

                    case Token::White :
                        std::cout<< "[" << std::setw(12) << white_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Blue :
                        std::cout<< "[" << std::setw(12) << blue_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Black :
                        std::cout<< "[" << std::setw(12) << black_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Red :
                        std::cout<< "[" << std::setw(12) << red_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Green :
                        std::cout<< "[" << std::setw(12) << green_effects[effect].substr(0, 12) << "]";
                    break;
                
                    default:
                        break;
                }

            }

            if(num_card == (int) m_enchantments.size() - 1 + bc_size) break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print second effect
        for (int j = 0; j < 8; j++){

            num_card = bc_size + i*8 + j;

            if((m_enchantments[j]->get_effects()).size() < 2){
                std::cout<< empty_case;
            } else{

                int effect = (m_enchantments[j]->get_effects())[1];

                switch (m_enchantments[j]->get_token()){

                    case Token::White :
                        std::cout<< "[" << std::setw(12) << white_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Blue :
                        std::cout<< "[" << std::setw(12) << blue_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Black :
                        std::cout<< "[" << std::setw(12) << black_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Red :
                        std::cout<< "[" << std::setw(12) << red_effects[effect].substr(0, 12) << "]";
                    break;

                    case Token::Green :
                        std::cout<< "[" << std::setw(12) << green_effects[effect].substr(0, 12) << "]";
                    break;
                
                    default:
                        break;
                }

            }

            if(num_card == (int) m_enchantments.size() - 1 + bc_size) break;
            std::cout << delimiter;
        }
        
        std::cout<<std::endl;
        std::cout<<std::endl;

    }

}
