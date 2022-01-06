#include <iostream>
#include <string>

#include "Card.hpp"

Card::Card(std::string name, int token): m_name(name), m_token(token){}

Card::~Card(){}