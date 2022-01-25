#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip> 
#include <map>

#include "Battlefield.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"
#include "SaveParser.hpp"
#include "Game.hpp"
#include "FonctionsAux.hpp"

Battlefield::Battlefield() {
    //std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::Battlefield(const std::vector<Card*>& basic_cards, const std::vector<Enchantment*>& enchantments) {
    for(auto c : basic_cards) {
        if (c->get_class() == Card_class::CREATURE)
            m_basic_cards.push_back(dynamic_cast<Creature*>(c));
        if (c->get_class() == Card_class::LAND)
            m_basic_cards.push_back(dynamic_cast<Land*>(c));
    }

    for(auto e : enchantments)
        if (e->get_class() == Card_class::ENCHANTEMENT)
            m_enchantments.push_back(dynamic_cast<Enchantment*>(e));
    //std::cout << "[Battlefield] : Création de " << this << std::endl;
}

Battlefield::~Battlefield() {
    for(auto c : m_basic_cards)
        delete c;
    
    for(auto c : m_enchantments)
        delete c;
    
    //std::cout << "[Battlefield] : Destruction de " << this << std::endl;
}

std::vector<BasicCard*> Battlefield::get_basic_cards() const {
    return m_basic_cards;
}

std::vector<Enchantment*> Battlefield::get_enchantments() const {
    return m_enchantments;
}

void Battlefield::set_basic_cards(std::vector<BasicCard*> bc) {
    m_basic_cards = bc;
}

void Battlefield::set_enchantments(std::vector<Enchantment*> e) {
    m_enchantments = e;
}

void Battlefield::sort_basic_cards() {

    std::vector<BasicCard*> bc_sort;

    for (auto bc : m_basic_cards) {
        if(bc->is_class(Card_class::LAND)) {
            bc_sort.push_back(bc);
        }
    }
    for (auto bc : m_basic_cards) {
        if(bc->is_class(Card_class::CREATURE)) {
            bc_sort.push_back(bc);
        }
    }
    set_basic_cards(bc_sort);
}

void Battlefield::remove_basic_card(BasicCard* bc) {
    remove(bc, m_basic_cards);
}

void Battlefield::remove_enchantment(Enchantment* e) {
    remove(e, m_enchantments);
}

std::vector<Creature*> Battlefield::get_engaged_creatures() {
    std::vector<Creature*> v = {};

    for (auto card : m_basic_cards) {
        if (card->is_class(Card_class::CREATURE) && card->get_engaged()) {
            v.push_back(dynamic_cast<Creature*>(card));
        }
    }
    return v;
}

std::vector<Creature*> Battlefield::get_available_creatures() {
    std::vector<Creature*> v = {};

    bool haste = false;

    for (auto card : m_basic_cards) {
        if(card->is_class(Card_class::CREATURE) && !card->get_engaged()) {
            Creature* c = dynamic_cast<Creature*>(card);
            for (auto a : c->get_abilities()){
                if(a == Ability::Haste) haste = true;
            }
            if(!c->get_is_first_turn() || haste){
                v.push_back(c);
            }
        }
    }
    return v;
}
std::vector<Land*> Battlefield::get_available_lands() {
    std::vector<Land*> v = {};
    
    for (auto card : m_basic_cards) {
        if(card->is_class(Card_class::LAND) && !card->get_engaged()) {
            v.push_back(dynamic_cast<Land*>(card));
        }
    }
    
    return v;
}

void Battlefield::place_basic_card(BasicCard* bc) {
    if(bc != nullptr)
        m_basic_cards.push_back(bc);
}

void Battlefield::place_enchantment(Enchantment* e) {
    if(e != nullptr)
        m_enchantments.push_back(e);
}

void Battlefield::disengage_card(BasicCard* bc) {
    if(bc->is_class(Card_class::CREATURE)){
        Creature* c = dynamic_cast<Creature*>(bc);
        c->set_is_first_turn(false);
    }
    bc->set_engaged(false);
}

