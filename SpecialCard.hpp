#ifndef SPECIALCARD_HPP
#define SPECIALCARD_HPP

#include <string>
#include <vector>

#include "Card.hpp"

class SpecialCard : public Card{

    private:
        int m_id;
        std::string m_info;

    public:
        SpecialCard(std::string, int, int, std::string);
        ~SpecialCard();

        virtual void print() = 0;

};

#endif