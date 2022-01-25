#include <iostream>
#include <string>
#include <vector>

#include "Land.hpp"
#include "BasicCard.hpp"
#include "Enchantment.hpp"
#include "FonctionsAux.hpp"
#include "SaveParser.hpp"

Land::Land(int c, std::string name, int token, int value): Card(c, name, token), BasicCard(c, name, token), m_value(value) {
    //std::cout << "[Land] : Création de " << this << std::endl;
}

Land::~Land(){
    //std::cout << "[Land] : Destruction de " << this << std::endl;
}

int Land::get_value() const{
    return m_value;
}

void Land::set_value(int value){
    m_value = value;
}

void Land::print(){
    
    print_info("Voici les information de " + get_name() + " :");

    std::string token;
    switch (get_token())
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

    if(get_token() == 0){
        std::cout << get_color(Color::Black) << token << get_name() << " est un terrain qui rapporte " + std::to_string(m_value) + " ressources."<< get_color(Color::Reset) << get_background_color(Color::Reset)<< std::endl;
    } else {
        std::cout << token << get_name() << " est un terrain qui rapporte " + std::to_string(m_value) + " ressources."<< get_background_color(Color::Reset) << std::endl;
    }

}

std::string Land::to_string() {
    std::string s = "";
    
    s += SaveParser::begin_card + "\n";
    s += SaveParser::classcard + std::to_string(Card_class::LAND) + "\n";
    s += SaveParser::name + get_name() + "\n";
    s += SaveParser::token + std::to_string(get_token()) + "\n";
    s += SaveParser::engaged + (get_engaged() ? "1" : "0") + "\n";
    s += SaveParser::value + std::to_string(m_value) + "\n";
    s += SaveParser::begin_enchantments + "\n";
    for(auto e : get_enchantments())
        s += e->to_string();
    s += SaveParser::end_enchantments + "\n";
    s += SaveParser::end_card + "\n";
    
    return s;
}
