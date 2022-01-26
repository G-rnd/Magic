#include <iostream>
#include <string>
#include <vector>

#include "Cost.hpp"

Cost::Cost(int any, int white, int blue, int black, int red, int green): 
m_any(any), m_white(white), m_blue(blue), m_black(black), m_red(red), m_green(green) {
    //std::cout << "[Cost] : Création de " << this << std::endl;
}

Cost::Cost(const Cost &other) : m_any(other.m_any), m_white(other.m_white), m_blue(other.m_blue), m_black(other.m_black), m_red(other.m_red), m_green(other.m_green){}

Cost::~Cost() {
    //std::cout << "[Cost] : Destruction de " << this << std::endl;
}

/*
    Retourne l'attribut m_any de Cost
*/
int Cost::get_any() const {
    return m_any;
}

/*
    Retourne l'attribut m_white de Cost
*/
int Cost::get_white() const {
    return m_white;
}

/*
    Retourne l'attribut m_black de Cost
*/
int Cost::get_black() const {
    return m_black;
}

/*
    Retourne l'attribut m_red de Cost
*/
int Cost::get_red() const {
    return m_red;
}

/*
    Retourne l'attribut m_green de Cost
*/
int Cost::get_green() const {
    return m_green;
}

/*
    Retourne l'attribut m_blue de Cost
*/
int Cost::get_blue() const {
    return m_blue;
}

/*
    Met à jour l'attribut m_any de Cost
*/
void Cost::set_any(int any) {
    m_any = any;
}

/*
    Met à jour l'attribut m_white de Cost
*/
void Cost::set_white(int white) {
    m_white = white;
}

/*
    Met à jour l'attribut m_black de Cost
*/
void Cost::set_black(int black) {
    m_black = black;
}

/*
    Met à jour l'attribut m_red de Cost
*/
void Cost::set_red(int red) {
    m_red = red;
}

/*
    Met à jour l'attribut m_green de Cost
*/
void Cost::set_green(int green) {
    m_green = green;
}

/*
    Met à jour l'attribut m_blue de Cost
*/
void Cost::set_blue(int blue) {
    m_blue = blue;
}

/*
    Met à jour l'attribut m_any de Cost
*/
void Cost::print() const {
    if(m_white > 0){
        std::cout << m_white << " terrains blancs, ";
    }
    if(m_blue > 0){
        std::cout << m_blue << " terrains bleus, ";
    }
    if(m_black > 0){
        std::cout << m_black << " terrains noirs,";
    }
    if(m_red > 0){
        std::cout << m_red << " terrains rouges,";
    }
    if(m_green > 0){
        std::cout << m_green << " terrains verts,";
    }
    if(m_any > 0){
        std::cout << m_any << " terrains aléatoires,";
    }
}

/*
    Retoure vrai si tous les champs de Cost sont à 0.
*/
bool Cost::is_null() {
    return is_any_null() && is_color_null();
}

/*
    Retourne vrai si le champ m_any est à 0.
*/
bool Cost::is_any_null() {
    return m_any <= 0;
}

/*
    Retourne vrai si les champs de couleurs sont à 0.
*/
bool Cost::is_color_null() {
    return is_white_null() && is_black_null() && is_red_null() && is_green_null() && is_blue_null();
}

/*
    Retourne vrai si le champ m_white est à 0.
*/
bool Cost::is_white_null() {
    return m_white <= 0;
}

/*
    Retourne vrai si le champ m_black est à 0.
*/
bool Cost::is_black_null() {
    return m_black <= 0;
}

/*
    Retourne vrai si le champ m_red est à 0.
*/
bool Cost::is_red_null() {
    return m_red <= 0;
}

/*
    Retourne vrai si le champ m_green est à 0.
*/
bool Cost::is_green_null() {
    return m_green <= 0;
}

/*
    Retourne vrai si le champ m_blue est à 0.
*/
bool Cost::is_blue_null() {
    return m_blue <= 0;
}

Cost& Cost::operator=(const Cost& o) {
    m_any   = o.m_any;
    m_white = o.m_white;
    m_black = o.m_black;
    m_red   = o.m_red;
    m_green = o.m_green;
    m_blue  = o.m_blue;

    return *this;
}

/*
    Renvoie les données du Cost au le format utilisé pour sauvegarder une partie
*/
std::string Cost::to_string() {
    return std::to_string(m_any) + ", " + std::to_string(m_white) + ", " + std::to_string(m_blue) + ", " + std::to_string(m_black) + ", " + std::to_string(m_red) + ", " + std::to_string(m_green);
}