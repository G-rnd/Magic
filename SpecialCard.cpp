#include <iostream>
#include <string>
#include <vector>

#include "SpecialCard.hpp"
#include "Card.hpp"

SpecialCard::SpecialCard(std::string name, int token, int id, std::string info): 
Card(name, token), m_id(id), m_info(info){}

SpecialCard::~SpecialCard(){}

