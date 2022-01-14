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
    std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard*> Battlefield::get_basic_cards() const{
    return m_basic_cards;
}

std::vector<Enchantment*> Battlefield::get_enchantments() const{
    return m_enchantments;
}

void Battlefield::set_basic_cards(std::vector<BasicCard*> bc){
    // TODO : à verifier que ça fonctionne
    m_basic_cards = bc;
}

void Battlefield::set_enchantments(std::vector<Enchantment*> e){
    // TODO : à verifier que ça fonctionne
    m_enchantments = e;
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
        if(instanceof<Creature>(card) && card->get_engaged()) {
            v.push_back(dynamic_cast<Creature*>(card));
        }
    }
    
    return v;
}

std::vector<Creature*> Battlefield::get_available_creatures() {
    std::vector<Creature*> v = {};

    for (auto card : m_basic_cards) {
        if(instanceof<Creature>(card) && !card->get_engaged()){
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
        if(instanceof<Land>(card) && !card->get_engaged()){
            v.push_back(dynamic_cast<Land*>(card));
        }
    }
    
    return v;
}

void Battlefield::place_basic_card(BasicCard* bc) {
    if(bc != nullptr)
        m_basic_cards.push_back(bc);
}

void Battlefield::disengage_card(BasicCard* bc){
    bc->set_engaged(false);
}

// Renvoie vrai si le terrain comporte assez de cartes terrains pour jouer la carte c
bool Battlefield::is_playable(Creature creature) {
    
    Cost c = Cost(*(creature.get_cost()));
    std::vector<Land*> lands = get_available_lands();
    for(long unsigned int i = 0; i < lands.size() && !c.is_null(); i++) {
        // Si la carte n'a pas besoin de terrains typés
        if (c.is_color_null()) {
            c.set_any(c.get_any() - lands[i]->get_value());
            lands.erase(lands.begin() + i);
            i--;
        } else {
            // TODO changer les == 0 par == Color::white etc

            // On commence par compter les terrains typés
            if (lands[i]->get_token() == 0) {
                c.set_white(c.get_white() - lands[i]->get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i]->get_token() == 1) {
                c.set_black(c.get_black() - lands[i]->get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i]->get_token() == 2) {
                c.set_red(c.get_red() - lands[i]->get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i]->get_token() == 3) {
                c.set_green(c.get_green() - lands[i]->get_value());
                lands.erase(lands.begin() + i);
            }

            if (lands[i]->get_token() == 4) {
                c.set_blue(c.get_blue() - lands[i]->get_value());
                lands.erase(lands.begin() + i);
            }
        }
    }
    return c.is_null();
}

void Battlefield::print() {
    
    int num_card;
    int n = (m_basic_cards.size() % 5 == 0) ? 0:1 ; // gérer le nb de lignes

    // print les basics_cards
    for (int i = 0; i < (m_basic_cards.size() / 5) + n ; i++){

        // print les numéros de cartes
        for (int j = 0; j < 5; j++){

            num_card = i*5 + j;

            if(m_basic_cards[num_card]->get_enchantments().empty()){
                if(m_basic_cards[num_card]->get_engaged()){
                    std::cout<< "[*" << std::setw(10) << num_card << "*]";
                } else{
                    std::cout<< "[" << std::setw(12) << num_card << "]";
                }
            } else {
                if(m_basic_cards[num_card]->get_engaged()){
                    std::cout<< "{*" << std::setw(10) << num_card << "*}";
                } else{
                    std::cout<< "{" << std::setw(12) << num_card << "}";
                }
            }

            if(num_card == m_basic_cards.size()) break;

        }

        std::cout<<std::endl;

        // print type de carte
        for (int j = 0; j < 5; j++){

            std::cout<< "AA"<<std::endl;

            num_card = i*5 + j;

            if(instanceof<Creature>(m_basic_cards[num_card])){

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                if(creature->get_enchantments().empty()){
                    if(creature->get_engaged()){
                        std::cout<< "[*" << std::setw(10) << "Creature" << "*]";
                    } else{
                        std::cout<< "[" << std::setw(12) << "Creature" << "]";
                    }
                } else {
                    if(m_basic_cards[num_card]->get_engaged()){
                        std::cout<< "{*" << std::setw(10) << "Creature" << "*}";
                    } else{
                        std::cout<< "{" << std::setw(12) << "Creature" << "}";
                    }
                }

            } else if(instanceof<Land>(m_basic_cards[num_card])){

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

            if(num_card == m_basic_cards.size()) break;

        }

        std::cout<<std::endl;

        // print nom de carte
        for (int j = 0; j < 5; j++){

            num_card = i*5 + j;

            std::string s = (m_basic_cards[num_card]->get_name()).substr(0, 12);

            if(m_basic_cards[num_card]->get_enchantments().empty()){
                if(m_basic_cards[num_card]->get_engaged()){
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

            if(num_card == m_basic_cards.size()) break;

        }

        std::cout<<std::endl;

    }

    // print les enchantements
    for (int i = 0; i < (m_basic_cards.size() / 5) + 1 ; i++){

        for (int j = 0; j < 6; j++){

            if(num_card < 10){
                std::cout<< "[" << num_card << "         ]   ";
            } else {
                std::cout<< "[" << num_card << "        ]   ";
            }
            num_card++;

        }

        std::cout<<std::endl;

        for (auto c : m_basic_cards){
            
            if(instanceof<Creature>(c)){

                

            } else if(instanceof<Land>(c)){

            }

        }
        

    }


    

}