// Renvoie vrai si le champ de bataille comporte assez de cartes terrains pour jouer la carte c
bool Battlefield::is_playable(Card* card) {
    Cost c = Cost(0, 0, 0, 0, 0, 0);
    switch (card->get_class()) {
        case Card_class::CREATURE:
            c = *(dynamic_cast<Creature*>(card)->get_cost());
            break;
        case Card_class::RITUAL:
            c = *(dynamic_cast<Ritual*>(card)->get_cost());
            break;
        case Card_class::ENCHANTEMENT:
            c = *(dynamic_cast<Enchantment*>(card)->get_cost());
            break;
        default:
            return false;
    }
    std::vector<Land*> lands = get_available_lands();

    for (size_t i = 0; i < lands.size() && !c.is_null(); i++) {
        // Si la carte n'a pas besoin de terrains typés ou que tous ses terrains colorés ont déjà été comptés
        if (c.is_color_null()) {
            c.set_any(c.get_any() - lands[i]->get_value());
            //lands.erase(lands.begin() + i);
            //i--;
        } else {
            // On commence par compter les terrains typés
            switch(lands[i]->get_token()){
                case Token::White:
                    if (c.get_white() > 0)
                        c.set_white(c.get_white() - lands[i]->get_value());
                    else 
                        c.set_any(c.get_any() - lands[i]->get_value());
                    break;

                case Token::Blue:
                    if (c.get_blue() > 0)
                        c.set_blue(c.get_blue() - lands[i]->get_value());
                    else 
                        c.set_any(c.get_any() - lands[i]->get_value());
                    break;

                case Token::Black:
                    if (c.get_black() > 0)
                        c.set_black(c.get_black() - lands[i]->get_value());
                    else 
                        c.set_any(c.get_any() - lands[i]->get_value());
                    break;

                case Token::Red:
                    if (c.get_red() > 0)
                        c.set_red(c.get_red() - lands[i]->get_value());
                    else 
                        c.set_any(c.get_any() - lands[i]->get_value());
                    break;

                case Token::Green:
                    if (c.get_green() > 0)
                        c.set_green(c.get_green() - lands[i]->get_value());
                    else 
                        c.set_any(c.get_any() - lands[i]->get_value()); 
                    break;

                default:
                    break;

            }
            //lands.erase(lands.begin() + i);
        }
    }
    return c.is_null();
}

void Battlefield::engage_lands(Cost* c){

    std::vector<Land*> lands = get_available_lands();
    std::vector<int> needed = {c->get_white(), c->get_blue(), c->get_black(), c->get_red(), c->get_green(), c->get_any()};

    for (auto land : lands){
        int value = land->get_value();
        if(needed[land->get_token()] >= 1){
            land->set_engaged(true);
            needed[land->get_token()] = needed[land->get_token()] - value; 
        } else if(needed[5] >= 1){ //any needed
            land->set_engaged(true);
            needed[5] = needed[5] - value; 
        }
    }

}

std::string Battlefield::to_string() {
    std::string s = "";
    s += SaveParser::begin_card_list + "\n";
    for(auto c : m_basic_cards)
        s += c->to_string();
    s += SaveParser::end_card_list + "\n";

    s += SaveParser::begin_enchantments + "\n";
    for(auto c : m_enchantments)
        s += c->to_string();
    s += SaveParser::end_enchantments + "\n";

    return s;
}

