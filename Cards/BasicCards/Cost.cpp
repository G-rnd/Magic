#include <iostream>
#include <string>
#include <vector>

#include "Cost.hpp"

Cost::Cost(int any, int white, int black, int red, int green, int blue): 
m_any(any), m_white(white), m_black(black), m_red(red), m_green(green), m_blue(blue){}

Cost::~Cost(){}

int Cost::get_any() const{
    return this->m_any;
}

int Cost::get_white() const{
    return this->m_white;
}

int Cost::get_black() const{
    return this->m_black;
}

int Cost::get_red() const{
    return this->m_red;
}

int Cost::get_green() const{
    return this->m_green;
}

int Cost::get_blue() const{
    return this->m_blue;
}

int Cost::set_any(int any){
    this->m_any = any;
}

int Cost::set_white(int white){
    this->m_white = white;
}

int Cost::set_black(int black){
    this->m_black = black;
}

int Cost::set_red(int red){
    this->m_red = red;
}

int Cost::set_green(int green){
    this->m_green = green;
}

int Cost::set_blue(int blue){
    this->m_blue = blue;
}
