#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Enchantment.hpp"
#include "FonctionsAux.hpp"
#include "SaveParser.hpp"

Enchantment::Enchantment(int c, std::string name, int token, Cost* cost, std::vector<int> effects): 
Card(c, name, token), SpecialCard(c, name, token, cost, effects) {
    //std::cout << "[Enchantment] : Création de " << this << std::endl;
}

Enchantment::~Enchantment() {
    //std::cout << "[Enchantment] : Destruction de " << this << std::endl;
}

/*
    Affiche l'Enchantement.
*/
void Enchantment::print() {
    print_info("Voici les information de " + get_name() + " :");

    std::string token;

    std::string str_effects = " a les effets suivants : ";
   
    std::string white_effects[] = {"Win_1_HP_white", "Flight_Life_link"};
    std::string blue_effects[]  = {"Control_creature"};
    std::string black_effects[] = {"Less_HP_death_creature"};
    std::string red_effects[]   = {"More_1_0_attack_creatures"};
    std::string green_effects[] = {"More_1_land", "More_G_G_creature"};

    switch (get_token()) {
        case 0:{
            token = get_background_color(Color::White);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += white_effects[get_effects()[i]];
                } else {
                    str_effects += white_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 1:{
            token = get_background_color(Color::Blue);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += blue_effects[get_effects()[i]];
                } else {
                    str_effects += blue_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 2: {
            token = get_background_color(Color::Black);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += black_effects[get_effects()[i]];
                } else {
                    str_effects += black_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 3:{
            token = get_background_color(Color::Red);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += red_effects[get_effects()[i]];
                } else {
                    str_effects += red_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 4: {
            token = get_background_color(Color::Green);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += green_effects[get_effects()[i]];
                } else {
                    str_effects += green_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        default:
            break;
    }

    Cost *c = get_cost();

    if(get_token() == 0){
        std::cout << get_color(Color::Black) << token << get_name() << " est un enchantement. ";
        std::cout<< get_name() << " vous coûte ";
        c->print();
        std::cout << std::endl << get_name() << str_effects << "." << get_color(Color::Reset) << get_background_color(Color::Reset)<< std::endl;
    } else {
        std::cout << get_color(Color::Black) << token << get_name() << " est un enchantement. ";
        std::cout<< get_name() << " vous coûte ";
        c->print();
        std::cout << std::endl << get_name() << str_effects << "." << get_color(Color::Reset) << get_background_color(Color::Reset)<< std::endl;
    }
}

/*
    Renvoie les données de l'Enchantment au le format utilisé pour sauvegarder une partie.
*/
std::string Enchantment::to_string() {
    std::string s = "";

    s += SaveParser::begin_card + "\n";
    s += SaveParser::classcard + std::to_string(Card_class::ENCHANTEMENT) + "\n";
    s += SaveParser::name + get_name() + "\n";
    s += SaveParser::token + std::to_string(get_token()) + "\n";
    s += SaveParser::effects + list_int_to_string(get_effects()) + "\n";
    s += SaveParser::cost + get_cost()->to_string() + "\n";
    s += SaveParser::end_card + "\n";
    
    return s;
}

/*
    Renvoie vrai si l'Enchantment est valide
*/
bool Enchantment::is_valid(size_t token, size_t id) {
    switch (token) {
        case Token::Black:
            return id < Black_enchantment_effects::Count;
        case Token::White:
            return id < White_enchantment_effects::Count;
        case Token::Blue:
            return id < Blue_enchantment_effects::Count;
        case Token::Red:
            return id < Red_enchantment_effects::Count;
        case Token::Green:
            return id < Green_enchantment_effects::Count;
    }
    return false;
}