void Battlefield::print() {

    sort_basic_cards();
    
    std::string delimiter = "     ";
    std::string empty_case = "                 ";
    int wid = 15; // width of a card
    std::string top_card_basic  = "┌─────────────────┐";
    std::string down_card_basic = "└─────────────────┘";
    std::string top_card_ench   = "╔═════════════════╗";
    std::string down_card_ench  = "╚═════════════════╝";
    std::vector<std::string> left = {"│ ", "│╳", "║ ", "║╳"};
    std::vector<std::string> right = {" │", "╳│", " ║", "╳║"};

    size_t num_card;
    int n = (m_basic_cards.size() % 8 == 0) ? 0 : 1 ; // gérer le nb de lignes

    // print les basics_cards
    for (size_t i = 0; i < (m_basic_cards.size() / 8) + n ; i++) {

        std::vector<int> cards_states = {}; // represents the state of each card of a line of the battlefield

        // print header of the card
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->get_enchantments().empty()) {
                if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE)
                && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                    cards_states.push_back(State::Engaged);
                } else {
                    cards_states.push_back(State::Normal);
                }
                std::cout<< top_card_basic;
            } else {
                if(m_basic_cards[num_card]->get_engaged() || (m_basic_cards[num_card]->is_class(Card_class::CREATURE)
                && dynamic_cast<Creature*>(m_basic_cards[num_card])->get_is_first_turn())){
                    cards_states.push_back(State::Engaged_Enchanted);
                } else {
                    cards_states.push_back(State::Enchanted);
                }
                std::cout<< top_card_ench;
            }

            if (num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout<< std::endl;

        // print les numéros de cartes && type de carte
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;
            int diff = num_card > 9 ? 2:1;

            // print the token on the first line background
            std::string token;
            switch (m_basic_cards[num_card]->get_token())
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
            
            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE)) {
                if(m_basic_cards[num_card]->get_token() == 0){
                    std::cout << left[cards_states[j]]<< get_color(Color::Black) << token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Creature" << get_color(Color::Reset) << get_background_color(Color::Reset) << right[cards_states[j]];
                } else {
                    std::cout << left[cards_states[j]]<< token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Creature" << get_background_color(Color::Reset) << right[cards_states[j]];
                }
            } else if (m_basic_cards[num_card]->is_class(Card_class::LAND)) {
                if(m_basic_cards[num_card]->get_token() == 0){
                    std::cout << left[cards_states[j]]<< get_color(Color::Black) << token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Land" << get_color(Color::Reset) << get_background_color(Color::Reset) << right[cards_states[j]];
                } else {
                    std::cout << left[cards_states[j]]<< token << num_card << std::setw(wid-diff) << std::setfill(' ') << "Land" << get_background_color(Color::Reset) << right[cards_states[j]];
                } 
           }

            if(num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print nom de carte
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout << left[cards_states[j]] << centered_string(m_basic_cards[num_card]->get_name(), wid) << right[cards_states[j]];

            if(num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;
        
        // print cost pour creature
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE)) {
                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                Cost* cost = creature->get_cost();

                std::cout<< left[cards_states[j]] << std::setw((wid - 11)/2) << std::setfill(' ') << " ";

                std::cout<< cost->get_any() << " ";
                std::cout<< get_background_color(Color::White) << get_color(Color::Black) << cost->get_white() << get_color(Color::Reset) << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Blue) << cost->get_blue() << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Black) << cost->get_black() << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Red) << cost->get_red() << get_background_color(Color::Reset) << " ";
                std::cout<< get_background_color(Color::Green) << cost->get_green() << get_background_color(Color::Reset);
                
                std::cout<< std::setw((wid - 11)/2) << std::setfill(' ') << " " << right[cards_states[j]];
  
            } else if (m_basic_cards[num_card]->is_class(Card_class::LAND)) {

                std::cout << left[cards_states[j]] << std::setw(wid) << " " << right[cards_states[j]];
            }

            if(num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout<< std::endl;

        // print type pour creature
        for (size_t j = 0; j < 8; j++) {
            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE)) {
                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                std::vector<std::string> types{"Angel", "Beast", "Human_werewolf", "HippoGriff", "Kor_warrior", "Dinosaur", "Humans", "Vampire", "Spider", "Elf", "Troll"}; // TODO : mettre à jour
                std::vector<int> creat_type = creature->get_types();
                std::string s = "";

                if(creat_type.empty()){
                    std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
                } else if(creat_type.size() == 1){
                    std::cout<< left[cards_states[j]] << centered_string(types[creat_type[0]], wid) << right[cards_states[j]];
                } else if(creat_type.size() == 2){
                    s = types[creat_type[0]] + types[creat_type[1]];
                    std::cout<< left[cards_states[j]] << centered_string(s, wid) << right[cards_states[j]];
                } else {
                    // TODO
                }

                
            } else if (m_basic_cards[num_card]->is_class(Card_class::LAND)) {
                std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
            }

            if (num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout<<std::endl;

        // print empty line
        for (size_t j = 0; j < 8; j++) {
            num_card = i*8 + j;

            std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];

            if (num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout<<std::endl;

        // print abilities
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                std::string abilities[] = {"Flight", "Scope", "Vigilance", "Touch_of_death", "Defender", "Initiative", "Double_initiative", "Haste", 
                           "Unblockable", "Life_link", "Threat", "Trampling", "White_protection", "Blue_protection", "Black_protection", "Red_protection", "Green_protection"};
                std::vector<int> abilities_crea =  creature->get_abilities();
                std::string s = "";
                
                if(abilities_crea.empty()){
                    std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
                } else if(abilities_crea.size() == 1){
                    std::cout<< left[cards_states[j]] << centered_string(abilities[abilities_crea[0]], wid) << right[cards_states[j]];
                } else {
                    s = abilities[abilities_crea[0]] + " " + abilities[abilities_crea[1]];
                    std::cout<< left[cards_states[j]] << centered_string(s, wid) << right[cards_states[j]];
                }
                
            } else if (m_basic_cards[num_card]->is_class(Card_class::LAND)) {
                std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
            }

            if(num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print abilities
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                std::string abilities[] = {"Flight", "Scope", "Vigilance", "Touch_of_death", "Defender", "Initiative", "Double_initiative", "Haste", 
                           "Unblockable", "Life_link", "Threat", "Trampling", "White_protection", "Blue_protection", "Black_protection", "Red_protection", "Green_protection"};
                std::vector<int> abilities_crea =  creature->get_abilities();
                std::string s = "";
                
                if(abilities_crea.size() == 3){
                    std::cout<< left[cards_states[j]] << centered_string(abilities[abilities_crea[2]], wid) << right[cards_states[j]];
                } else if(abilities_crea.size() > 3){
                    s = abilities[abilities_crea[2]] + " " +  abilities[abilities_crea[3]];
                    std::cout<< left[cards_states[j]] << centered_string(s, wid) << right[cards_states[j]];
                } else {
                    std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
                }
                
            } else if (m_basic_cards[num_card]->is_class(Card_class::LAND)) {
                std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
            }

            if(num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout<<std::endl;

        // print power/toughness for creatures and value for lands
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->is_class(Card_class::CREATURE)) {

                Creature* creature = dynamic_cast<Creature*>(m_basic_cards[num_card]);
                int pow = creature->get_power_current();
                int tough = creature->get_toughness_current();

                std::string s = std::to_string(pow) + " / " + std::to_string(tough);

                std::cout<< left[cards_states[j]] << std::setw(wid) << s << right[cards_states[j]];

            } else if (m_basic_cards[num_card]->is_class(Card_class::LAND)){
                Land* land = dynamic_cast<Land*>(m_basic_cards[num_card]);
                
                std::cout<< left[cards_states[j]] << std::setw(wid) << land->get_value() << right[cards_states[j]];
            }

            if (num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // print bottom of the card
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if (m_basic_cards[num_card]->get_enchantments().empty()) {
                std::cout<< down_card_basic;
            } else {
                std::cout<< down_card_ench;
            }

            if (num_card == m_basic_cards.size() - 1)
                break;
            std::cout << delimiter;
        } 

        std::cout << std::endl;
        std::cout << std::endl;
    }

    size_t bc_size = m_basic_cards.size() - 1;
    size_t m = (m_enchantments.size() % 8) == 0 ? 0 : 1;

    std::string white_effects[] = {"Win_1_HP_white", "Flight_Life_link"};
    std::string blue_effects[]  = {"Control_creature"};
    std::string black_effects[] = {"Less_HP_death_creature"};
    std::string red_effects[]   = {"More_1_0_attack_creatures"};
    std::string green_effects[] = {"More_1_land", "More_G_G_creature"};

    // print les enchantements
    for (size_t i = 0; i < (m_enchantments.size()/8) + m; i++) {

        std::vector<int> cards_states = {}; // represents the state of each card of a line of the battlefield

        // print header of the card
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            cards_states.push_back(State::Normal);
            std::cout<< top_card_basic;

            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout<< std::endl;

        // print les numéros de cartes && type de carte
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;
            int diff = num_card + bc_size > 9 ? 2:1;

            // print the token on the first line background
            std::string token;
            switch (m_enchantments[num_card]->get_token())
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
            
            if(m_enchantments[num_card]->get_token() == 0){
                std::cout << left[cards_states[j]]<< get_color(Color::Black) << token << num_card + bc_size << std::setw(wid-diff) << std::setfill(' ') << "Enchantment" << get_color(Color::Reset) << get_background_color(Color::Reset) << right[cards_states[j]];
            } else {
                std::cout << left[cards_states[j]]<< token << num_card + bc_size << std::setw(wid-diff) << std::setfill(' ') << "Enchantment" << get_background_color(Color::Reset) << right[cards_states[j]];
            }

            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print nom de carte
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout << left[cards_states[j]] << centered_string(m_enchantments[num_card]->get_name(), wid) << right[cards_states[j]];

            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;
        
        // print cost
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            Enchantment* enchantment = m_enchantments[num_card];
            Cost* cost = enchantment->get_cost();

            std::cout<< left[cards_states[j]] << std::setw((wid - 11)/2) << std::setfill(' ') << " ";

            std::cout<< cost->get_any() << " ";
            std::cout<< get_background_color(Color::White) << get_color(Color::Black) << cost->get_white() << get_color(Color::Reset) << get_background_color(Color::Reset) << " ";
            std::cout<< get_background_color(Color::Blue) << cost->get_blue() << get_background_color(Color::Reset) << " ";
            std::cout<< get_background_color(Color::Black) << cost->get_black() << get_background_color(Color::Reset) << " ";
            std::cout<< get_background_color(Color::Red) << cost->get_red() << get_background_color(Color::Reset) << " ";
            std::cout<< get_background_color(Color::Green) << cost->get_green() << get_background_color(Color::Reset);
            
            std::cout<< std::setw((wid - 11)/2) << std::setfill(' ') << " " << right[cards_states[j]];


            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;

        }

        std::cout << std::endl;

        // empty line
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];

            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print first effect
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if ((m_enchantments[j]->get_effects()).empty()) {
                std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
            } else {

                int effect = (m_enchantments[j]->get_effects())[0];

                switch (m_enchantments[j]->get_token()){

                    case Token::White:
                        std::cout<< left[cards_states[j]]<< centered_string(white_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Blue:
                        std::cout<< left[cards_states[j]]<< centered_string(blue_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Black:
                        std::cout<< left[cards_states[j]]<< centered_string(black_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Red:
                        std::cout<< left[cards_states[j]]<< centered_string(red_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Green:
                        std::cout<< left[cards_states[j]]<< centered_string(green_effects[effect], wid) << right[cards_states[j]];
                        break;
                
                    default:
                        break;
                }
            }

            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print second effect
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            if ((m_enchantments[j]->get_effects()).size() < 2) {
                std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
            } else {

                int effect = (m_enchantments[j]->get_effects())[1];

                switch (m_enchantments[j]->get_token()){

                    case Token::White:
                        std::cout<< left[cards_states[j]]<< centered_string(white_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Blue:
                        std::cout<< left[cards_states[j]]<< centered_string(blue_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Black:
                        std::cout<< left[cards_states[j]]<< centered_string(black_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Red:
                        std::cout<< left[cards_states[j]]<< centered_string(red_effects[effect], wid) << right[cards_states[j]];
                        break;

                    case Token::Green:
                        std::cout<< left[cards_states[j]]<< centered_string(green_effects[effect], wid) << right[cards_states[j]];
                        break;
                
                    default:
                        break;
                }
            }
            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // empty line
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
            
            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // empty line
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout<< left[cards_states[j]] << std::setw(wid) << std::setfill(' ') << " " << right[cards_states[j]];
            
            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        }

        std::cout << std::endl;

        // print bottom of the card
        for (size_t j = 0; j < 8; j++) {

            num_card = i*8 + j;

            std::cout<< down_card_basic;

            if (num_card == m_enchantments.size() - 1)
                break;
            std::cout << delimiter;
        } 

        std::cout << std::endl;
        std::cout << std::endl;
    }
    
}
