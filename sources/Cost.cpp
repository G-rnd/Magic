#include <iostream>
#include <string>
#include <vector>

#include "../includes/Cost.hpp"

Cost::Cost(int any, int white, int black, int red, int green, int blue): 
m_any(any), m_white(white), m_black(black), m_red(red), m_green(green), m_blue(blue){
    std::cout << "[Cost] : Création de " << this << std::endl;
}

Cost::~Cost(){
    std::cout << "[Cost] : Destruction de " << this << std::endl;
}

int Cost::get_any() const{
    return m_any;
}

int Cost::get_white() const{
    return m_white;
}

int Cost::get_black() const{
    return m_black;
}

int Cost::get_red() const{
    return m_red;
}

int Cost::get_green() const{
    return m_green;
}

int Cost::get_blue() const{
    return m_blue;
}

void Cost::set_any(int any){
    m_any = any;
}

void Cost::set_white(int white){
    m_white = white;
}

void Cost::set_black(int black){
    m_black = black;
}

void Cost::set_red(int red){
    m_red = red;
}

void Cost::set_green(int green){
    m_green = green;
}

void Cost::set_blue(int blue){
    m_blue = blue;
}

void Cost::print() const{
    // TODO : print
}