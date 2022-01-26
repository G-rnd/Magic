#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"
#include "SpecialCard.hpp"
#include "Ritual.hpp"
#include "FonctionsAux.hpp"
#include "SaveParser.hpp"

Ritual::Ritual(int c, std::string name, int token, Cost* cost, std::vector<int> effects): 
Card(c, name, token), SpecialCard(c, name, token, cost, effects) {
    //std::cout << "[Ritual] : Création de " << this << std::endl;
}

Ritual::~Ritual() {
    //std::cout << "[Ritual] : Destruction de " << this << std::endl;
}

/*
    Affiche un Ritual.
*/
void Ritual::print() {
    print_info("Voici les information de " + get_name() + " :");

    std::string token;

    std::string str_effects = " a les effets suivants : ";
   
    std::string white_ritual_effects[] = { "More_3_HP", "More_1_1_creature_current", "Destroy_engaged_creature", "Destroy_enchantment" };
    std::string blue_ritual_effects[]  = { "Draw_2_cards", "Back_hand_creature" };
    std::string black_ritual_effects[] = { "Kill_creature", "Kill_creature_2_power", "Kill_not_angel", "Less_2_2_creature_current"};
    std::string red_ritual_effects[]   = { "Damage_3_creature_or_player", "Damage_4_creatures"};
    std::string green_ritual_effects[] = { "Play_another_land", "Take_2_lands_library_shuffle"};

    switch (get_token()) {
        case 0:{
            token = get_background_color(Color::White);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += white_ritual_effects[get_effects()[i]];
                } else {
                    str_effects += white_ritual_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 1:{
            token = get_background_color(Color::Blue);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += blue_ritual_effects[get_effects()[i]];
                } else {
                    str_effects += blue_ritual_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 2: {
            token = get_background_color(Color::Black);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += black_ritual_effects[get_effects()[i]];
                } else {
                    str_effects += black_ritual_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 3:{
            token = get_background_color(Color::Red);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += red_ritual_effects[get_effects()[i]];
                } else {
                    str_effects += red_ritual_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        case 4: {
            token = get_background_color(Color::Green);
            for (size_t i = 0; i < get_effects().size(); i++){
                if(i == get_effects().size() - 1){
                    str_effects += green_ritual_effects[get_effects()[i]];
                } else {
                    str_effects += green_ritual_effects[get_effects()[i]] + ", ";
                }
            }
        }
        break;
        default:
            break;
    }

    Cost *c = get_cost();

    if(get_token() == 0){
        std::cout << get_color(Color::Black) << token << get_name() << " est un rituel. ";
        std::cout<< get_name() << " vous coûte ";
        c->print();
        std::cout << std::endl << get_name() << str_effects << "." << get_color(Color::Reset) << get_background_color(Color::Reset)<< std::endl;
    } else {
        std::cout << get_color(Color::Black) << token << get_name() << " est un rituel. ";
        std::cout<< get_name() << " vous coûte ";
        c->print();
        std::cout << std::endl << get_name() << str_effects << "." << get_color(Color::Reset) << get_background_color(Color::Reset)<< std::endl;
    }
}

/*
    Renvoie les données du Ritual au le format utilisé pour sauvegarder une partie.
*/
std::string Ritual::to_string() {
    std::string s = "";

    s += SaveParser::begin_card + "\n";
    s += SaveParser::classcard + std::to_string(Card_class::RITUAL) + "\n";
    s += SaveParser::name + get_name() + "\n";
    s += SaveParser::token + std::to_string(get_token()) + "\n";
    s += SaveParser::effects + list_int_to_string(get_effects()) + "\n";
    s += SaveParser::cost + get_cost()->to_string() + "\n";
    s += SaveParser::end_card + "\n";
    
    return s;
}