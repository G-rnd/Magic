#include <iostream>
#include <string>
#include <vector>

#include "Cost.hpp"

Cost::Cost(int any, int white, int black, int red, int green, int blue): 
m_any(any), m_white(white), m_black(black), m_red(red), m_green(green), m_blue(blue) {
    //std::cout << "[Cost] : Création de " << this << std::endl;
}

Cost::Cost(const Cost &other) : m_any(other.m_any), m_white(other.m_white), m_black(other.m_black), m_red(other.m_red), m_green(other.m_green), m_blue(other.m_blue) {}

Cost::~Cost() {
    //std::cout << "[Cost] : Destruction de " << this << std::endl;
}

int Cost::get_any() const {
    return m_any;
}

int Cost::get_white() const {
    return m_white;
}

int Cost::get_black() const {
    return m_black;
}

int Cost::get_red() const {
    return m_red;
}

int Cost::get_green() const {
    return m_green;
}

int Cost::get_blue() const {
    return m_blue;
}

void Cost::set_any(int any) {
    m_any = any;
}

void Cost::set_white(int white) {
    m_white = white;
}

void Cost::set_black(int black) {
    m_black = black;
}

void Cost::set_red(int red) {
    m_red = red;
}

void Cost::set_green(int green) {
    m_green = green;
}

void Cost::set_blue(int blue) {
    m_blue = blue;
}

void Cost::print() const {
    // TODO : print (mais probablement pas besoin)
}

bool Cost::is_null() {
    return is_any_null() && is_color_null();
}

bool Cost::is_any_null() {
    return m_any <= 0;
}

bool Cost::is_color_null() {
    return is_white_null() && is_black_null() && is_red_null() && is_green_null() && is_blue_null();
}

bool Cost::is_white_null() {
    return m_white <= 0;
}

bool Cost::is_black_null() {
    return m_black <= 0;
}

bool Cost::is_red_null() {
    return m_red <= 0;
}

bool Cost::is_green_null() {
    return m_green <= 0;
}

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
